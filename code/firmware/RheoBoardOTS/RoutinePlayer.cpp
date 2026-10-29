// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include "RoutinePlayer.h"

#include "BLEHandler.h"
#include "MemoryStream.h"
#include "OSCHandler.h"
#include "PneumaticSystem.h"

namespace {
void notifyMessage(OSCMessage& message) {
  uint8_t buffer[256];
  MemoryStream stream(buffer, sizeof(buffer));
  message.send(stream);
  pTxCharacteristic->setValue(buffer, stream.length());
  pTxCharacteristic->notify();
}

bool elapsedAtLeast(uint32_t now, uint32_t then, uint32_t duration) {
  return static_cast<uint32_t>(now - then) >= duration;
}
}  // namespace

bool RoutinePlayer::clear() {
  if (running_) return false;
  stepCount_ = 0;
  maximumDurationMs_ = 0;
  return true;
}

bool RoutinePlayer::append(
    int primitive, int direction, int startLevel, int endLevel, long durationMs,
    int pulseCount, int pulseMs, int gapMs) {
  if (running_ || stepCount_ >= RHEO_PROGRAM_MAX_STEPS) return false;
  if (primitive < REST || primitive > PULSE) return false;
  if (durationMs < 1 || static_cast<uint32_t>(durationMs) > RHEO_PROGRAM_MAX_STEP_MS) return false;
  if (startLevel < 0 || startLevel > 100 || endLevel < 0 || endLevel > 100) return false;

  Primitive p = static_cast<Primitive>(primitive);
  Direction d = static_cast<Direction>(direction);
  if (p == REST) {
    if (d != NONE || startLevel != 0 || endLevel != 0) return false;
  } else if (p == RETRACT) {
    if (d != RETRACT_DIRECTION || startLevel != endLevel) return false;
  } else if (p == PUSH) {
    if (d != PUSH_DIRECTION || startLevel != endLevel) return false;
  } else if (p == PULSE) {
    if (d != PUSH_DIRECTION || startLevel != endLevel) return false;
    if (pulseCount < 1 || pulseCount > 8 || pulseMs < 10 || pulseMs > 1000 ||
        gapMs < 0 || gapMs > 5000) return false;
    uint32_t requestedDuration =
        static_cast<uint32_t>(pulseCount) * static_cast<uint32_t>(pulseMs) +
        static_cast<uint32_t>(pulseCount - 1) * static_cast<uint32_t>(gapMs);
    if (requestedDuration != static_cast<uint32_t>(durationMs)) return false;
  } else if (d != RETRACT_DIRECTION && d != PUSH_DIRECTION) {
    return false;
  }

  uint32_t added = static_cast<uint32_t>(durationMs) + (p == REST ? 0 : VALVE_SETTLE_MS);
  if (maximumDurationMs_ > RHEO_PROGRAM_MAX_TOTAL_MS - added) return false;
  steps_[stepCount_++] = {
      p, d, static_cast<uint8_t>(startLevel), static_cast<uint8_t>(endLevel),
      static_cast<uint32_t>(durationMs), static_cast<uint8_t>(pulseCount),
      static_cast<uint16_t>(pulseMs), static_cast<uint16_t>(gapMs)};
  maximumDurationMs_ += added;
  return true;
}

bool RoutinePlayer::run(int sampleRateMs) {
  if (running_ || stepCount_ == 0) return false;
  sampleRateMs_ = constrain(sampleRateMs, MIN_RATE, MAX_RATE);
  running_ = true;
  stepIndex_ = 0;
  programStartedAt_ = millis();
  lastSampleAt_ = programStartedAt_ - sampleRateMs_;
  pumpsOff();
  valveSuck();
  sendSimpleOSC("/db/start");
  sendEvent("program-start");
  beginStep(programStartedAt_);
  return true;
}

void RoutinePlayer::stop(const char* reason) {
  if (running_) finish(reason);
  else {
    pumpsOff();
    valveSuck();
  }
}

void RoutinePlayer::beginStep(uint32_t now) {
  const Step& step = steps_[stepIndex_];
  pumpsOff();
  stepStartedAt_ = now;
  valveStartedAt_ = now;
  valveSettling_ = step.primitive != REST;
  pulseValveOpen_ = false;
  if (step.primitive == PULSE) {
    valveSuck();
    pumpSetPct(PUMP_2, step.endLevel);
  } else if (step.direction == PUSH_DIRECTION) {
    valveBlow();
  } else {
    valveSuck();
  }
  sendEvent("step-start");
}

void RoutinePlayer::applyLevel(const Step& step, uint32_t elapsed) {
  uint8_t level = step.endLevel;
  if (step.primitive == RAMP && elapsed < step.durationMs) {
    long span = static_cast<long>(step.endLevel) - step.startLevel;
    level = static_cast<uint8_t>(
        constrain(static_cast<long>(step.startLevel) +
                      span * static_cast<long>(elapsed) / static_cast<long>(step.durationMs),
                  0L, 100L));
  }
  pumpSetPct(step.direction == RETRACT_DIRECTION ? PUMP_1 : PUMP_2, level);
}

void RoutinePlayer::update() {
  if (!running_) return;
  uint32_t now = millis();

  // Sampling is cooperative (no delay(rate)); one sensor read/notification may still
  // consume device/I2C time. Watchdog allowance includes the configured rate.
  if (elapsedAtLeast(now, lastSampleAt_, sampleRateMs_)) {
    lastSampleAt_ = now;
    sendTrace(static_cast<uint32_t>(now - programStartedAt_), readPressure());
    now = millis();
  }

  if (elapsedAtLeast(
          now, programStartedAt_, maximumDurationMs_ + static_cast<uint32_t>(sampleRateMs_) + 1000UL)) {
    finish("watchdog");
    return;
  }

  const Step& step = steps_[stepIndex_];
  if (valveSettling_) {
    if (!elapsedAtLeast(now, valveStartedAt_, VALVE_SETTLE_MS)) return;
    valveSettling_ = false;
    stepStartedAt_ = now;
    if (step.primitive == PULSE) {
      valveBlow();
      pulseValveOpen_ = true;
    } else {
      applyLevel(step, 0);
    }
    sendEvent("actuation-start");
    return;
  }

  uint32_t elapsed = static_cast<uint32_t>(now - stepStartedAt_);
  if (step.primitive == PULSE) {
    uint32_t cycleMs = static_cast<uint32_t>(step.pulseMs) + step.gapMs;
    uint32_t completedCycles = cycleMs ? elapsed / cycleMs : 0;
    bool shouldOpen =
        completedCycles < step.pulseCount &&
        (elapsed % cycleMs) < static_cast<uint32_t>(step.pulseMs);
    if (shouldOpen != pulseValveOpen_) {
      if (shouldOpen) valveBlow();
      else valveSuck();
      pulseValveOpen_ = shouldOpen;
    }
  } else if (step.primitive != REST) {
    applyLevel(step, elapsed);
  }
  if (!elapsedAtLeast(now, stepStartedAt_, step.durationMs)) return;

  pumpsOff();
  sendEvent("step-complete");
  if (++stepIndex_ >= stepCount_) {
    finish("completed");
  } else {
    beginStep(now);
  }
}

void RoutinePlayer::finish(const char* reason) {
  pumpsOff();
  valveSuck();
  sendEvent(reason);
  const char* status = !strcmp(reason, "completed")
      ? "complete"
      : (!strcmp(reason, "cancelled") || !strcmp(reason, "button-abort") ||
         !strcmp(reason, "disconnect"))
          ? "cancelled"
          : "failed";
  sendCompletion(reason, status);
  running_ = false;
  sendSimpleOSC("/db/save");
}

void RoutinePlayer::sendEvent(const char* name) {
  OSCMessage message("/rheo/program/event");
  message.add(name);
  message.add(static_cast<int32_t>(millis() - programStartedAt_));
  message.add(static_cast<int32_t>(stepIndex_));
  notifyMessage(message);
}

void RoutinePlayer::sendCompletion(const char* reason, const char* status) {
  OSCMessage message("/rheo/program/complete");
  message.add(reason);
  message.add(status);
  message.add(static_cast<int32_t>(millis() - programStartedAt_));
  notifyMessage(message);
}

void RoutinePlayer::sendTrace(uint32_t elapsedMs, float pressurePa) {
  OSCMessage message("/rheo/program/trace");
  message.add(static_cast<int32_t>(elapsedMs));
  message.add(pressurePa);
  notifyMessage(message);
}
