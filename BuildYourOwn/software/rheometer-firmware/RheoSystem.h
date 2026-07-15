// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see LICENSE
#ifndef RHEO_SYSTEM_H
#define RHEO_SYSTEM_H

#include "PneumaticSystem.h"

// ---- REP timing + actuation defaults ----------------------------------------
#define REP_TIME                   1500  // ms; full window (baseline→retract→extrude→relax)
#define REP_BASELINE_MS             420
#define REP_RETRACT_MS              315
#define REP_EXTRUDE_MS              345
#define REP_RETRACT_PCT              54
#define REP_EXTRUDE_PCT             100
#define REP_EXTRUDE_RAMP_MS         300
#define REP_EXTRUDE_RAMP_START_PCT   40
#define REP_INTERVAL_MS             500  // ms; gap between REPs when running /rheo/rep/triad

// ============================================================================
// RheoSystem — extends PneumaticSystem with a REP sensing routine
// ============================================================================
class RheoSystem : public PneumaticSystem {
public:
    // ---- Runtime-tunable REP params (compile-time defaults above) ----------
    int retractPct          = REP_RETRACT_PCT;
    int retractMs           = REP_RETRACT_MS;
    int extrudePctMax       = REP_EXTRUDE_PCT;
    int extrudeMs           = REP_EXTRUDE_MS;
    int extrudeRampMs       = REP_EXTRUDE_RAMP_MS;
    int extrudeRampStartPct = REP_EXTRUDE_RAMP_START_PCT;
    int baselineMs          = REP_BASELINE_MS;
    int repIntervalMs       = REP_INTERVAL_MS;

    // ---- REP state (read by .ino for LED) -----------------------------------
    bool          recording = false;
    unsigned long startedAt = 0;

    // ---- REP triad state (three REPs back-to-back via /rheo/rep/triad) ------
    bool          triadActive    = false;
    int           triadCount     = 0;   // reps remaining to start
    unsigned long triadWaitUntil = 0;   // 0 = not waiting

    bool isActive()     const override { return recording; }
    unsigned long sessionStart() const override { return startedAt; }

    // ---- Overrides: use REP params as default power for manual gestures -----
    void blow(int power = -1) override;   // -1 → extrudePctMax
    void suck(int power = -1) override;   // -1 → retractPct

    // ---- REP routine --------------------------------------------------------
    void startRep();
    void startRepTriad();               // runs startRep() three times, repIntervalMs apart
    void stopRep(bool cancelTriad = true);  // cancelTriad=false lets an in-progress triad continue
    void update();    // call every loop(): advance phase machine + auto-stop + triad sequencing

    // ---- Lifecycle ----------------------------------------------------------
    void setup() override;
    void loop() override;

    // ---- OSC ----------------------------------------------------------------
    void printApi() override;
    bool routeOSC(OSCMessage& msg, char* buffer) override;

private:
    enum Phase { IDLE, BASELINE, RETRACT, EXTRUDE, RELAX };
    Phase         phase      = IDLE;
    unsigned long phaseStart = 0;
    bool          pumpOn     = false;
    unsigned long pumpStart  = 0;

    int  calcExtrudePct(unsigned long pe);
    void repEvent(const char* label);
    void phaseUpdate();
};

extern RheoSystem rheo;

#endif // RHEO_SYSTEM_H
