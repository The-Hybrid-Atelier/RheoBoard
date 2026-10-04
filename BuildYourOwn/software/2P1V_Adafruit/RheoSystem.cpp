// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include "RheoSystem.h"
#include "OSCHandler.h"

RheoSystem rheo;

// ---- Overrides: default to REP params for manual gestures ------------------
void RheoSystem::blow(int power) { PneumaticSystem::blow(power < 0 ? extrudePctMax : power); }
void RheoSystem::suck(int power) { PneumaticSystem::suck(power < 0 ? retractPct    : power); }

// ---- Lifecycle -------------------------------------------------------------
void RheoSystem::setup() { PneumaticSystem::setup(); }

void RheoSystem::loop() {
  update();
  if (recording)     sampleAir();
  PneumaticSystem::loop();
}

// ---- REP routine -----------------------------------------------------------
void RheoSystem::startRep() {
  cancelSuck();
  recording = true;
  sendSimpleOSC("/db/start");
#if SERIAL_STREAM
  Serial.print("#REP_START,"); Serial.println(readPressure(), 1);
#endif
  startedAt  = millis();
  pumpsOff();
  phase = BASELINE; phaseStart = millis(); pumpOn = false;
  repEvent("BASELINE");
}

void RheoSystem::startRepTriad() {
  triadActive    = true;
  triadCount     = 3;
  triadWaitUntil = 0;
  startRep();
  triadCount--;   // one just started
}

void RheoSystem::stopRep(bool cancelTriad) {
  recording = false;
  pumpsOff(); valveSuck(); phase = IDLE;
  repEvent("STOP");
  startedAt = 0;
#if SERIAL_STREAM
  Serial.println("#REP_END");
#endif
  sendSimpleOSC("/db/save");

  if (cancelTriad) {
    triadActive    = false;
    triadCount     = 0;
    triadWaitUntil = 0;
    return;
  }
  // Triad-driven stop (auto-timeout, not a manual /rheo/stop): schedule the next REP
  // instead of cancelling, unless this was already the last one in the sequence.
  if (triadActive) {
    if (triadCount > 0) triadWaitUntil = millis() + (unsigned long)repIntervalMs;
    else                triadActive    = false;
  }
}

void RheoSystem::update() {
  phaseUpdate();
  if (startedAt > 0 && millis() - startedAt > REP_TIME) stopRep(false);

  if (triadActive && !recording && triadWaitUntil > 0 && millis() >= triadWaitUntil) {
    triadWaitUntil = 0;
    if (triadCount > 0) { startRep(); triadCount--; }
    else                triadActive = false;
  }
}

// ---- Phase machine ---------------------------------------------------------
void RheoSystem::repEvent(const char* label) {
#if SERIAL_STREAM
  Serial.print("#PH,"); Serial.print((long)(millis() - startedAt));
  Serial.print(','); Serial.println(label);
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

void RheoSystem::phaseUpdate() {
  if (phase == IDLE) return;
  unsigned long elapsed = millis() - phaseStart;
  switch (phase) {
    case BASELINE:
      if (elapsed >= (unsigned long)baselineMs) {
        pumpsOff(); valveSuck();
        phase = RETRACT; phaseStart = millis(); pumpOn = false;
        repEvent("RETRACT_VALVE");
      }
      break;
    case RETRACT:
      if (!pumpOn) {
        if (elapsed >= VALVE_SETTLE_MS) {
          pumpSetPct(PUMP_1, retractPct);
          pumpOn = true; pumpStart = millis(); repEvent("RETRACT_PUMP");
        }
      } else if (millis() - pumpStart >= (unsigned long)retractMs) {
        pumpSet(PUMP_1, false); valveBlow();
        phase = EXTRUDE; phaseStart = millis(); pumpOn = false;
        repEvent("EXTRUDE_VALVE");
      }
      break;
    case EXTRUDE:
      if (!pumpOn) {
        if (elapsed >= VALVE_SETTLE_MS) {
          pumpSetPct(PUMP_2, calcExtrudePct(0));
          pumpOn = true; pumpStart = millis(); repEvent("EXTRUDE_PUMP");
        }
      } else {
        unsigned long pe = millis() - pumpStart;
        pumpSetPct(PUMP_2, calcExtrudePct(pe));
        if (pe >= (unsigned long)extrudeMs) {
          pumpsOff();
          phase = RELAX; phaseStart = millis(); pumpOn = false;
          repEvent("RELAX");
        }
      }
      break;
    case RELAX:
      break;
    default:
      break;
  }
}

// ---- OSC -------------------------------------------------------------------
void RheoSystem::printApi() {
  const char* api[] = {
    // Pump/valve entries from PneumaticSystem — /pump/purge omitted (superseded by /rheo/purge)
    "/pump/blow [<0-100>]",
    "/pump/suck [<0-100>]",
    "/pump/off",
    "/valve/on",
    "/valve/off",
    "/system/off",
    "/rheo/sense/rate [<ms>]",
    // RheoSystem commands
    "/rheo/rep",
    "/rheo/rep/triad",
    "/rheo/stop",
    "/rheo/purge",
    // Runtime-tunable REP params
    "/rheo/rep/pull/power [<0-100>]",
    "/rheo/rep/pull/time [<ms>]",
    "/rheo/rep/push/power [<0-100>]",
    "/rheo/rep/push/time [<ms>]",
    "/rheo/rep/push/ramp/start [<0-100>]",
    "/rheo/rep/push/ramp/time [<ms>]",
    "/rheo/rep/baseline/time [<ms>]",
    "/rheo/rep/interval [<ms>]",
    "/rheo/api",
  };
  for (const char* line : api) { Serial.println(line); sendApiEntry(line); }
}

bool RheoSystem::routeOSC(OSCMessage& msg, char* buffer) {
  if (PneumaticSystem::routeOSC(msg, buffer)) return true;

  if (!strcmp(buffer, "/rheo/rep"))  { if (!recording && !manualBlowing) startRep(); return true; }
  if (msg.match("/rheo/rep/triad"))  { if (!recording && !manualBlowing && !triadActive) startRepTriad(); return true; }
  if (msg.match("/rheo/stop"))       { if (recording) stopRep(true); return true; }
  if (msg.match("/rheo/purge"))      { purge(); return true; }
  if (msg.match("/rheo/api"))        { printApi(); return true; }

#define PARAM(addr, var, lo, hi) \
  if (msg.match(addr "*")) { \
    if (strchr(buffer,' ')) var = constrain(getIntArg(msg,1), lo, hi); \
    sendSampleOSC(addr, (float)(var)); \
    Serial.print(addr); Serial.print("="); Serial.println(var); \
    return true; \
  }
  PARAM("/rheo/rep/pull/power",       retractPct,          0,        100)
  PARAM("/rheo/rep/pull/time",        retractMs,           1,        5000)
  PARAM("/rheo/rep/push/power",       extrudePctMax,       0,        100)
  PARAM("/rheo/rep/push/time",        extrudeMs,           1,        5000)
  PARAM("/rheo/rep/push/ramp/start",  extrudeRampStartPct, 0,        100)
  PARAM("/rheo/rep/push/ramp/time",   extrudeRampMs,       0,        5000)
  PARAM("/rheo/rep/baseline/time",    baselineMs,          0,        5000)
  PARAM("/rheo/rep/interval",         repIntervalMs,       0,        5000)
#undef PARAM

  return false;
}
