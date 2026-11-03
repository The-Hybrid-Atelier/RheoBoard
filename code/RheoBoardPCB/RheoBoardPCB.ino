// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE
// RheoBoardPCB: original v1 PCB + SparkFun ESP32 Thing Plus.
// PCB wiring and firmware usage: README.md.

// ---- BLE identity (consumed by BLEHandler.h #ifndef guards) -----------------
#define DEVICE_NAME            "RheoBoardPCB"
#define SERVICE_UUID           "71c978dc-2d05-4a35-b56f-12f4fee4ee31"
#define CHARACTERISTIC_UUID_RX "43d4940a-3037-4040-9c3d-6ecd138325ca"
#define CHARACTERISTIC_UUID_TX "78d88040-6306-40bd-b47d-e1feb75d6482"

// ---- Board ------------------------------------------------------------------
#define BAUDRATE    115200
#define LED_PIN     13
#define BUTTON_PIN  0

#include <Arduino.h>
#include "HardwareProfile.h"
#include "MemoryStream.h"
#include "BLEHandler.h"
#include "OSCHandler.h"
#include <string.h>
#include "PneumaticSystem.h"
#include "RheoSystem.h"
#include "ControlButtons.h"

#define SLIP_OSC_Q_DEPTH 8
#define SLIP_OSC_MSG_MAX 128

static char slipOscQueue[SLIP_OSC_Q_DEPTH][SLIP_OSC_MSG_MAX];
static volatile uint8_t slipOscHead = 0;
static volatile uint8_t slipOscTail = 0;

// BLE callbacks and loop can run on different ESP32 cores. Disabling local
// interrupts alone does not protect this shared queue across the two cores.
static portMUX_TYPE slipOscMux = portMUX_INITIALIZER_UNLOCKED;
static volatile bool slipStopRequested = false;
static void lockOscQueue() { portENTER_CRITICAL(&slipOscMux); }
static void unlockOscQueue() { portEXIT_CRITICAL(&slipOscMux); }

class SlipDeferredOSCCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) override {
    String received = pCharacteristic->getValue().c_str();
    if (received.length() == 0 || received.length() >= SLIP_OSC_MSG_MAX) return;
    OSCMessage command(received.c_str());
    if (RheoSystem::isStopCommand(command, received.c_str())) {
      lockOscQueue();
      slipStopRequested = true;
      slipOscHead = slipOscTail; // Discard commands queued before this stop.
      unlockOscQueue();
      return; // Hardware operations stay in loop, never in the BLE callback.
    }
    lockOscQueue();
    if (slipStopRequested) { unlockOscQueue(); return; }
    uint8_t next = (uint8_t)((slipOscTail + 1) % SLIP_OSC_Q_DEPTH);
    if (next == slipOscHead) {
      unlockOscQueue();
      return;
    }
    strncpy(slipOscQueue[slipOscTail], received.c_str(), SLIP_OSC_MSG_MAX - 1);
    slipOscQueue[slipOscTail][SLIP_OSC_MSG_MAX - 1] = '\0';
    slipOscTail = next;
    unlockOscQueue();
  }
};

static void drainDeferredBLEOSC() {
  while (true) {
    lockOscQueue();
    if (slipStopRequested) {
      slipStopRequested = false;
      slipOscHead = slipOscTail;
      unlockOscQueue();
      rheo.systemOff();
      return;
    }
    if (slipOscHead == slipOscTail) {
      unlockOscQueue();
      return;
    }
    char local[SLIP_OSC_MSG_MAX];
    memcpy(local, slipOscQueue[slipOscHead], SLIP_OSC_MSG_MAX);
    slipOscHead = (uint8_t)((slipOscHead + 1) % SLIP_OSC_Q_DEPTH);
    unlockOscQueue();
    OSCMessage oscReceived(local);
    routeOSC(oscReceived);
  }
}

char          buffer[255];

void routeOSC(OSCMessage &msg) {
  msg.getAddress(buffer, 0, 255);
  Serial.println(buffer);

  if (rheo.routeOSC(msg, buffer)) return;

  else { Serial.println("Unrecognized OSC address."); }
}

static char serialCommand[16];
static uint8_t serialLength = 0;
static bool serialOverflow = false;
static ControlButtons controlButtons;

static void discardPendingCommands() {
  lockOscQueue();
  slipOscHead = slipOscTail;
  slipStopRequested = false;
  unlockOscQueue();
  while (Serial.available()) Serial.read();
  serialLength = 0;
  serialOverflow = false;
}

// Returns true when physical STOP/conflicting directions consume this loop.
bool handleControlButtons() {
  static uint32_t observedStop = 0;
  static uint8_t manualOwner = 0;
  static bool physicalStop = false;
  uint8_t raw = 0;
  if (!v1Hardware::buttons(raw)) return true;
  if (observedStop != rheo.stopGeneration) {
    observedStop = rheo.stopGeneration;
    controlButtons.inhibit();
    manualOwner = 0;
  }
  controlButtons.update(raw, static_cast<uint32_t>(millis()));
  const uint8_t held = raw | controlButtons.stable();
  if ((held & 0x02) || (held & 0x0C) == 0x0C) {
    if (!physicalStop) rheo.systemOff();
    physicalStop = true;
    manualOwner = 0;
    controlButtons.inhibit();
    discardPendingCommands();
    digitalWrite(LED_PIN, LOW);
    ledExt(false);
    return true;
  }
  physicalStop = false;
  if (manualOwner) {
    // Release stops immediately; only starting an output waits for debounce.
    if (!(raw & manualOwner)) {
      rheo.systemOff();
      manualOwner = 0;
      controlButtons.inhibit();
      discardPendingCommands();
      return true;
    }
    return false;
  }
  // Ignore buttons pressed during another operation, including triad gaps.
  if (rheo.actuationBusy()) {
    controlButtons.inhibit();
    return false;
  }
  const uint8_t pressed = controlButtons.pressed();
  if (pressed & 0x01) rheo.startRep();
  else if (pressed & 0x04) { rheo.suck(); manualOwner = 0x04; }
  else if (pressed & 0x08) { rheo.blow(); manualOwner = 0x08; }
  return false;
}

#if SERIAL_STREAM
void handleSerialCommand() {
  char* cmd = serialCommand;
  uint8_t& n = serialLength;
  bool& overflow = serialOverflow;
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      cmd[n] = '\0'; n = 0;
      if (overflow) { overflow = false; Serial.println("#ERROR,command too long"); continue; }
      if (!strcmp(cmd, "PRESSURE")) {
        const float pressure = readPressure();
        if (isfinite(pressure)) { Serial.print("#PRESSURE,"); Serial.println(pressure, 1); }
        else Serial.println("#ERROR,pressure sensor fault");
        continue;
      }
      if (!strcmp(cmd, "BUTTONS")) {
        uint8_t pressed;
        if (v1Hardware::buttons(pressed)) { Serial.print("#BUTTONS,"); Serial.println(pressed, BIN); }
        continue;
      }
      if      (!strcmp(cmd, "REP"))        { if (!rheo.actuationBusy()) rheo.startRep(); }
      else if (!strcmp(cmd, "STOP") || !strcmp(cmd, "VALVES OFF")) {
        if (rheo.testStreaming) Serial.println("#TEST_END");
        rheo.systemOff();
      }
      else if (!strncmp(cmd, "PUMP1 ", 6)) rheo.startPumpTest(PUMP_1, atoi(cmd + 6));
      else if (!strncmp(cmd, "PUMP2 ", 6)) rheo.startPumpTest(PUMP_2, atoi(cmd + 6));
      else if (!strcmp(cmd, "VALVE ON"))  rheo.valveOn();
      else if (!strcmp(cmd, "VALVE OFF")) rheo.valveOff();
      else if (!strcmp(cmd, "VALVES PRESS")) rheo.valveOn();
      else if (!strcmp(cmd, "VALVES VAC")) rheo.valveOff();
      else if (!strcmp(cmd, "VALVES REST")) rheo.valvesRest();
      else if (!strcmp(cmd, "U9 ON")) rheo.valvePhysical(1, true);
      else if (!strcmp(cmd, "U9 OFF")) rheo.valvePhysical(1, false);
      else if (!strcmp(cmd, "U10 ON")) rheo.valvePhysical(0, true);
      else if (!strcmp(cmd, "U10 OFF")) rheo.valvePhysical(0, false);
    } else if (n < sizeof(serialCommand) - 1) {
      cmd[n++] = c;
    } else { overflow = true; }
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
  Serial.println("v1 buttons: SW1 REP; SW2 STOP; SW3 hold vacuum; SW4 hold pressure.");
  setupBLE(DEVICE_NAME, SERVICE_UUID, CHARACTERISTIC_UUID_RX, CHARACTERISTIC_UUID_TX);
  if (pRxCharacteristic != nullptr) {
    pRxCharacteristic->setCallbacks(new SlipDeferredOSCCallbacks());
  }
  Serial.println("RheoBoardPCB initialized. Send rheo/api for supported commands.");
}

void loop() {
  // Hardware faults are latched until restart; no queued command may rearm.
  if (!pwmReady()) {
    static bool reported = false;
    if (!reported) {
      if (rheo.testStreaming) Serial.println("#TEST_END");
      rheo.systemOff();
      Serial.println("v1 halted after hardware fault. Remove 12 V, inspect, then restart.");
      reported = true;
    }
    delay(10);
    return;
  }
  // Cancel stale writes before servicing the command queue on disconnect.
  if (!deviceConnected && oldDeviceConnected) {
    lockOscQueue();
    slipOscHead = slipOscTail;
    slipStopRequested = false;
    unlockOscQueue();
    rheo.systemOff();
    oldDeviceConnected = false;
    pServer->startAdvertising();
    return;
  }
  if (handleControlButtons()) { delay(1); return; }
  drainDeferredBLEOSC();

  static bool buttonPressed = false;

  bool sw1 = sw1Pressed();
  if ((digitalRead(BUTTON_PIN) == LOW || sw1) && !buttonPressed) {
    buttonPressed = true;
    if (rheo.actuationBusy()) rheo.systemOff();
    else rheo.startRep();
    return;
  }
  if (digitalRead(BUTTON_PIN) == HIGH && !sw1) buttonPressed = false;

  handleSerialCommand();
  rheo.loop();

  bool active = rheo.isActive() || rheo.manualBlowing || rheo.latchedSuck
      || rheo.testStreaming || rheo.purgeActive || rheo.timedAirActive;
  digitalWrite(LED_PIN, active ? HIGH : LOW);
  ledExt(active);

  if (deviceConnected && !oldDeviceConnected) oldDeviceConnected = deviceConnected;
}
