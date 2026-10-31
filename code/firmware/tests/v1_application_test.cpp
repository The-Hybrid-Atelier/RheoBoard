// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <Wire.h>
#include <BLEHandler.h>
#include "RheoSystem.h"
#include <iostream>
#include <cassert>
// Include the sole real sketch. This is a host test, not another Arduino sketch.
#include "RheoBoardOTS.ino"

static void check(bool value, const char *message) {
  if (!value) { std::cerr << "FAIL: " << message << " at " << millis() << " ms\n"; std::exit(1); }
}
static bool valvePowered() { return Wire.tca[1] & (1 << RHEO_V1_VALVE_PORT); }
static void invariant() {
  check(Wire.pumpOff(0) || Wire.pumpOff(1), "both pumps commanded on");
  check(!(Wire.tca[1] & (1 << (1 - RHEO_V1_VALVE_PORT))), "unused valve commanded on");
  for (int ch = 2; ch < 16; ++ch) check(Wire.pumpOff(ch), "unused PWM output commanded on");
}
static void tick(unsigned ms = 1) { delay(ms); loop(); invariant(); }
static void runFor(unsigned ms) {
  const auto end = millis() + ms;
  while (millis() < end) tick();
}
static void serial(const char *command) { Serial.inject(std::string(command) + "\n"); tick(); }
static void ble(const char *command) { fakeRx.receive(command); tick(); }
static void allOff() {
  check(Wire.pumpOff(0) && Wire.pumpOff(1) && !valvePowered(), "outputs not all off");
}
static void stopped() {
  allOff(); check(!rheo.actuationBusy(), "stop left an active state");
  for (int i = 0; i < 1600; ++i) { tick(); allOff(); }
}
int main(int argc, char **argv) {
  check(argc == 2, "scenario name required");
  const std::string test = argv[1];
  setup(); allOff(); runFor(30);
  if (test == "serial") {
    serial("PUMP1 100"); check(!Wire.pumpOff(PWM_CH_PUMP1) && !valvePowered(), "PUMP1 mapping");
    serial("STOP"); stopped();
    serial("PUMP2 50"); check(!Wire.pumpOff(PWM_CH_PUMP2) && valvePowered(), "PUMP2 mapping");
    serial("STOP"); stopped();
    serial("VALVE ON"); check(valvePowered(), "VALVE ON");
    serial("VALVE OFF"); allOff();
  } else if (test == "rep") {
    serial("REP"); runFor(280);
    check(!Wire.pumpOff(PWM_CH_PUMP1) && !valvePowered(), "REP retract");
    runFor(250); check(!Wire.pumpOff(PWM_CH_PUMP2) && valvePowered(), "REP push");
    runFor(250); check(Wire.pumpOff(0) && Wire.pumpOff(1) && valvePowered(), "REP relax");
    runFor(500); check(!rheo.recording && rheo.repSamplesSent == 66, "REP completion");
    serial("STOP"); stopped();
  } else if (test == "system_stop") {
    ble("/rheo/rep"); runFor(280); ble("/system/off"); stopped();
  } else if (test == "pump_zero") {
    ble("/pump/blow 100"); check(!Wire.pumpOff(PWM_CH_PUMP2), "BLE blow start");
    ble("/pump/blow 0"); stopped();
    ble("/pump/suck 100"); check(!Wire.pumpOff(PWM_CH_PUMP1), "BLE suck start");
    ble("/pump/suck 0"); stopped();
  } else if (test == "disconnect") {
    deviceConnected = true; tick(); ble("/rheo/rep"); runFor(280);
    deviceConnected = false; tick(); stopped();
  } else if (test == "program") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 1 -1 100 100 200");
    ble("/rheo/program/step 2 1 100 100 200");
    ble("/rheo/program/run"); runFor(50);
    check(!Wire.pumpOff(PWM_CH_PUMP1), "program retract");
    runFor(200); check(!Wire.pumpOff(PWM_CH_PUMP2), "program push");
    serial("STOP"); stopped();
  } else if (test == "triad_stop") {
    ble("/rheo/rep/triad"); runFor(1250); serial("STOP"); stopped();
  } else if (test == "sensor_fault") {
    serial("PUMP1 100"); Wire.sensorStatus = 0x44; tick(); tick(); stopped();
    serial("PUMP2 100"); allOff(); check(!v1Hardware::ready(), "fault rearmed");
  } else if (test == "bubble_fault") {
    ble("/rheo/bubble"); Wire.sensorStatus = 0x44; tick(); tick(); stopped();
  } else if (test == "button") {
    Wire.tca[0] &= ~0x10; tick(30); runFor(620);
    check(!Wire.pumpOff(PWM_CH_PUMP2), "hold-to-blow");
    Wire.tca[0] |= 0x10; tick(30); stopped();
  } else if (test == "button_stop") {
    // A click queued before STOP must not launch a delayed REP afterward.
    Wire.tca[0] &= ~0x10; tick(30);
    Wire.tca[0] |= 0x10; tick(30);
    serial("STOP"); stopped();
    Wire.tca[0] &= ~0x10; tick(30); runFor(620);
    check(!Wire.pumpOff(PWM_CH_PUMP2), "hold before stop");
    serial("STOP"); stopped(); // SW1 remains held throughout.
    Wire.tca[0] |= 0x10; tick(30); stopped();
  } else if (test == "stop_matrix") {
    const char *starts[] = {"/rheo/rep", "/rheo/rep/triad", "/rheo/bubble/triad",
                           "/pump/blow 100", "/pump/suck 100", "/rheo/purge",
                           "/slip/pump/add 100 5000"};
    const char *stops[] = {"/system/off", "/pump/off", "/slip/pump/off", "/rheo/stop",
                          "/slip/pa/stop", "/pump/blow 0", "/pump/suck 0",
                          "/slip/pump/add 0", "/slip/pump/remove 0", "/rheo/program/stop"};
    for (const char *start : starts) for (const char *stop : stops) {
      ble(start); runFor(40); ble(stop); stopped();
    }
  } else if (test == "queue_stop") {
    ble("/pump/blow 100");
    for (int i = 0; i < 8; ++i) fakeRx.receive("/rheo/rep");
    fakeRx.receive("/system/off");
    fakeRx.receive("/pump/blow 100"); // Cannot overtake the pending stop.
    tick(); stopped();
  } else if (test == "program_complete") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 1 -1 100 100 100");
    ble("/rheo/program/step 2 1 100 100 100");
    ble("/rheo/program/run"); runFor(250); stopped();
  } else { check(false, "unknown scenario"); }
  std::cout << "PASS application: " << test << '\n';
}
