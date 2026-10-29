// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#ifndef PNEUMATIC_SYSTEM_H
#define PNEUMATIC_SYSTEM_H

#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_MicroPressure.h>
#include <Adafruit_PWMServoDriver.h>  // Adafruit product 815 — Library Manager: "Adafruit PWM Servo Driver"
#include "OSCHandler.h"

// ---- Build flags ------------------------------------------------------------
#define SERIAL_STREAM 1        // USB serial bench stream; set 0 to disable

// ---- I2C: one peripheral, two physical connections --------------------------
// Wire.begin() uses the ESP32 Thing Plus defaults (variant esp32thing_plus):
// SDA = GPIO 23, SCL = GPIO 22. The Qwiic connector and the header SDA/SCL
// pins are that same bus.
// Qwiic cable, Qwiic connector only: SparkFun button (0x6F) and MPRLS (0x18).
// PCA9685 product 815 has no Qwiic socket. Four header jumper wires:
//   ESP32 GND → PCA9685 GND
//   ESP32 3V3 → PCA9685 VCC   (logic only; never 12 V)
//   ESP32 SDA (GPIO 23) → PCA9685 SDA
//   ESP32 SCL (GPIO 22) → PCA9685 SCL
// Address stays 0x40 (no address jumpers). Leave V+ and OE unconnected.
// Do not put 12 V on the PCA9685. Logic 3.3 V does not power the motors.
#define MPRLS_I2C_ADDR    0x18  // SparkFun MicroPressure, on the Qwiic cable
#define QWIIC_BUTTON_ADDR 0x6F  // SparkFun Qwiic Button (library default)
#define PWM_DRIVER_ADDR   0x40  // Adafruit PCA9685, product 815, header jumpers

// ---- Pump/valve enables are PCA9685 outputs, not ESP32 GPIOs ----------------
// Two L298N modules. Direction pins IN1–IN4 are hardwired; these channels
// drive the enable pins only. On this module INA is ENA (enables OUT1/OUT2)
// and INB is ENB (enables OUT3/OUT4). Remove the ENA and ENB jumpers.
// LEFT ENB (INB) is wired to GND, not to the PCA9685. Do not drive it from
// channel 11. Channel 11 is unused and not connected; firmware parks it
// full-off so the pin stays low.
// One 12 V brick feeds BOTH L298N VS terminals. Common ground: ESP32, PCA9685,
// both L298N GND terminals, and the 12 V supply negative.
// LEFT L298N: one pump. OUT1/OUT2 = left pump (ENA / INA, channel 10).
//   OUT3/OUT4 unused. IN3 and IN4 tied to GND. ENB (INB) tied to GND.
// RIGHT L298N: valve and the other pump. OUT1/OUT2 = FA0520E (ENA / INA, channel 8).
//   OUT3/OUT4 = right pump (ENB / INB, channel 9).
#define PWM_CH_VALVE   8   // RIGHT L298N ENA (INA) — FA0520E, full-on / full-off, OUT1/OUT2
#define PWM_CH_PUMP2   9   // RIGHT L298N ENB (INB) — PUMP_2 extrude / pressure, OUT3/OUT4
#define PWM_CH_PUMP1   10  // LEFT L298N ENA (INA) — PUMP_1 retract / vacuum, OUT1/OUT2
#define PWM_CH_UNUSED  11  // not connected; parked full-off so the pin stays low

// FA0520E (Adafruit 4663) is a 6 V 3-way solenoid. valveBlow is full-on and
// valveSuck is full-off. Do not PWM the valve.
// Powered (plastic port open to common) = blow / pressure path.
// Released (metal port open to common) = suck / vacuum path.

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

extern SparkFun_MicroPressure mpr;
extern Adafruit_PWMServoDriver pwm;  // PCA9685 driving the pump and valve enables

enum PumpChannel { PUMP_1 = 1, PUMP_2 = 2 };

// ---- Pump / valve / sensor primitives ---------------------------------------
// 0% and the valve-released state are PCA9685 full-off (setPin 0, always-off
// bit). 100% and the valve-powered state are full-on (setPin 4095, always-on
// bit). Intermediate pump duties are normal 12-bit PWM. Pumps park full-off
// at boot and on REP stop (setPin 0, no chip reset). The valve parks full-off
// at boot. A completed REP leaves channel 8 full-on (blow) until the next
// retract, or until system-off / disconnect releases it.
void pumpSetPct(PumpChannel c, int pct);
void pumpSet(PumpChannel c, bool on);
void pumpsOff();
void pumpsOffNow();
void valveSuck();
void valveBlow();
bool pwmReady();
inline void ledExt(bool on)    { if (PIN_LED1 >= 0) digitalWrite(PIN_LED1, on ? HIGH : LOW); }
inline bool sw1Pressed()       { return PIN_SW1 >= 0 && digitalRead(PIN_SW1) == LOW; }
inline float readPressure()    { return mpr.readPressure(PA); }

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

    void off();        // /pump/off    — pumps off, valve unchanged
    void purge();      // /pump/purge  — nonblocking 1 s pressure clear
    void valveOn();    // /valve/on    — energize valve (pressure path, no pump)
    void valveOff();   // /valve/off   — de-energize valve (vacuum path, no pump)
    void systemOff();  // /system/off  — emergency stop: pumps off, valve park, reset state
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
