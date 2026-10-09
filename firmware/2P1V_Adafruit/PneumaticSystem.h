// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#ifndef PNEUMATIC_SYSTEM_H
#define PNEUMATIC_SYSTEM_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MicroPressure.h>
#include <Adafruit_seesaw.h>   // Adafruit ATtiny1616 Breakout with seesaw -- Library Manager: "Adafruit seesaw Library"
#include "OSCHandler.h"

// ---- Build flags ------------------------------------------------------------
#define SERIAL_STREAM 1        // USB serial bench stream; set 0 to disable

// ---- Qwiic bus (I2C, shared/daisy-chained): Button + MicroPressure + this
// ATtiny1616 seesaw board all hang off the ESP32's single Qwiic connector. -----
#define MPRLS_I2C_ADDR   0x18
#define SEESAW_I2C_ADDR  0x49   // Adafruit seesaw factory default (no address jumpers needed --
                                 // doesn't collide with MPRLS 0x18 or Qwiic Button 0x6F)

// ---- Pump/valve control -- routed through the ATtiny1616 seesaw board ------
// Earlier builds sourced PUMP1_EN/PUMP2_EN from ESP32 pins 32/33 (LEDC PWM)
// and VALVE2_EN from pin 14. Here the Adafruit ATtiny1616 breakout (seesaw
// firmware) sits between the ESP32 and the two L298N drivers, carrying those
// three connected signals -- and unlike a plain digital
// I2C GPIO expander (no PWM), seesaw exposes REAL 8-bit PWM over I2C
// (Adafruit_seesaw::analogWrite), so pump drive stays fully proportional.
// RheoSystem's REP phase machine, config percentages, and ramp logic are
// therefore UNCHANGED; only the low-level primitives below differ.
#define SS_PUMP1_EN   0   // was ESP32 pin 32 (vacuum pump, L298N #1 ENA) -- seesaw PWM pin
#define SS_PUMP2_EN   1   // was ESP32 pin 33 (pressure pump, L298N #1 ENB) -- seesaw PWM pin
#define SS_VALVE1_EN  4   // reserved for future VALVE1; physically NC in this single-valve build
#define SS_VALVE2_EN  5   // was ESP32 pin 14 (the only valve driven -- flip selector) -- plain GPIO

// ---- Optional bench extras (native ESP32 pins; unaffected by the seesaw move)
#define PIN_SW1   -1     // optional external REP button to GND; -1 = none
#define PIN_LED1  -1     // optional external status LED; -1 = none

#define PWM_FREQ_HZ     1000   // set via Adafruit_seesaw::setPWMFreq
#define VALVE_SETTLE_MS 15     // valve-flip guard before pump drive

// ---- Pressure sampling ------------------------------------------------------
#define DEFAULT_RATE 10        // ms between samples
#define MIN_RATE     10
#define MAX_RATE     1000

extern SparkFun_MicroPressure mpr;
extern Adafruit_seesaw ss;   // ATtiny1616 seesaw driving PUMP1/PUMP2/VALVE2; VALVE1 pin is reserved

enum PumpChannel { PUMP_1 = 1, PUMP_2 = 2 };

// ---- Pump / valve / sensor primitives ---------------------------------------
inline void pumpSetPct(PumpChannel c, int pct) {
  pct = constrain(pct, 0, 100);
  uint8_t duty = (uint8_t)((pct * 255) / 100);
  ss.analogWrite(c == PUMP_1 ? SS_PUMP1_EN : SS_PUMP2_EN, duty);
}
inline void pumpSet(PumpChannel c, bool on) { pumpSetPct(c, on ? 100 : 0); }
inline void pumpsOff()         { pumpSet(PUMP_1, false); pumpSet(PUMP_2, false); }
inline void ledExt(bool on)    { if (PIN_LED1 >= 0) digitalWrite(PIN_LED1, on ? HIGH : LOW); }
inline bool sw1Pressed()       { return PIN_SW1 >= 0 && digitalRead(PIN_SW1) == LOW; }
inline float readPressure()    { return mpr.readPressure(PA); }
inline void valveSuck() { ss.digitalWrite(SS_VALVE2_EN, LOW);  }
inline void valveBlow() { ss.digitalWrite(SS_VALVE2_EN, HIGH); }

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
    float         pressure      = 0.0f;
    int           rate          = DEFAULT_RATE;

    // Explicit power (0-100); subclasses may override to supply a default.
    virtual void blow(int power);
    virtual void suck(int power);      // latches until stopSuck
    void         stopBlow();
    void         stopSuck();
    void         cancelSuck();
    void         toggleSuck();         // calls suck(-1) so subclass default applies

    void off();        // /pump/off    — pumps off, valve unchanged
    void purge();      // /pump/purge  — clear line with pressure, then rest
    void valveOn();    // /valve/on    — energize valve (pressure path, no pump)
    void valveOff();   // /valve/off   — de-energize valve (vacuum path, no pump)
    void systemOff();  // /system/off  — emergency stop: pumps off, valve park, reset state

    // Pressure sensing
    virtual unsigned long sessionStart() const { return 0; }
    void sampleAir();     // read pressure, send /rheo/sense/air, delay(rate)
    void sampleTest();    // bench stream: #S,<ms>,<Pa> then delay(rate)

    // Bench pump-test
    virtual bool isActive() const;
    void startPumpTest(PumpChannel ch, int pct);
    void stopPumpTest();

    virtual void setup();
    virtual void loop();

    virtual void printApi();
    virtual bool routeOSC(OSCMessage& msg, char* buffer);
};

#endif // PNEUMATIC_SYSTEM_H
