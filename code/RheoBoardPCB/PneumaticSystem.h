// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE
#ifndef PNEUMATIC_SYSTEM_H
#define PNEUMATIC_SYSTEM_H

#include "HardwareProfile.h"
#include "V1Hardware.h"
#include <Arduino.h>
#include <Wire.h>
#include "OSCHandler.h"

// ---- Build flags ------------------------------------------------------------
#define SERIAL_STREAM 1        // USB serial bench stream; set 0 to disable

// Original v1: PCA9685 0x40 pumps, TCA9534A 0x3F valves/buttons,
// MPRLS0025PA00001A 0x18 absolute pressure. SDA23 / SCL22.
#define PWM_CH_PUMP1 RHEO_V1_VACUUM_CHANNEL
#define PWM_CH_PUMP2 (1 - RHEO_V1_VACUUM_CHANNEL)
// Both valve states are configured independently for pressure/vacuum/rest.
// Verify tubing against these logical states before actuation.

// ---- Optional bench extras (native ESP32 pins) ------------------------------
#define PIN_SW1   -1     // optional external REP button to GND; -1 = none
#define PIN_LED1  -1     // optional external status LED; -1 = none

#define PWM_FREQ_HZ     1000   // setPWMFreq(1000); every PCA9685 channel shares it
#define VALVE_SETTLE_MS 0      // match SlipThing / PA `'d'` command clocks
// Purge is a 1 s push at full pump drive, same as a 100% extrude.
#define PURGE_PCT          100

// ---- Pressure sampling ------------------------------------------------------
#define DEFAULT_RATE 10        // ms between samples
#define MIN_RATE     10
#define MAX_RATE     1000

enum PumpChannel { PUMP_1 = 1, PUMP_2 = 2 };

// ---- Pump / valve / sensor primitives ---------------------------------------
// Pump duty is 0..100%; valves use steady DC. STOP releases every output.
// Normal REP/program completion uses the configured rest state.
void pumpSetPct(PumpChannel c, int pct);
void pumpSet(PumpChannel c, bool on);
void pumpsOff();
void pumpsOffNow();
void valveSuck();
void valveBlow();
void valveRest();
void valvesOff();
bool pwmReady();
inline void ledExt(bool on)    { if (PIN_LED1 >= 0) digitalWrite(PIN_LED1, on ? HIGH : LOW); }
inline bool sw1Pressed()       { return PIN_SW1 >= 0 && digitalRead(PIN_SW1) == LOW; }
inline float readPressure() {
  return v1Hardware::pressurePa();
}

bool halBegin();
void sendApiEntry(const char* entry);  // notifies one /rheo/api reply line over BLE TX

// ============================================================================
// PneumaticSystem — pump/valve/sensor control + /pump/* /valve/* /system/* OSC
// ============================================================================
class PneumaticSystem {
public:
    bool          manualBlowing = false;
    bool          latchedSuck   = false;
    bool          testStreaming = false;
    unsigned long testStart     = 0;
    bool          purgeActive   = false;
    unsigned long purgeEndsAt   = 0;
    bool          timedAirActive = false;
    bool          timedAirIsBlow = false;
    unsigned long timedAirStopsAt = 0;
    float         pressure      = 0.0f;
    int           rate          = DEFAULT_RATE;

    // Explicit power (0-100); subclasses may override to supply a default.
    virtual void blow(int power);
    virtual void suck(int power);      // latches until stopSuck
    void         stopBlow();
    void         stopSuck();
    void         cancelSuck();
    void         toggleSuck();         // calls suck(-1) so subclass default applies

    void off();        // /pump/off    — cancel work and release all outputs
    void purge();      // /pump/purge  — nonblocking 1 s pressure clear
    void valveOn();    // /valve/on    — select pressure state, idle only
    bool valvePhysical(uint8_t port, bool on);
    void valvesRest();
    void valveOff();   // /valve/off   — legacy vacuum-state alias, idle only
    virtual void systemOff();  // pumps off, valve park; subclass cancels its routines
    void startTimedAir(bool blowDir, int power, int duration);
    void stopTimedAir();
    void updateAuxiliary();
    virtual bool actuationBusy() const;

    // Pressure sensing
    virtual unsigned long sessionStart() const { return 0; }
    void sampleAir();     // read pressure, send /rheo/sense/air, then wait rate ms
    void sampleTest();    // bench stream: #S,<ms>,<Pa> then delay(rate)

    // Bench pump-test
    virtual bool isActive() const;
    void startPumpTest(PumpChannel ch, int pct);
    void stopPumpTest();

    virtual void setup();
    virtual void loop();

    virtual void printApi();
    virtual bool routeOSC(OSCMessage& msg, char* buffer);

protected:
    // Runs about once per millisecond while sampleAir() waits.
    virtual void onSampleWait() {}
};

#endif // PNEUMATIC_SYSTEM_H
