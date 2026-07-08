// ============================================================================
// 2P1VX -- 2 pumps, 1 valve bench firmware (SparkFun ESP32 Thing Plus + L298N).
// Extends 2P1V with a runtime-tunable rheo/* OSC API: all REP shape parameters
// (pull power/time, push power/time, ramp, baseline) are settable over BLE without
// reflashing. Command namespace is rheo/* (replaces /slip/*).
// One build: BLE pipeline ("2P1VX") + USB serial bench stream (SERIAL_STREAM).
// Detailed docs: README.md
//
// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE-SOFTWARE.txt
// ============================================================================

// ---- BLE identity (consumed by BLEHandler.h #ifndef guards) -----------------
#define DEVICE_NAME            "2P1VX"
#define SERVICE_UUID           "71c978dc-2d05-4a35-b56f-12f4fee4ee31"
#define CHARACTERISTIC_UUID_RX "43d4940a-3037-4040-9c3d-6ecd138325ca"
#define CHARACTERISTIC_UUID_TX "78d88040-6306-40bd-b47d-e1feb75d6482"

// ---- Board ------------------------------------------------------------------
#define BAUDRATE    115200
#define LED_PIN     13
#define BUTTON_PIN  0
#define BTN_DEBOUNCE_MS     25
#define BTN_DOUBLE_CLICK_MS 400
#define BTN_LONG_PRESS_MS   600

#include <Arduino.h>
#include "MemoryStream.h"      // ThingPlusBLEOSC library
#include "BLEHandler.h"        //   -> BLE globals + callbacks; calls routeOSC()
#include "OSCHandler.h"        //   -> OSCMessage, sendSimpleOSC(), sendSampleOSC()
#include <SparkFun_Qwiic_Button.h>
#include "PneumaticSystem.h"   // HAL + PneumaticSystem (pump/valve/sensing)
#include "RheoSystem.h"        // RheoSystem rheo (extends PneumaticSystem)

// ============================================================================
// Globals
// ============================================================================
QwiicButton   qwiicButton;
bool          qwiicButtonReady = false;
char          buffer[255];

// ============================================================================
// OSC router -- incoming server commands over BLE RX
// ============================================================================
void routeOSC(OSCMessage &msg) {
  msg.getAddress(buffer, 0, 255);
  Serial.println(buffer);

  if (rheo.routeOSC(msg, buffer)) return;

  else { Serial.println("Unrecognized OSC address."); }
}

// ============================================================================
// Qwiic button: non-blocking click-pattern state machine
// 1-click = REP, 2-click = toggle latched SUCK, hold = momentary BLOW
// ============================================================================
void handleQwiicButton() {
  if (!qwiicButtonReady) return;
  static bool          pressed       = false;
  static unsigned long pressStartMs  = 0;
  static bool          holdConsumed  = false;
  static int           clickCount    = 0;
  static unsigned long lastReleaseMs = 0;
  static unsigned long lastEdgeMs    = 0;

  unsigned long now     = millis();
  bool          rawDown = qwiicButton.isPressed();

  if (rawDown != pressed && (now - lastEdgeMs) >= BTN_DEBOUNCE_MS) {
    lastEdgeMs = now;
    if (rawDown) {
      pressed = true; pressStartMs = now; holdConsumed = false;
    } else {
      pressed = false;
      if (holdConsumed) { if (rheo.manualBlowing) rheo.stopBlow(); }
      else              { clickCount++; lastReleaseMs = now; }
    }
  }

  if (pressed && !holdConsumed &&
      (now - pressStartMs) >= BTN_LONG_PRESS_MS && !rheo.manualBlowing && !rheo.recording) {
    rheo.blow();
    holdConsumed = true;
  }

  if (clickCount > 0 && !pressed && (now - lastReleaseMs) > BTN_DOUBLE_CLICK_MS) {
    if (!rheo.recording) {
      if (clickCount == 1) rheo.startRep();
      else                 rheo.toggleSuck();
    }
    clickCount = 0;
  }
}

// ============================================================================
// Serial commands (bench-only): REP / STOP / PUMP1 <pct> / PUMP2 <pct>
// ============================================================================
#if SERIAL_STREAM
void handleSerialCommand() {
  static char    cmd[16];
  static uint8_t n = 0;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      cmd[n] = '\0'; n = 0;
      if      (!strcmp(cmd, "REP"))        { if (!rheo.recording && !rheo.manualBlowing) rheo.startRep(); }
      else if (!strcmp(cmd, "STOP"))       { if (rheo.recording) rheo.stopRep(); if (rheo.testStreaming) rheo.stopPumpTest(); }
      else if (!strncmp(cmd, "PUMP1 ", 6)) rheo.startPumpTest(PUMP_1, atoi(cmd + 6));
      else if (!strncmp(cmd, "PUMP2 ", 6)) rheo.startPumpTest(PUMP_2, atoi(cmd + 6));
    } else if (n < sizeof(cmd) - 1) {
      cmd[n++] = c;
    }
  }
}
#else
inline void handleSerialCommand() {}
#endif

// ============================================================================
void setup() {
  Serial.begin(BAUDRATE);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  rheo.setup();
  qwiicButtonReady = qwiicButton.begin();
  Serial.println(qwiicButtonReady ? "Qwiic button ready (1-click=REP, 2-click=suck, hold=blow)."
                                  : "Qwiic button not found (optional, skipping).");
  // Pass all four explicitly -- setupBLE()'s defaults only resolve per-translation-unit,
  // so an unparameterized call would silently build against BLEHandler.h's placeholder
  // fallbacks (see BLEHandler.h) instead of this sketch's real name/UUIDs.
  setupBLE(DEVICE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID_RX, CHARACTERISTIC_UUID_TX);
  Serial.println("2P1VX initialized. Send rheo/api for supported commands.");
}

void loop() {
  static bool buttonPressed = false;

  bool sw1 = sw1Pressed();
  if ((digitalRead(BUTTON_PIN) == LOW || sw1) && !buttonPressed) {
    buttonPressed = true;
    if (rheo.recording) rheo.stopRep(); else rheo.startRep();
    return;
  }
  if (digitalRead(BUTTON_PIN) == HIGH && !sw1) buttonPressed = false;

  handleSerialCommand();
  handleQwiicButton();
  rheo.loop();

  bool active = rheo.recording || rheo.manualBlowing || rheo.latchedSuck || rheo.testStreaming;
  digitalWrite(LED_PIN, active ? HIGH : LOW);
  ledExt(active);

  if (!deviceConnected && oldDeviceConnected) {
    delay(500); pServer->startAdvertising();
    Serial.println("Started advertising again");
    oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected) oldDeviceConnected = deviceConnected;
}
