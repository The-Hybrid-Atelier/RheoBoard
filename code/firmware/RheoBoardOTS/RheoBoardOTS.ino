// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
// ============================================================================
// RheoBoardOTS -- 2 pumps, 1 valve bench firmware (SparkFun ESP32 Thing Plus).
// An Adafruit PCA9685 16-channel PWM driver (product 815, I2C 0x40, no address
// jumpers) is four header jumpers from the Thing Plus, not a Qwiic cable:
// GND, 3V3→VCC, SDA GPIO 23, SCL GPIO 22. Product 815 has no Qwiic socket.
// V+ and OE stay open. The Qwiic connector carries only the button (0x6F) and
// MPRLS (0x18); those pins are the same I2C peripheral as the header.
// Channels: 8 RIGHT ENA (INA) valve, 9 RIGHT ENB (INB) right pump,
// 10 LEFT ENA (INA) left pump. Channel 11 is not wired and is held off.
// LEFT ENB (INB) is tied to GND. One 12V brick feeds both L298N motor supplies.
// Shared sensing follows SlipThing: PA `'d'`-shaped REP (250/250/250 @ 100%),
// Surface Tension (50 ms / 10% blow, 200 ms capture), /slip/* aliases,
// deferred BLE OSC, disconnect /db/save. No PA red/blue buttons and no `'z'`
// watchdog. Calibration knobs remain writable.
// One build: BLE pipeline ("RheoBoardOTS") + USB serial bench stream (SERIAL_STREAM).
// Detailed docs: README.md
//
// Valve control stays full-on / full-off.
// ============================================================================

// ---- BLE identity (consumed by BLEHandler.h #ifndef guards) -----------------
#define DEVICE_NAME            "RheoBoardOTS"
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
#include "MemoryStream.h"
#include "BLEHandler.h"
#include "OSCHandler.h"
#include <SparkFun_Qwiic_Button.h>
#include <string.h>
#include "PneumaticSystem.h"
#include "RheoSystem.h"

#define SLIP_OSC_Q_DEPTH 8
#define SLIP_OSC_MSG_MAX 128

static char slipOscQueue[SLIP_OSC_Q_DEPTH][SLIP_OSC_MSG_MAX];
static volatile uint8_t slipOscHead = 0;
static volatile uint8_t slipOscTail = 0;

class SlipDeferredOSCCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String received = pCharacteristic->getValue().c_str();
    if (received.length() == 0) return;
    noInterrupts();
    uint8_t next = (uint8_t)((slipOscTail + 1) % SLIP_OSC_Q_DEPTH);
    if (next == slipOscHead) {
      interrupts();
      return;
    }
    strncpy(slipOscQueue[slipOscTail], received.c_str(), SLIP_OSC_MSG_MAX - 1);
    slipOscQueue[slipOscTail][SLIP_OSC_MSG_MAX - 1] = '\0';
    slipOscTail = next;
    interrupts();
  }
};

static void drainDeferredBLEOSC() {
  while (true) {
    noInterrupts();
    if (slipOscHead == slipOscTail) {
      interrupts();
      return;
    }
    char local[SLIP_OSC_MSG_MAX];
    memcpy(local, slipOscQueue[slipOscHead], SLIP_OSC_MSG_MAX);
    slipOscHead = (uint8_t)((slipOscHead + 1) % SLIP_OSC_Q_DEPTH);
    interrupts();
    OSCMessage oscReceived(local);
    routeOSC(oscReceived);
  }
}

QwiicButton   qwiicButton;
bool          qwiicButtonReady = false;
char          buffer[255];

void routeOSC(OSCMessage &msg) {
  msg.getAddress(buffer, 0, 255);
  Serial.println(buffer);

  if (rheo.routeOSC(msg, buffer)) return;

  else { Serial.println("Unrecognized OSC address."); }
}

void handleQwiicButton() {
  if (!qwiicButtonReady) return;
  static bool          armed         = false;
  static bool          pressed       = false;
  static unsigned long pressStartMs  = 0;
  static bool          holdConsumed  = false;
  static int           clickCount    = 0;
  static unsigned long lastReleaseMs = 0;
  static unsigned long lastEdgeMs    = 0;

  unsigned long now     = millis();
  bool          rawDown = qwiicButton.isPressed();
  // isPressed() is often true at boot; that used to start blow() after 600 ms
  // and leave the right-pump ENB (channel 9) running. Wait for a real release first.
  if (!armed) {
    if (!rawDown) armed = true;
    return;
  }

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
      (now - pressStartMs) >= BTN_LONG_PRESS_MS && !rheo.manualBlowing
      && !rheo.isActive() && !rheo.purgeActive && !rheo.timedAirActive) {
    rheo.blow();
    holdConsumed = true;
  }

  if (clickCount > 0 && !pressed && (now - lastReleaseMs) > BTN_DOUBLE_CLICK_MS) {
    if (!rheo.isActive() && !rheo.purgeActive && !rheo.timedAirActive) {
      if (clickCount == 1) rheo.startRep();
      else                 rheo.toggleSuck();
    }
    clickCount = 0;
  }
}

#if SERIAL_STREAM
void handleSerialCommand() {
  static char    cmd[16];
  static uint8_t n = 0;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      cmd[n] = '\0'; n = 0;
      if      (!strcmp(cmd, "REP"))        { if (!rheo.actuationBusy()) rheo.startRep(); }
      else if (!strcmp(cmd, "STOP")) {
        if (rheo.bubbleActive || rheo.bubbleTriadActive) rheo.stopBubble(true);
        else if (rheo.recording) rheo.stopRep();
        if (rheo.testStreaming) rheo.stopPumpTest();
        if (rheo.purgeActive || rheo.timedAirActive) rheo.systemOff();
      }
      else if (!strncmp(cmd, "PUMP1 ", 6)) rheo.startPumpTest(PUMP_1, atoi(cmd + 6));
      else if (!strncmp(cmd, "PUMP2 ", 6)) rheo.startPumpTest(PUMP_2, atoi(cmd + 6));
      else if (!strcmp(cmd, "VALVE ON"))  rheo.valveOn();
      else if (!strcmp(cmd, "VALVE OFF")) rheo.valveOff();
    } else if (n < sizeof(cmd) - 1) {
      cmd[n++] = c;
    }
  }
}
#else
inline void handleSerialCommand() {}
#endif

void setup() {
  Serial.begin(BAUDRATE);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  rheo.setup();
  qwiicButtonReady = qwiicButton.begin();
  if (qwiicButtonReady) {
    Serial.print("Qwiic button ready (1-click=REP, 2-click=suck, hold=blow). at-boot pressed=");
    Serial.println(qwiicButton.isPressed() ? "yes (blow armed after release)" : "no");
  } else {
    Serial.println("Qwiic button not found (optional, skipping).");
  }
  setupBLE(DEVICE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID_RX, CHARACTERISTIC_UUID_TX);
  if (pRxCharacteristic != nullptr) {
    pRxCharacteristic->setCallbacks(new SlipDeferredOSCCallbacks());
  }
  Serial.println("RheoBoardOTS initialized. Send rheo/api for supported commands.");
}

void loop() {
  drainDeferredBLEOSC();

  static bool buttonPressed = false;

  bool sw1 = sw1Pressed();
  if ((digitalRead(BUTTON_PIN) == LOW || sw1) && !buttonPressed) {
    buttonPressed = true;
    if (rheo.purgeActive || rheo.timedAirActive) rheo.systemOff();
    else if (rheo.program.isRunning()) rheo.program.stop("button-abort");
    else if (rheo.bubbleActive || rheo.bubbleTriadActive) rheo.stopBubble();
    else if (rheo.recording) rheo.stopRep();
    else rheo.startRep();
    return;
  }
  if (digitalRead(BUTTON_PIN) == HIGH && !sw1) buttonPressed = false;

  handleSerialCommand();
  handleQwiicButton();
  rheo.loop();

  bool active = rheo.isActive() || rheo.manualBlowing || rheo.latchedSuck
      || rheo.testStreaming || rheo.purgeActive || rheo.timedAirActive;
  digitalWrite(LED_PIN, active ? HIGH : LOW);
  ledExt(active);

  if (!deviceConnected && oldDeviceConnected) {
    if (rheo.program.isRunning()) rheo.program.stop("disconnect");
    if (rheo.bubbleActive || rheo.bubbleTriadActive) rheo.stopBubble(true);
    else if (rheo.recording || rheo.triadActive) rheo.stopRep(true);
    rheo.systemOff();
    delay(500); pServer->startAdvertising();
    Serial.println("Started advertising again");
    oldDeviceConnected = deviceConnected;
  }
  if (deviceConnected && !oldDeviceConnected) oldDeviceConnected = deviceConnected;
}
