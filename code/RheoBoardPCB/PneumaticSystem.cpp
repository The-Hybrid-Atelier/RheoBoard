// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE
#include <Wire.h>
#include "PneumaticSystem.h"
#include "OSCHandler.h"
#include "BLEHandler.h"   // pTxCharacteristic (sendApiEntry notifies /rheo/api replies directly)
#include "MemoryStream.h"


bool pwmReady() { return v1Hardware::ready(); }
static void holdUnusedChannelOff() {} // v1 parks unused outputs during init.
static void parkPumps() { v1Hardware::pumpsOff(); }
void pumpSetPct(PumpChannel c, int pct) {
  v1Hardware::pump(c == PUMP_1 ? PWM_CH_PUMP1 : PWM_CH_PUMP2, pct);
}
void pumpSet(PumpChannel c, bool on) { pumpSetPct(c, on ? 100 : 0); }
void pumpsOff() { parkPumps(); }
void pumpsOffNow() { parkPumps(); }
void valveSuck() { v1Hardware::valves(RHEO_V1_VACUUM_VALVES); }
void valveBlow() { v1Hardware::valves(RHEO_V1_PRESSURE_VALVES); }
void valveRest() { v1Hardware::valves(RHEO_V1_REST_VALVES); }
void valvesOff() { v1Hardware::valves(0); }
bool halBegin() { return v1Hardware::begin(); }

// ---- Pump / valve -----------------------------------------------------------

void PneumaticSystem::blow(int power) {
  // Controller sliders send an explicit zero at release. Treat that as a real
  // stop so manualBlowing cannot remain latched and block the next REP.
  if (power <= 0) { systemOff(); return; }
  cancelSuck();
  pumpSet(PUMP_1, false); valveBlow();
  delay(VALVE_SETTLE_MS);
  pumpSetPct(PUMP_2, power);
  manualBlowing = true;
}
void PneumaticSystem::stopBlow() { pumpsOffNow(); valvesOff(); manualBlowing = false; }

void PneumaticSystem::suck(int power) {
  // Match blow(0): zero means stop, not a zero-drive latched manual mode.
  if (power == 0) { systemOff(); return; }
  manualBlowing = false;
  pumpSet(PUMP_2, false); valveSuck();
  delay(VALVE_SETTLE_MS);
  pumpSetPct(PUMP_1, power);
  latchedSuck = true;
}
void PneumaticSystem::stopSuck()   { pumpsOffNow(); valvesOff(); latchedSuck = false; }
void PneumaticSystem::cancelSuck() { if (latchedSuck) stopSuck(); }
void PneumaticSystem::toggleSuck() { if (latchedSuck) stopSuck(); else suck(-1); }

void PneumaticSystem::off()     { systemOff(); }
void PneumaticSystem::purge()   {
  cancelSuck();
  stopTimedAir();
  manualBlowing = false;
  pumpSet(PUMP_1, false);
  valveBlow();
  pumpSetPct(PUMP_2, PURGE_PCT);
  purgeActive = true;
  purgeEndsAt = millis() + 1000UL;
}
void PneumaticSystem::valveOn()  { if (!actuationBusy()) valveBlow(); }
void PneumaticSystem::valveOff() { if (!actuationBusy()) valveSuck(); }
void PneumaticSystem::valvesRest() { if (!actuationBusy()) valveRest(); }
bool PneumaticSystem::valvePhysical(uint8_t port, bool on) {
  if (actuationBusy() || !pwmReady() || port > 1) return false;
  v1Hardware::valvePort(port, on);
  return pwmReady();
}
void PneumaticSystem::systemOff() {
  purgeActive = false;
  purgeEndsAt = 0;
  timedAirActive = false;
  timedAirStopsAt = 0;
  pumpsOffNow(); valvesOff();
  manualBlowing = false; latchedSuck = false; testStreaming = false;
}

bool PneumaticSystem::actuationBusy() const {
  return manualBlowing || latchedSuck || testStreaming || purgeActive || timedAirActive;
}

void PneumaticSystem::startTimedAir(bool blowDir, int power, int duration) {
  if (actuationBusy()) return;
  if (power <= 0) { systemOff(); return; }
  if (blowDir) blow(power);
  else suck(power);
  timedAirActive = true;
  timedAirIsBlow = blowDir;
  timedAirStopsAt = millis() + static_cast<unsigned long>(duration);
}

void PneumaticSystem::stopTimedAir() {
  if (!timedAirActive) return;
  if (timedAirIsBlow) stopBlow();
  else stopSuck();
  valveRest();
  timedAirActive = false;
  timedAirStopsAt = 0;
}

void PneumaticSystem::updateAuxiliary() {
  const unsigned long now = millis();
  if (purgeActive && static_cast<long>(now - purgeEndsAt) >= 0) {
    pumpsOff();
    valveRest();
    purgeActive = false;
    purgeEndsAt = 0;
  }
  if (timedAirActive && static_cast<long>(now - timedAirStopsAt) >= 0) {
    stopTimedAir();
  }
}

// ---- Sensing ----------------------------------------------------------------

void PneumaticSystem::sampleAir() {
  pressure = readPressure();
  // ThingPlusBLEOSC's sendSampleOSC accepts int. Do not convert a failed
  // sensor reading (NaN) into an integer; the driver has already latched OFF.
  if (!isfinite(pressure)) return;
  sendSampleOSC("/rheo/sense/air", pressure);
#if SERIAL_STREAM
  Serial.print("#S,"); Serial.print(millis() - sessionStart());
  Serial.print(','); Serial.println(pressure, 1);
#endif
  // rate one-tick delays wait exactly as long as delay(rate), so sample spacing is unchanged.
  for (int i = 0; i < rate; i++) {
    onSampleWait();
    delay(1);
  }
}

void PneumaticSystem::sampleTest() {
  Serial.print("#S,"); Serial.print(millis() - testStart);
  Serial.print(','); Serial.println(readPressure(), 1);
  delay(rate);
}

// ---- Pump test --------------------------------------------------------------

bool PneumaticSystem::isActive() const { return false; }

void PneumaticSystem::startPumpTest(PumpChannel ch, int pct) {
  if (isActive() || actuationBusy()) return;
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
  pumpsOff(); valvesOff();
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
    Serial.println("PneumaticSystem: hardware initialization failed; actuation disabled. Check the selected hardware profile, supplies and I2C devices.");
    while (1) delay(1000);
  }
}

void PneumaticSystem::loop() {
  updateAuxiliary();
  if (testStreaming) sampleTest();
  if (!actuationBusy() && pwmReady()) {
    parkPumps();
    holdUnusedChannelOff();
  }
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
  if (!strcmp(buffer, "/valve/u9/on")) { valvePhysical(1, true); return true; }
  if (!strcmp(buffer, "/valve/u9/off")) { valvePhysical(1, false); return true; }
  if (!strcmp(buffer, "/valve/u10/on")) { valvePhysical(0, true); return true; }
  if (!strcmp(buffer, "/valve/u10/off")) { valvePhysical(0, false); return true; }
  if (!strcmp(buffer, "/valves/pressure")) { valveOn(); return true; }
  if (!strcmp(buffer, "/valves/vacuum")) { valveOff(); return true; }
  if (!strcmp(buffer, "/valves/rest")) { valvesRest(); return true; }
  if (!strcmp(buffer, "/valves/off")) { systemOff(); return true; }
  // Compatibility aliases used by the current RheoData app/bridge. The
  // discoverable device API remains rheo/* so 2P1VX and this build stay aligned.
  if (msg.match("/slip/pump/add*")) {
    const char* firstSpace = strchr(buffer, ' ');
    const char* secondSpace = firstSpace ? strchr(firstSpace + 1, ' ') : nullptr;
    int power = firstSpace ? constrain(getIntArg(msg, 1), 0, 100) : 100;
    int duration = secondSpace ? constrain(getIntArg(msg, 2), 1, 5000) : 1000;
    if (power <= 0) { systemOff(); return true; }
    if (!actuationBusy()) startTimedAir(true, power, duration);
    return true;
  }
  if (msg.match("/slip/pump/remove*")) {
    const char* firstSpace = strchr(buffer, ' ');
    const char* secondSpace = firstSpace ? strchr(firstSpace + 1, ' ') : nullptr;
    int power = firstSpace ? constrain(getIntArg(msg, 1), 0, 100) : 100;
    int duration = secondSpace ? constrain(getIntArg(msg, 2), 1, 5000) : 1000;
    if (power <= 0) { systemOff(); return true; }
    if (!actuationBusy()) startTimedAir(false, power, duration);
    return true;
  }
  if (msg.match("/slip/pump/off")) { systemOff(); return true; }
  if (msg.match("/slip/air/rate*")) {
    if (strchr(buffer, ' ')) rate = constrain(getIntArg(msg, 1), MIN_RATE, MAX_RATE);
    sendSampleOSC("/rheo/sense/rate", (float)rate);
    Serial.print("/rheo/sense/rate="); Serial.println(rate);
    return true;
  }

  if (msg.match("/pump/blow*"))  {
    if (!actuationBusy()) blow(strchr(buffer,' ') ? constrain(getIntArg(msg,1),0,100) : 100);
    return true;
  }
  if (msg.match("/pump/suck*"))  {
    if (!actuationBusy()) suck(strchr(buffer,' ') ? constrain(getIntArg(msg,1),0,100) : -1);
    return true;
  }
  if (msg.match("/pump/off"))    { off();       return true; }
  if (msg.match("/pump/purge"))  {
    if (!actuationBusy()) purge();
    return true;
  }
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
