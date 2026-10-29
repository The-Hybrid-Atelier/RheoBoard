// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#ifndef RHEO_SYSTEM_H
#define RHEO_SYSTEM_H

#include "PneumaticSystem.h"
#include "RoutinePlayer.h"

// Defaults match Programmable Air `'d'` (wait 250 → retract 250 @ 100% →
// push 250 @ 100%). Device Manager calibration may overwrite the runtime
// knobs. Sample spacing stays on the 18 ms SlipThing grid below.
#define REP_TIME                   1200
// Saved REP is sample-index → Pa, spaced like SlipThing. One sample every
// 18 ms from t = 0, so a 1200 ms window is 66 samples (index 0..65).
// Index 14 / 28 / 42 is 252 / 504 / 756 ms. The grid does not follow
// /rheo/sense/rate.
#define REP_GRID_MS                  18
#define REP_GRID_COUNT               (REP_TIME / REP_GRID_MS)
#define REP_BASELINE_MS             250
#define REP_RETRACT_MS              250
#define REP_EXTRUDE_MS              250
#define REP_RETRACT_PCT             100
#define REP_EXTRUDE_PCT             100
#define REP_EXTRUDE_RAMP_MS           0
#define REP_EXTRUDE_RAMP_START_PCT    0
#define REP_PUSH_DELAY_MS             0
// SlipThing `'d'` ends with pumps off. 0 is that hard cutoff. A stored
// calibration can still set a fall time. A non-zero fall steps down to the
// floor, then pumpsOff() parks both pump channels full-off.
#define REP_PUSH_FALL_MS              0
#define REP_PUSH_FALL_FLOOR_PCT       8
#define REP_INTERVAL_MS             500
// ST framework (PA `'b'` stand-in). Compiled and routed, but RheoData does
// not offer Surface Tension on this BLE label until ST development is done.
#define BUBBLE_TIME                 200
#define BUBBLE_PULSE_MS              50
#define BUBBLE_PULSE_PCT             10
#define REP_SAVE_FLUSH_MS            80

class RheoSystem : public PneumaticSystem {
public:
    int retractPct          = REP_RETRACT_PCT;
    int retractMs           = REP_RETRACT_MS;
    int extrudePctMax       = REP_EXTRUDE_PCT;
    int extrudeMs           = REP_EXTRUDE_MS;
    int extrudeRampMs       = REP_EXTRUDE_RAMP_MS;
    int extrudeRampStartPct = REP_EXTRUDE_RAMP_START_PCT;
    int pushDelayMs         = REP_PUSH_DELAY_MS;
    int pushFallMs          = REP_PUSH_FALL_MS;
    int baselineMs          = REP_BASELINE_MS;
    int repIntervalMs       = REP_INTERVAL_MS;

    bool          recording = false;
    unsigned long startedAt = 0;
    int           repSamplesSent = 0;
    int           repSamplesFlushed = 0;
    int32_t       repGridPa[REP_GRID_COUNT] = {};
    RoutinePlayer program;

    bool          triadActive    = false;
    int           triadCount     = 0;
    unsigned long triadWaitUntil = 0;

    bool          bubbleActive      = false;
    bool          bubbleTriadActive = false;
    unsigned long bubbleStartedAt   = 0;
    unsigned long bubbleWaitUntil   = 0;
    int           bubbleTriadCount  = 0;

    bool isActive() const override { return recording || program.isRunning(); }
    bool actuationBusy() const override;
    unsigned long sessionStart() const override;

    void blow(int power = -1) override;
    void suck(int power = -1) override;

    void startRep();
    void startRepTriad();
    void stopRep(bool cancelTriad = true);
    void startBubble();
    void startBubbleTriad();
    void stopBubble(bool cancelTriad = true);
    void update();

    void setup() override;
    void loop() override;

    void printApi() override;
    bool routeOSC(OSCMessage& msg, char* buffer) override;

private:
    enum Phase { IDLE, BASELINE, RETRACT, PUSH_DELAY, EXTRUDE, PUSH_FALL, RELAX };
    Phase         phase      = IDLE;
    bool          pumpOn     = false;
    int           extrudeNowPct = 0;
    int           fallFromPct = 0;
    int           fallStep    = -1;
    bool          bubblePulseOn = false;
    unsigned long bubblePulseEndsAt = 0;

    int  calcExtrudePct(unsigned long pe);
    void repEvent(const char* label);
    void phaseUpdate();
    void captureDueRepSlots();
    void transmitRepSlack();
    void flushRepSamples();
    void onSampleWait() override { phaseUpdate(); }
};

extern RheoSystem rheo;

#endif // RHEO_SYSTEM_H
