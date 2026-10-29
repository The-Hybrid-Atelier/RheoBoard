// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#ifndef ROUTINE_PLAYER_H
#define ROUTINE_PLAYER_H

#include <Arduino.h>

#define RHEO_PROGRAM_MAX_STEPS 16
#define RHEO_PROGRAM_MAX_STEP_MS 30000UL
#define RHEO_PROGRAM_MAX_TOTAL_MS 120000UL

class RoutinePlayer {
public:
    enum Primitive : uint8_t { REST = 0, RETRACT = 1, PUSH = 2, RAMP = 3, PULSE = 4 };
    enum Direction : int8_t { NONE = 0, RETRACT_DIRECTION = -1, PUSH_DIRECTION = 1 };

    struct Step {
        Primitive primitive;
        Direction direction;
        uint8_t startLevel;
        uint8_t endLevel;
        uint32_t durationMs;
        uint8_t pulseCount;
        uint16_t pulseMs;
        uint16_t gapMs;
    };

    bool clear();
    bool append(
        int primitive, int direction, int startLevel, int endLevel, long durationMs,
        int pulseCount = 0, int pulseMs = 0, int gapMs = 0);
    bool run(int sampleRateMs);
    void stop(const char* reason = "cancelled");
    void update();
    bool isRunning() const { return running_; }
    uint8_t stepCount() const { return stepCount_; }
    uint32_t maximumDurationMs() const { return maximumDurationMs_; }

private:
    Step steps_[RHEO_PROGRAM_MAX_STEPS];
    uint8_t stepCount_ = 0;
    uint8_t stepIndex_ = 0;
    bool running_ = false;
    bool valveSettling_ = false;
    bool pulseValveOpen_ = false;
    uint32_t programStartedAt_ = 0;
    uint32_t stepStartedAt_ = 0;
    uint32_t valveStartedAt_ = 0;
    uint32_t lastSampleAt_ = 0;
    uint32_t maximumDurationMs_ = 0;
    uint16_t sampleRateMs_ = 10;

    void beginStep(uint32_t now);
    void finish(const char* reason);
    void applyLevel(const Step& step, uint32_t elapsed);
    void sendEvent(const char* name);
    void sendCompletion(const char* reason, const char* status);
    void sendTrace(uint32_t elapsedMs, float pressurePa);
};

#endif
