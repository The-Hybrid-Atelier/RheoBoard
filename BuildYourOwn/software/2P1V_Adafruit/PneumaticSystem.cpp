// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include <Wire.h>
#include "PneumaticSystem.h"
#include "OSCHandler.h"
#include "BLEHandler.h"   // pTxCharacteristic (sendApiEntry notifies /rheo/api replies directly)
#include "MemoryStream.h"

SparkFun_MicroPressure mpr;
Adafruit_seesaw ss;

bool halBegin() {
  Wire.begin();

  bool seesawOk = ss.begin(SEESAW_I2C_ADDR);
  ss.pinMode(SS_PUMP1_EN,  OUTPUT);
  ss.pinMode(SS_PUMP2_EN,  OUTPUT);
  ss.pinMode(SS_VALVE1_EN, OUTPUT);
  ss.pinMode(SS_VALVE2_EN, OUTPUT);
  if (seesawOk) {
    ss.setPWMFreq(SS_PUMP1_EN, PWM_FREQ_HZ);
    ss.setPWMFreq(SS_PUMP2_EN, PWM_FREQ_HZ);
  }
  // Safe idle: pumps off, valve resting on the de-energized (vacuum) pole.
  // Reserved VALVE1 pin is held low even though it is physically NC.
  pumpsOff();
  ss.digitalWrite(SS_VALVE1_EN, LOW);
  valveSuck();

  if (PIN_SW1  >= 0) pinMode(PIN_SW1, INPUT_PULLUP);
  if (PIN_LED1 >= 0) { pinMode(PIN_LED1, OUTPUT); digitalWrite(PIN_LED1, LOW); }

  bool mprOk = mpr.begin(MPRLS_I2C_ADDR, Wire);
  if (!seesawOk) Serial.println("PneumaticSystem: ATtiny1616 seesaw board not found on Qwiic bus.");
  if (!mprOk)    Serial.println("PneumaticSystem: MPRLS pressure sensor not found on Qwiic bus.");
  return seesawOk && mprOk;
}

// ---- Pump / valve -----------------------------------------------------------

void PneumaticSystem::blow(int power) {
  cancelSuck();
  pumpSet(PUMP_1, false); valveBlow();
  delay(VALVE_SETTLE_MS);
  pumpSetPct(PUMP_2, power);
  manualBlowing = true;
}
void PneumaticSystem::stopBlow() { pumpsOff(); valveSuck(); manualBlowing = false; }

void PneumaticSystem::suck(int power) {
  manualBlowing = false;
  pumpSet(PUMP_2, false); valveSuck();
  delay(VALVE_SETTLE_MS);
  pumpSetPct(PUMP_1, power);
  latchedSuck = true;
}
void PneumaticSystem::stopSuck()   { pumpsOff(); latchedSuck = false; }
void PneumaticSystem::cancelSuck() { if (latchedSuck) stopSuck(); }
void PneumaticSystem::toggleSuck() { if (latchedSuck) stopSuck(); else suck(-1); }

void PneumaticSystem::off()     { pumpsOff(); }
void PneumaticSystem::purge()   {
  pumpSet(PUMP_1, false); valveBlow(); delay(VALVE_SETTLE_MS);
  pumpSet(PUMP_2, true); delay(1000);
  pumpsOff(); valveSuck();
}
void PneumaticSystem::valveOn()  { valveBlow(); }
void PneumaticSystem::valveOff() { valveSuck(); }
void PneumaticSystem::systemOff() {
  pumpsOff(); valveSuck();
  manualBlowing = false; latchedSuck = false; testStreaming = false;
}

// ---- Sensing ----------------------------------------------------------------

void PneumaticSystem::sampleAir() {
  pressure = readPressure();
  sendSampleOSC("/rheo/sense/air", pressure);
#if SERIAL_STREAM
  Serial.print("#S,"); Serial.print(millis() - sessionStart());
  Serial.print(','); Serial.println(pressure, 1);
#endif
  delay(rate);
}

void PneumaticSystem::sampleTest() {
  Serial.print("#S,"); Serial.print(millis() - testStart);
  Serial.print(','); Serial.println(readPressure(), 1);
  delay(rate);
}

// ---- Pump test --------------------------------------------------------------

bool PneumaticSystem::isActive() const { return false; }

void PneumaticSystem::startPumpTest(PumpChannel ch, int pct) {
  if (isActive() || manualBlowing || latchedSuck || testStreaming) return;
  pct = constrain(pct, 0, 100);
  if (ch == PUMP_1) { pumpSet(PUMP_2, false); valveSuck(); }
  else              { pumpSet(PUMP_1, false); valveBlow(); }
  delay(VALVE_SETTLE_MS);
  pumpSetPct(ch, pct);
  testStreaming = true;
  testStart = millis();
  Serial.print("#TEST_START,"); Serial.println(readPressure(), 1);
}

void PneumaticSystem::stopPumpTest() {
  pumpsOff(); valveSuck();
  testStreaming = false;
  Serial.println("#TEST_END");
}

void sendApiEntry(const char* entry) {
  OSCMessage msg("/rheo/api");
  msg.add(entry);
  uint8_t buf[256];
  MemoryStream ms(buf, sizeof(buf));
  msg.send(ms);
  pTxCharacteristic->setValue(buf, ms.length());
  pTxCharacteristic->notify();
}

// ---- Setup / Loop -----------------------------------------------------------

void PneumaticSystem::setup() {
  if (!halBegin()) {
    Serial.println("PneumaticSystem: HAL init failed (seesaw board and/or MPRLS not found).");
    while (1) delay(1000);
  }
}

void PneumaticSystem::loop() {
  if (testStreaming) sampleTest();
}

// ---- OSC --------------------------------------------------------------------

void PneumaticSystem::printApi() {
  const char* api[] = {
    "/pump/blow [<0-100>]",
    "/pump/suck [<0-100>]",
    "/pump/off",
    "/pump/purge",
    "/valve/on",
    "/valve/off",
    "/system/off",
    "/rheo/sense/rate [<ms>]",
  };
  for (const char* line : api) { Serial.println(line); sendApiEntry(line); }
}

bool PneumaticSystem::routeOSC(OSCMessage& msg, char* buffer) {
  if (msg.match("/pump/blow*"))  { blow(strchr(buffer,' ') ? constrain(getIntArg(msg,1),0,100) : 100); return true; }
  if (msg.match("/pump/suck*"))  { suck(strchr(buffer,' ') ? constrain(getIntArg(msg,1),0,100) : -1);  return true; }
  if (msg.match("/pump/off"))    { off();       return true; }
  if (msg.match("/pump/purge"))  { purge();     return true; }
  if (msg.match("/valve/on"))    { valveOn();   return true; }
  if (msg.match("/valve/off"))   { valveOff();  return true; }
  if (msg.match("/system/off"))  { systemOff(); return true; }
  if (msg.match("/rheo/sense/rate*")) {
    if (strchr(buffer,' ')) rate = constrain(getIntArg(msg,1), MIN_RATE, MAX_RATE);
    sendSampleOSC("/rheo/sense/rate", (float)rate);
    Serial.print("/rheo/sense/rate="); Serial.println(rate);
    return true;
  }
  return false;
}
