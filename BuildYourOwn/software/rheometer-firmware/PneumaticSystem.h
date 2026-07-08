// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE-SOFTWARE.txt
#ifndef PNEUMATIC_SYSTEM_H
#define PNEUMATIC_SYSTEM_H

#include <Arduino.h>
#include <SparkFun_MicroPressure.h>
#include "OSCHandler.h"

// ---- Build flags ------------------------------------------------------------
#define SERIAL_STREAM 1        // USB serial bench stream; set 0 to disable

// ---- Pneumatic hardware (GPIO, PWM, I2C) ------------------------------------
#define PIN_PUMP1_EN    32
#define PIN_PUMP2_EN    33
#define PIN_VALVE1_EN   15     // wired but unused in this single-valve build
#define PIN_VALVE2_EN   14
#define PIN_SW1         -1     // optional external REP button to GND; -1 = none
#define PIN_LED1        -1     // optional external status LED; -1 = none
#define MPRLS_I2C_ADDR  0x18
#define PWM_FREQ_HZ     1000
#define PWM_RES_BITS    8
#define VALVE_SETTLE_MS 15     // valve-flip guard before pump drive

// ---- Pressure sampling ------------------------------------------------------
#define DEFAULT_RATE 10        // ms between samples
#define MIN_RATE     10
#define MAX_RATE     1000

extern SparkFun_MicroPressure mpr;

enum PumpChannel { PUMP_1 = 1, PUMP_2 = 2 };

// ---- LEDC PWM helpers (ESP32 Arduino core 2.x / 3.x) -----------------------
inline void pwmAttach(uint8_t pin, uint8_t ch) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcAttach(pin, PWM_FREQ_HZ, PWM_RES_BITS); (void)ch;
#else
  ledcSetup(ch, PWM_FREQ_HZ, PWM_RES_BITS); ledcAttachPin(pin, ch);
#endif
}
inline void pwmWrite(uint8_t pin, uint8_t ch, uint8_t duty) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 3)
  ledcWrite(pin, duty); (void)ch;
#else
  ledcWrite(ch, duty); (void)pin;
#endif
}

// ---- Pump / valve / sensor primitives ---------------------------------------
inline void pumpSetPct(PumpChannel c, int pct) {
  pct = constrain(pct, 0, 100);
  uint8_t duty = (uint8_t)((pct * 255) / 100);
  if (c == PUMP_1) pwmWrite(PIN_PUMP1_EN, 0, duty);
  else             pwmWrite(PIN_PUMP2_EN, 1, duty);
}
inline void pumpSet(PumpChannel c, bool on) { pumpSetPct(c, on ? 100 : 0); }
inline void pumpsOff()         { pumpSet(PUMP_1, false); pumpSet(PUMP_2, false); }
inline void ledExt(bool on)    { if (PIN_LED1 >= 0) digitalWrite(PIN_LED1, on ? HIGH : LOW); }
inline bool sw1Pressed()       { return PIN_SW1 >= 0 && digitalRead(PIN_SW1) == LOW; }
inline float readPressure()    { return mpr.readPressure(PA); }
inline void valveSuck() { digitalWrite(PIN_VALVE2_EN, LOW);  }
inline void valveBlow() { digitalWrite(PIN_VALVE2_EN, HIGH); }

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
