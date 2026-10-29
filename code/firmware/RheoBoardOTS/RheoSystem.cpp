// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include "RheoSystem.h"
#include "OSCHandler.h"
#include "BLEHandler.h"
#include "MemoryStream.h"

RheoSystem rheo;

// Pressure notifies go out without the library's per-sample USB log. That
// print blocks on a slow host and was stretching each slot well past 10 ms.
static void notifyOsc(const char* address, const int32_t* arg) {
  OSCMessage msg(address);
  if (arg) msg.add(*arg);
  uint8_t buf[64];
  MemoryStream stream(buf, sizeof(buf));
  msg.send(stream);
  if (pTxCharacteristic == nullptr) return;
  pTxCharacteristic->setValue(buf, stream.length());
  pTxCharacteristic->notify();
}

#if SERIAL_STREAM
struct RepMark {
  unsigned long ms;
  const char* label;
};
static RepMark repMarks[12];
static int repMarkCount = 0;

static void dumpRepMarks(int samples) {
  Serial.println("#REP_START");
  for (int i = 0; i < repMarkCount; i++) {
    Serial.print("#PH,");
    Serial.print(static_cast<long>(repMarks[i].ms));
    Serial.print(',');
    Serial.println(repMarks[i].label);
  }
  Serial.print("#REP_SAMPLES,");
  Serial.println(samples);
  Serial.println("#REP_END");
  repMarkCount = 0;
}
#endif

void RheoSystem::blow(int power) { PneumaticSystem::blow(power < 0 ? extrudePctMax : power); }
void RheoSystem::suck(int power) { PneumaticSystem::suck(power < 0 ? retractPct    : power); }

bool RheoSystem::actuationBusy() const {
  return PneumaticSystem::actuationBusy() || recording || triadActive
      || bubbleActive || bubbleTriadActive || program.isRunning();
}

unsigned long RheoSystem::sessionStart() const {
  if (bubbleActive && bubbleStartedAt > 0) return bubbleStartedAt;
  return startedAt;
}

void RheoSystem::setup() { PneumaticSystem::setup(); }

void RheoSystem::loop() {
  update();
  program.update();
  if (recording && bubbleActive) sampleAir();
  PneumaticSystem::loop();
}

void RheoSystem::captureDueRepSlots() {
  if (startedAt == 0 || repSamplesSent >= REP_GRID_COUNT) return;
  if (millis() - startedAt < static_cast<unsigned long>(repSamplesSent) * REP_GRID_MS) return;
  // One conversion. If it overruns the next slot, hold this reading through
  // those indexes so index k is still the k*18 ms sample.
  const int32_t pa = static_cast<int32_t>(readPressure());
  unsigned long slot = (millis() - startedAt) / REP_GRID_MS;
  if (slot >= REP_GRID_COUNT) slot = REP_GRID_COUNT - 1;
  while (repSamplesSent <= static_cast<int>(slot) && repSamplesSent < REP_GRID_COUNT) {
    repGridPa[repSamplesSent++] = pa;
  }
}

void RheoSystem::transmitRepSlack() {
  if (repSamplesFlushed >= repSamplesSent || startedAt == 0) return;
  if (repSamplesSent >= REP_GRID_COUNT) return;
  const unsigned long nextDue = static_cast<unsigned long>(repSamplesSent) * REP_GRID_MS;
  // Keep the next conversion on its mark. A notify only goes out in the slack.
  if (millis() - startedAt + 2UL >= nextDue) return;
  notifyOsc("/rheo/sense/air", &repGridPa[repSamplesFlushed]);
  repSamplesFlushed++;
}

void RheoSystem::flushRepSamples() {
  while (repSamplesFlushed < repSamplesSent) {
    notifyOsc("/rheo/sense/air", &repGridPa[repSamplesFlushed]);
    repSamplesFlushed++;
  }
}

void RheoSystem::startRep() {
  if (program.isRunning() || manualBlowing || latchedSuck || testStreaming
      || bubbleActive || bubbleTriadActive || purgeActive || timedAirActive) return;
  cancelSuck();
  recording = true;
  repSamplesSent = 0;
  repSamplesFlushed = 0;
#if SERIAL_STREAM
  repMarkCount = 0;
#endif
  notifyOsc("/db/start", nullptr);
  startedAt = millis();
  pumpsOff();
  phase = BASELINE; pumpOn = false;
  captureDueRepSlots();
  repEvent("BASELINE");
}

void RheoSystem::startRepTriad() {
  if (program.isRunning() || recording || manualBlowing || latchedSuck || testStreaming
      || bubbleActive || bubbleTriadActive || purgeActive || timedAirActive) return;
  triadActive    = true;
  triadCount     = 3;
  triadWaitUntil = 0;
  startRep();
  triadCount--;
}

void RheoSystem::stopRep(bool cancelTriad) {
  recording = false;
  // PA `'3'` / switchOffPumps: pump channels full-off, valve left as-is.
  // A finished REP is already blow, so channel 8 stays full-on through the
  // next baseline. systemOff() and disconnect still release it.
  pumpsOffNow();
  phase = IDLE;
  // The grid is already in repGridPa. Send it, then /db/save, before any USB
  // log. A notify inside a slot would move the index off the 18 ms clock.
  flushRepSamples();
  notifyOsc("/db/save", nullptr);
  repEvent("STOP");
  const int samples = repSamplesSent;
  startedAt = 0;
#if SERIAL_STREAM
  dumpRepMarks(samples);
#else
  (void)samples;
#endif
  delay(REP_SAVE_FLUSH_MS);

  if (cancelTriad) {
    triadActive    = false;
    triadCount     = 0;
    triadWaitUntil = 0;
    return;
  }
  if (triadActive) {
    if (triadCount > 0) triadWaitUntil = millis() + (unsigned long)repIntervalMs;
    else                triadActive    = false;
  }
}

void RheoSystem::startBubble() {
  if (program.isRunning() || recording || manualBlowing || latchedSuck || testStreaming
      || triadActive || purgeActive || timedAirActive) return;
  cancelSuck();
  recording = true;
  bubbleActive = true;
  sendSimpleOSC("/db/start");
#if SERIAL_STREAM
  Serial.print("#BUBBLE_START,"); Serial.println(readPressure(), 1);
#endif
  bubbleStartedAt = millis();
  bubblePulseOn = true;
  bubblePulseEndsAt = bubbleStartedAt + BUBBLE_PULSE_MS;
  phase = IDLE;
  pumpOn = false;
  pumpsOff();
  valveBlow();
  pumpSetPct(PUMP_2, BUBBLE_PULSE_PCT);
}

void RheoSystem::startBubbleTriad() {
  if (program.isRunning() || recording || manualBlowing || latchedSuck || testStreaming
      || triadActive || purgeActive || timedAirActive) return;
  bubbleTriadActive = true;
  bubbleTriadCount = 3;
  bubbleWaitUntil = 0;
  startBubble();
  bubbleTriadCount--;
}

void RheoSystem::stopBubble(bool cancelTriad) {
  recording = false;
  bubbleActive = false;
  bubblePulseOn = false;
  // SlipThing stopBubble is pumpsOff() (`'3'`): pumps off, valve unchanged.
  // A finished 50 ms gate already called valveSuck(); an early abort leaves blow.
  pumpsOffNow();
  phase = IDLE;
  pumpOn = false;
  bubbleStartedAt = 0;
#if SERIAL_STREAM
  Serial.println("#BUBBLE_END");
#endif
  sendSimpleOSC("/db/save");
  delay(REP_SAVE_FLUSH_MS);
  if (cancelTriad) {
    bubbleTriadActive = false;
    bubbleTriadCount = 0;
    bubbleWaitUntil = 0;
    return;
  }
  if (bubbleTriadActive) {
    if (bubbleTriadCount > 0) bubbleWaitUntil = millis() + (unsigned long)repIntervalMs;
    else bubbleTriadActive = false;
  }
}

void RheoSystem::update() {
  phaseUpdate();
  if (recording && !bubbleActive) {
    captureDueRepSlots();
    transmitRepSlack();
  }
  phaseUpdate();
  // 66 slots are read on the SlipThing clock (0, 18, … 1170 ms).
  // Slack between reads sends them; stopRep sends the rest, then /db/save.
  if (startedAt > 0 && !bubbleActive &&
      (repSamplesSent >= REP_GRID_COUNT || millis() - startedAt > REP_TIME + 300UL)) {
    stopRep(false);
  }
  if (bubblePulseOn && (long)(millis() - bubblePulseEndsAt) >= 0) {
    pumpsOff();
    valveSuck();
    bubblePulseOn = false;
  }
  if (bubbleStartedAt > 0 && millis() - bubbleStartedAt > BUBBLE_TIME) stopBubble(false);

  if (triadActive && !recording && triadWaitUntil > 0 && millis() >= triadWaitUntil) {
    triadWaitUntil = 0;
    if (triadCount > 0) { startRep(); triadCount--; }
    else                triadActive = false;
  }
  if (bubbleTriadActive && !recording && bubbleWaitUntil > 0 && millis() >= bubbleWaitUntil) {
    bubbleWaitUntil = 0;
    if (bubbleTriadCount > 0) {
      startBubble();
      bubbleTriadCount--;
    } else {
      bubbleTriadActive = false;
    }
  }
}

void RheoSystem::repEvent(const char* label) {
#if SERIAL_STREAM
  if (repMarkCount >= 12) return;
  repMarks[repMarkCount].ms = startedAt ? millis() - startedAt : 0;
  repMarks[repMarkCount].label = label;
  repMarkCount++;
#else
  (void)label;
#endif
}

int RheoSystem::calcExtrudePct(unsigned long pe) {
  if (extrudeRampMs > 0 && pe < (unsigned long)extrudeRampMs) {
    long span = (long)extrudePctMax - extrudeRampStartPct;
    return (int)(extrudeRampStartPct + span * (long)pe / extrudeRampMs);
  }
  return extrudePctMax;
}

// Every phase edge is a fixed offset from startedAt, so a late poll never pushes
// the later phases back. Baseline does not move the valve. Retract begins with
// valveSuck(); extrude begins with valveBlow(); relax leaves that blow on.
void RheoSystem::phaseUpdate() {
  if (phase == IDLE || phase == RELAX) return;
  const unsigned long t = millis() - startedAt;
  const unsigned long pullValveAt = (unsigned long)baselineMs;
  const unsigned long pullOnAt    = pullValveAt + VALVE_SETTLE_MS;
  const unsigned long pullOffAt   = pullOnAt + (unsigned long)retractMs;
  const unsigned long pushValveAt = pullOffAt + (unsigned long)pushDelayMs;
  const unsigned long pushOnAt    = pushValveAt + VALVE_SETTLE_MS;
  const unsigned long pushOffAt   = pushOnAt + (unsigned long)extrudeMs;

  for (;;) {
    switch (phase) {
      case BASELINE:
        if (t < pullValveAt) return;
        pumpsOff(); valveSuck();
        phase = RETRACT; pumpOn = false;
        repEvent("RETRACT_VALVE");
        break;
      case RETRACT:
        if (!pumpOn) {
          if (t < pullOnAt || !pwmReady()) return;
          pumpSetPct(PUMP_1, retractPct);
          pumpOn = true;
          repEvent("RETRACT_PUMP");
        } else {
          if (t < pullOffAt) return;
          pumpSet(PUMP_1, false);
          pumpOn = false;
          if (pushDelayMs > 0) {
            phase = PUSH_DELAY;
            repEvent("PUSH_DELAY");
          } else {
            valveBlow();
            phase = EXTRUDE;
            repEvent("EXTRUDE_VALVE");
          }
        }
        break;
      case PUSH_DELAY:
        if (t < pushValveAt) return;
        valveBlow();
        phase = EXTRUDE; pumpOn = false;
        repEvent("EXTRUDE_VALVE");
        break;
      case EXTRUDE:
        if (!pumpOn) {
          if (t < pushOnAt || !pwmReady()) return;
          extrudeNowPct = calcExtrudePct(0);
          pumpSetPct(PUMP_2, extrudeNowPct);
          pumpOn = true;
          repEvent("EXTRUDE_PUMP");
        } else if (t < pushOffAt) {
          const int pct = calcExtrudePct(t - pushOnAt);
          if (pct != extrudeNowPct) {
            extrudeNowPct = pct;
            pumpSetPct(PUMP_2, pct);
          }
          return;
        } else if (pushFallMs <= 0) {
          pumpsOff();
          phase = RELAX; pumpOn = false;
          repEvent("RELAX");
        } else {
          phase = PUSH_FALL;
          fallFromPct = extrudePctMax < 1 ? 1 : extrudePctMax;
          fallStep = -1;
          repEvent("PUSH_FALL");
        }
        break;
      case PUSH_FALL: {
        const unsigned long fe = t - pushOffAt;
        if (fe >= (unsigned long)pushFallMs) {
          pumpsOff();
          phase = RELAX; pumpOn = false;
          fallStep = -1;
          repEvent("RELAX");
          break;
        }
        const int steps = 8;
        int step = (int)((fe * (unsigned long)steps) / (unsigned long)pushFallMs);
        if (step >= steps) step = steps - 1;
        if (step != fallStep) {
          fallStep = step;
          int floorPct = REP_PUSH_FALL_FLOOR_PCT;
          if (floorPct >= fallFromPct) floorPct = fallFromPct > 1 ? fallFromPct - 1 : 1;
          int pct = fallFromPct - ((fallFromPct - floorPct) * step) / (steps - 1);
          if (pct < 1) pct = 1;
          pumpSetPct(PUMP_2, pct);
        }
        return;
      }
      default:
        return;
    }
  }
}

void RheoSystem::printApi() {
  const char* api[] = {
    "/pump/blow [<0-100>]",
    "/pump/suck [<0-100>]",
    "/pump/off",
    "/valve/on",
    "/valve/off",
    "/system/off",
    "/rheo/sense/rate [<ms>]",
    "/rheo/rep",
    "/rheo/rep/triad",
    "/rheo/bubble",
    "/rheo/bubble/triad",
    "/rheo/stop",
    "/rheo/purge",
    "/rheo/program/clear",
    "/rheo/program/step <type:0-4> <direction:-1|0|1> <start:0-100> <end:0-100> <duration_ms> [pulse_count pulse_ms gap_ms]",
    "/rheo/program/run",
    "/rheo/program/stop",
    "/rheo/rep/pull/power [<0-100>]",
    "/rheo/rep/pull/time [<ms>]",
    "/rheo/rep/push/delay [<ms>]",
    "/rheo/rep/push/power [<0-100>]",
    "/rheo/rep/push/time [<ms>]",
    "/rheo/rep/push/ramp/start [<0-100>]",
    "/rheo/rep/push/ramp/time [<ms>]",
    "/rheo/rep/push/fall/time [<ms>]",
    "/rheo/rep/baseline/time [<ms>]",
    "/rheo/rep/interval [<ms>]",
    "/rheo/api",
  };
  for (const char* line : api) { Serial.println(line); sendApiEntry(line); }
}

bool RheoSystem::routeOSC(OSCMessage& msg, char* buffer) {
  if (program.isRunning()) {
    if (msg.match("/rheo/program/stop") || msg.match("/rheo/stop")) {
      program.stop("cancelled");
      return true;
    }
    if (msg.match("/system/off")) {
      program.stop("aborted");
      systemOff();
      return true;
    }
    if (!msg.match("/rheo/api")) {
      return true;
    }
  }
  if (PneumaticSystem::routeOSC(msg, buffer)) return true;

  if (!strcmp(buffer, "/slip/pa/rep")) {
    if (!recording && !manualBlowing && !triadActive && !bubbleTriadActive) startRep();
    return true;
  }
  if (msg.match("/slip/pa/stop")) {
    if (bubbleActive || bubbleTriadActive) stopBubble(true);
    else stopRep(true);
    return true;
  }
  if (msg.match("/slip/pa/purge")) {
    if (!actuationBusy()) purge();
    return true;
  }

  if (!strcmp(buffer, "/rheo/rep")) {
    if (!recording && !manualBlowing && !program.isRunning() && !bubbleTriadActive) startRep();
    return true;
  }
  if (msg.match("/rheo/rep/triad")) {
    if (!recording && !manualBlowing && !triadActive && !bubbleTriadActive) startRepTriad();
    return true;
  }
  if (!strcmp(buffer, "/rheo/bubble")) {
    if (!recording && !manualBlowing && !program.isRunning() && !triadActive && !bubbleTriadActive)
      startBubble();
    return true;
  }
  if (msg.match("/rheo/bubble/triad")) {
    if (!recording && !manualBlowing && !program.isRunning() && !triadActive && !bubbleTriadActive)
      startBubbleTriad();
    return true;
  }
  if (msg.match("/rheo/stop")) {
    if (bubbleActive || bubbleTriadActive) stopBubble(true);
    else stopRep(true);
    return true;
  }
  if (msg.match("/rheo/purge")) {
    if (!actuationBusy()) purge();
    return true;
  }
  if (msg.match("/rheo/api"))        { printApi(); return true; }
  if (msg.match("/rheo/program/clear")) {
    if (!recording && !triadActive && !manualBlowing && !latchedSuck && !testStreaming
        && !bubbleActive && !bubbleTriadActive && !purgeActive && !timedAirActive) {
      program.clear();
    }
    return true;
  }
  if (msg.match("/rheo/program/step*")) {
    if (recording || triadActive || manualBlowing || latchedSuck || testStreaming
        || bubbleActive || bubbleTriadActive || purgeActive || timedAirActive) return true;
    int primitive = getIntArg(msg, 1);
    if (!program.append(
          primitive, getIntArg(msg, 2), getIntArg(msg, 3),
          getIntArg(msg, 4), getIntArg(msg, 5),
          primitive == RoutinePlayer::PULSE ? getIntArg(msg, 6) : 0,
          primitive == RoutinePlayer::PULSE ? getIntArg(msg, 7) : 0,
          primitive == RoutinePlayer::PULSE ? getIntArg(msg, 8) : 0)) {
      program.clear();
      systemOff();
    }
    return true;
  }
  if (msg.match("/rheo/program/run")) {
    if (recording || triadActive || manualBlowing || latchedSuck || testStreaming
        || bubbleActive || bubbleTriadActive || purgeActive || timedAirActive) return true;
    if (!program.run(rate)) {
      systemOff();
    }
    return true;
  }
  if (msg.match("/rheo/program/stop")) { program.stop("cancelled"); return true; }

#define PARAM(addr, var, lo, hi) \
  if (msg.match(addr "*")) { \
    if (strchr(buffer,' ')) var = constrain(getIntArg(msg,1), lo, hi); \
    sendSampleOSC(addr, (float)(var)); \
    Serial.print(addr); Serial.print("="); Serial.println(var); \
    return true; \
  }
  PARAM("/rheo/rep/pull/power",       retractPct,          0,        100)
  PARAM("/rheo/rep/pull/time",        retractMs,           1,        5000)
  PARAM("/rheo/rep/push/delay",       pushDelayMs,         0,        5000)
  PARAM("/rheo/rep/push/power",       extrudePctMax,       0,        100)
  PARAM("/rheo/rep/push/time",        extrudeMs,           1,        5000)
  PARAM("/rheo/rep/push/ramp/start",  extrudeRampStartPct, 0,        100)
  PARAM("/rheo/rep/push/ramp/time",   extrudeRampMs,       0,        5000)
  PARAM("/rheo/rep/push/fall/time",   pushFallMs,          0,        500)
  PARAM("/rheo/rep/baseline/time",    baselineMs,          0,        5000)
  PARAM("/rheo/rep/interval",         repIntervalMs,       0,        5000)
#undef PARAM

  return false;
}
