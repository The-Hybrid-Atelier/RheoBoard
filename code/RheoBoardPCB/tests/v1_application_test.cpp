// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <Wire.h>
#include <BLEHandler.h>
#include "RheoSystem.h"
#include <iostream>
#include <cassert>
// Include the real PCB sketch. This is a host test, not another Arduino sketch.
#include "RheoBoardPCB.ino"

static void check(bool value, const char *message) {
  if (!value) { std::cerr << "FAIL: " << message << " at " << millis() << " ms\n"; std::exit(1); }
}
static bool pressureValves() { return Wire.tca[1] == RHEO_V1_PRESSURE_VALVES; }
static bool vacuumValves() { return Wire.tca[1] == RHEO_V1_VACUUM_VALVES; }
static bool restValves() { return Wire.tca[1] == RHEO_V1_REST_VALVES; }
static void invariant() {
  check(Wire.pumpOff(0) || Wire.pumpOff(1), "both pumps commanded on");
  check(!(Wire.tca[1] & 0xFC), "non-valve expander output commanded on");
  for (int ch = 2; ch < 16; ++ch) check(Wire.pumpOff(ch), "unused PWM output commanded on");
}
static void tick(unsigned ms = 1) { delay(ms); loop(); invariant(); }
static void runFor(unsigned ms) {
  const auto end = millis() + ms;
  while (millis() < end) tick();
}
static void buttons(uint8_t pressed) {
  Wire.tca[0] = static_cast<uint8_t>(~(pressed << 4));
  tick(); runFor(30);
}
static void serial(const char *command) { Serial.inject(std::string(command) + "\n"); tick(); }
static void ble(const char *command) { fakeRx.receive(command); tick(); }
static void allOff() {
  check(Wire.pumpOff(0) && Wire.pumpOff(1) && Wire.tca[1] == 0, "outputs not all off");
}
static void stopped() {
  allOff(); check(!rheo.actuationBusy(), "stop left an active state");
  for (int i = 0; i < 1600; ++i) { tick(); allOff(); }
}
int main(int argc, char **argv) {
  check(argc == 2, "scenario name required");
  const std::string test = argv[1];
  if (test == "button_boot") Wire.tca[0] = static_cast<uint8_t>(~(0x09 << 4));
  setup(); allOff(); runFor(30);
  if (test == "serial") {
    serial("PRESSURE"); allOff();
    serial("PUMP1 100"); check(!Wire.pumpOff(PWM_CH_PUMP1) && vacuumValves(), "PUMP1 mapping");
    serial("STOP"); stopped();
    serial("PUMP2 50"); check(!Wire.pumpOff(PWM_CH_PUMP2) && pressureValves(), "PUMP2 mapping");
    serial("STOP"); stopped();
    serial("VALVE ON"); check(pressureValves(), "VALVE ON");
    serial("VALVE OFF"); check(vacuumValves(), "legacy vacuum state");
    serial("VALVES OFF"); stopped();
  } else if (test == "dual_valves") {
    serial("U9 ON"); check(Wire.tca[1] == 2, "U9 must be P1");
    serial("U10 ON"); check(Wire.tca[1] == 3, "U10 must preserve U9");
    serial("U9 OFF"); check(Wire.tca[1] == 1, "U9 must preserve U10");
    serial("U10 OFF"); allOff();
    ble("/valve/u10/on"); ble("/valve/u9/on"); check(Wire.tca[1] == 3, "BLE both valves");
    ble("/valve/u10/off"); check(Wire.tca[1] == 2, "BLE individual off");
    ble("/valve/u9/off"); allOff();
    serial("VALVES PRESS"); check(pressureValves(), "serial pressure state");
    serial("VALVES VAC"); check(vacuumValves(), "serial vacuum state");
    serial("VALVES REST"); check(restValves(), "serial rest state");
    ble("/valves/pressure"); check(pressureValves(), "BLE pressure state");
    ble("/valves/vacuum"); check(vacuumValves(), "BLE vacuum state");
    ble("/valves/rest"); check(restValves(), "BLE rest state");
    ble("/valves/off"); stopped();
    serial("PUMP1 25"); const auto mask = Wire.tca[1];
    serial("U9 ON"); ble("/valve/u10/off"); ble("/valves/pressure");
    serial("VALVE ON"); check(Wire.tca[1] == mask, "manual valve command interrupted pump test");
    serial("STOP"); stopped();
    serial("U9 ON"); serial("U10 ON"); buttons(2); stopped(); buttons(0);
  } else if (test == "auxiliary_routes") {
    ble("/slip/pump/add 30 100"); check(pressureValves(), "timed pressure route");
    runFor(120); check(!rheo.actuationBusy() && restValves(), "timed pressure rest");
    ble("/slip/pump/remove 30 100"); check(vacuumValves(), "timed vacuum route");
    runFor(120); check(!rheo.actuationBusy() && restValves(), "timed vacuum rest");
    ble("/pump/purge"); check(pressureValves(), "purge pressure route");
    runFor(1050); check(!rheo.actuationBusy() && restValves(), "purge rest");
    serial("STOP"); stopped();
  } else if (test == "program_routes") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 0 0 0 0 100");
    ble("/rheo/program/step 1 -1 50 50 100");
    ble("/rheo/program/step 3 1 20 80 200");
    ble("/rheo/program/run"); runFor(40);
    check(restValves() && Wire.pumpOff(0) && Wire.pumpOff(1), "REST route");
    runFor(100); check(vacuumValves() && !Wire.pumpOff(PWM_CH_PUMP1), "RETRACT route");
    runFor(100); check(pressureValves() && !Wire.pumpOff(PWM_CH_PUMP2), "RAMP route");
    runFor(230); check(!rheo.program.isRunning() && restValves(), "normal program rest");
    serial("STOP"); stopped();
  } else if (test == "pulse_routes") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 4 1 50 50 250 2 100 50");
    ble("/rheo/program/run"); runFor(20); check(pressureValves(), "pulse pressure gate");
    runFor(100); check(restValves(), "pulse gap must use rest, not vacuum");
    runFor(50); check(pressureValves(), "second pulse pressure gate");
    runFor(130); check(restValves() && !rheo.program.isRunning(), "pulse final rest");
    serial("STOP"); stopped();
  } else if (test == "bubble_routes") {
    ble("/rheo/bubble"); check(pressureValves(), "bubble pressure gate");
    runFor(75); check(restValves() && Wire.pumpOff(0) && Wire.pumpOff(1), "bubble rest gate");
    runFor(200); check(!rheo.actuationBusy() && restValves(), "bubble completion");
    serial("STOP"); stopped();
  } else if (test == "triad_complete") {
    ble("/rheo/rep/triad"); runFor(6000);
    check(!rheo.actuationBusy() && restValves(), "triad did not finish at rest");
    size_t recordings = 0;
    for (const auto &line : Serial.messages) if (line == "#REP_START") ++recordings;
    check(recordings == 3, "triad must record three REPs");
    serial("STOP"); stopped();
    ble("/rheo/bubble/triad"); runFor(3500);
    check(!rheo.actuationBusy() && restValves(), "bubble triad did not finish at rest");
    size_t bubbles = 0;
    for (const auto &line : Serial.messages) if (line.find("#BUBBLE_START,") == 0) ++bubbles;
    check(bubbles == 3, "bubble triad must record three bubbles");
    serial("STOP"); stopped();
  } else if (test == "rep") {
    serial("REP"); runFor(280);
    check(!Wire.pumpOff(PWM_CH_PUMP1) && vacuumValves(), "REP retract");
    runFor(250); check(!Wire.pumpOff(PWM_CH_PUMP2) && pressureValves(), "REP push");
    runFor(250); check(Wire.pumpOff(0) && Wire.pumpOff(1) && restValves(), "REP relax");
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
    buttons(1); check(rheo.recording, "SW1 must start REP");
    buttons(2); stopped(); buttons(0);
    buttons(4); check(rheo.latchedSuck && !Wire.pumpOff(PWM_CH_PUMP1) && vacuumValves(), "SW3 vacuum");
    buttons(0); stopped();
    buttons(8); check(rheo.manualBlowing && !Wire.pumpOff(PWM_CH_PUMP2) && pressureValves(), "SW4 pressure");
    buttons(0); stopped();
  } else if (test == "button_boot") {
    runFor(700); stopped(); // Held-at-boot SW1/SW4 cannot actuate.
    buttons(0); buttons(8);
    check(rheo.manualBlowing, "button did not rearm after boot release");
    buttons(0); stopped();
  } else if (test == "button_debounce") {
    for (int i = 0; i < 4; ++i) {
      Wire.tca[0] &= ~0x10; tick(); runFor(10);
      Wire.tca[0] |= 0x10; tick(); runFor(10);
    }
    check(!rheo.recording, "contact bounce started REP");
    buttons(1); check(rheo.recording, "stable SW1 press did not start REP");
    runFor(2000); check(!rheo.recording, "held SW1 retriggered REP");
    buttons(2); stopped(); buttons(0);
  } else if (test == "button_busy") {
    serial("PUMP1 25"); buttons(8);
    check(!rheo.manualBlowing && rheo.testStreaming && !Wire.pumpOff(PWM_CH_PUMP1),
          "SW4 took over a pump test");
    buttons(0); check(rheo.testStreaming, "unowned release stopped pump test");
    serial("STOP"); stopped();
    ble("/rheo/rep/interval 2000"); ble("/rheo/rep/triad"); runFor(1250);
    check(rheo.triadActive && !rheo.recording, "expected triad gap");
    buttons(8); runFor(100);
    check(!rheo.manualBlowing && rheo.triadActive, "SW4 took over a triad gap");
    buttons(0); serial("STOP"); stopped();
    ble("/slip/pump/add 25 100"); buttons(8); runFor(700);
    check(!rheo.actuationBusy() && restValves(), "held button restarted after timed air completed");
    buttons(0); serial("STOP"); stopped();
  } else if (test == "button_stop") {
    buttons(8); check(rheo.manualBlowing, "SW4 before stop");
    serial("STOP"); stopped(); // SW4 still held; cannot restart.
    buttons(0); buttons(8); check(rheo.manualBlowing, "SW4 after fresh press");
    buttons(10); stopped(); // STOP plus pressure.
    for (int i = 0; i < 8; ++i) fakeRx.receive("/rheo/rep");
    Serial.inject("PUMP1 100\n"); tick(); stopped();
    buttons(8); stopped(); // Releasing STOP while pressure held cannot restart.
    buttons(0); buttons(4); check(rheo.latchedSuck, "SW3 after release");
    buttons(12); stopped(); // Opposing buttons stop; neither direction wins.
    buttons(4); stopped();
    buttons(0); buttons(4); check(rheo.latchedSuck, "SW3 after conflict release");
    buttons(0); stopped();
  } else if (test == "physical_stop_matrix") {
    const char *starts[] = {"/rheo/rep", "/rheo/rep/triad", "/rheo/bubble/triad",
                           "/pump/blow 100", "/pump/suck 100", "/rheo/purge",
                           "/slip/pump/add 100 5000"};
    for (const char *start : starts) {
      ble(start); runFor(40); buttons(2); stopped(); buttons(0);
    }
    ble("/rheo/program/clear"); ble("/rheo/program/step 2 1 100 100 1000");
    ble("/rheo/program/run"); runFor(40); buttons(2); stopped(); buttons(0);
  } else if (test == "stop_matrix") {
    const char *starts[] = {"/rheo/rep", "/rheo/rep/triad", "/rheo/bubble/triad",
                           "/pump/blow 100", "/pump/suck 100", "/rheo/purge",
                           "/slip/pump/add 100 5000"};
    const char *stops[] = {"/system/off", "/pump/off", "/slip/pump/off", "/valves/off", "/rheo/stop",
                          "/slip/pa/stop", "/pump/blow 0", "/pump/suck 0",
                          "/slip/pump/add 0", "/slip/pump/remove 0", "/rheo/program/stop"};
    for (const char *start : starts) for (const char *stop : stops) {
      ble(start); runFor(40); ble(stop); stopped();
    }
  } else if (test == "queue_stop") {
    for (const char *stop : {"/system/off", "/valves/off"}) {
      ble("/pump/blow 100");
      for (int i = 0; i < 8; ++i) fakeRx.receive("/rheo/rep");
      fakeRx.receive(stop);
      fakeRx.receive("/pump/blow 100"); // Cannot overtake the pending stop.
      tick(); stopped();
    }
  } else if (test == "program_complete") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 1 -1 100 100 100");
    ble("/rheo/program/step 2 1 100 100 100");
    ble("/rheo/program/run"); runFor(250);
    check(!rheo.program.isRunning() && restValves(), "program completion rest");
    serial("STOP"); stopped();
  } else if (test == "oversize") {
    serial("PUMP1 100000000000000000000"); allOff();
    serial("PUMP1 20"); check(!Wire.pumpOff(PWM_CH_PUMP1), "serial recovery");
    serial("STOP"); stopped();
    fakeRx.receive(std::string("/pump/blow 100 ") + std::string(150, 'x'));
    tick(); allOff();
  } else if (test == "disconnect_queue") {
    deviceConnected = true; tick();
    fakeRx.receive("/pump/blow 100");
    deviceConnected = false;
    const auto before = Wire.transactions.size();
    tick(); stopped();
    for (size_t i = before; i < Wire.transactions.size(); ++i) {
      const auto &t = Wire.transactions[i];
      check(t.read || t.address != 0x40 || t.bytes.size() != 5,
            "queued pump write executed after disconnect");
    }
  } else if (test == "program_fault") {
    ble("/rheo/program/clear");
    ble("/rheo/program/step 2 1 100 100 1000");
    ble("/rheo/program/run"); runFor(20);
    Wire.sensorStatus = 0x41; tick(20); stopped();
    check(!pwmReady(), "program sensor failure did not latch");
  } else { check(false, "unknown scenario"); }
  std::cout << "PASS application: " << test << '\n';
}
