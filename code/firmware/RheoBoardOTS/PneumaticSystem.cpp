// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include <Wire.h>
#include "PneumaticSystem.h"
#include "OSCHandler.h"
#include "BLEHandler.h"   // pTxCharacteristic (sendApiEntry notifies /rheo/api replies directly)
#include "MemoryStream.h"

SparkFun_MicroPressure mpr;
Adafruit_PWMServoDriver pwm(PWM_DRIVER_ADDR);

static bool pwmReadyFlag = false;

bool pwmReady() { return pwmReadyFlag; }

// setPin(0) writes the PCA9685 always-off bit. setPin(4095) writes the
// always-on bit. A raw off-count of 4095 would leave a one-tick low pulse.
static void pwmWriteFullOff(uint8_t channel) {
  pwm.setPin(channel, 0);
}

static void pwmWriteFullOn(uint8_t channel) {
  pwm.setPin(channel, 4095);
}

static void pwmWritePct(uint8_t channel, int pct) {
  if (pct <= 0) {
    pwmWriteFullOff(channel);
    return;
  }
  if (pct >= 100) {
    pwmWriteFullOn(channel);
    return;
  }
  const uint16_t counts = (uint16_t)(((uint32_t)pct * 4095UL) / 100UL);
  pwm.setPin(channel, counts);
}

// Channel 11 is not wired. Park it full-off so the pin stays low.
static void holdUnusedChannelOff() {
  pwmWriteFullOff(PWM_CH_UNUSED);
}

static void parkPumps() {
  pwmWriteFullOff(PWM_CH_PUMP1);
  pwmWriteFullOff(PWM_CH_PUMP2);
}

static void parkAllEnables() {
  parkPumps();
  pwmWriteFullOff(PWM_CH_VALVE);
  holdUnusedChannelOff();
}

void pumpSetPct(PumpChannel c, int pct) {
  pct = constrain(pct, 0, 100);
  if (!pwmReady()) return;
  const uint8_t channel = (c == PUMP_1) ? PWM_CH_PUMP1 : PWM_CH_PUMP2;
  pwmWritePct(channel, pct);
}

void pumpSet(PumpChannel c, bool on) { pumpSetPct(c, on ? 100 : 0); }

void pumpsOff() {
  if (!pwmReady()) return;
  parkPumps();
}

void pumpsOffNow() {
  pumpsOff();
}

void valveSuck() {
  if (!pwmReady()) return;
  pwmWriteFullOff(PWM_CH_VALVE);
  holdUnusedChannelOff();
}

void valveBlow() {
  if (!pwmReady()) return;
  pwmWriteFullOn(PWM_CH_VALVE);
  holdUnusedChannelOff();
}

bool halBegin() {
  // Thing Plus defaults: SDA GPIO 23, SCL GPIO 22. Header jumpers to the
  // PCA9685 use those pins. The Qwiic connector is the same I2C peripheral.
  Wire.begin();

  pwmReadyFlag = false;
  const bool pwmOk = pwm.begin();
  if (pwmOk) {
    pwm.setPWMFreq(1000);
    pwm.setOutputMode(true);  // totem pole, so a full-on enable is a driven high
    pwmReadyFlag = true;
    parkAllEnables();

    Serial.println("PneumaticSystem: PCA9685 addr 0x40, setPWMFreq(1000), totem-pole.");
    Serial.println("PneumaticSystem: header jumpers SDA GPIO 23 / SCL GPIO 22 (not a Qwiic plug).");
    Serial.println("PneumaticSystem: INA is ENA (OUT1/OUT2), INB is ENB (OUT3/OUT4).");
    Serial.println("PneumaticSystem: ch 8 = RIGHT ENA valve, ch 9 = RIGHT ENB pressure pump.");
    Serial.println("PneumaticSystem: ch 10 = LEFT ENA vacuum pump. LEFT ENB is tied to GND.");
    Serial.println("PneumaticSystem: ch 11 is not connected and is held full-off.");
    Serial.println("PneumaticSystem: pulsing channel 9 (extrude pump, RIGHT L298N ENB).");
    for (int i = 0; i < 3; i++) {
      pwmWriteFullOn(PWM_CH_PUMP2);
      delay(250);
      pwmWriteFullOff(PWM_CH_PUMP2);
      delay(250);
    }
    Serial.println("PneumaticSystem: channel 9 parked full-off.");
    Serial.println("PneumaticSystem: pulsing channel 8 (valve, RIGHT L298N ENA). Meter OUT1-OUT2.");
    for (int i = 0; i < 3; i++) {
      valveBlow();
      delay(250);
      valveSuck();
      delay(250);
    }
  } else {
    Serial.println("PneumaticSystem: PCA9685 not found on I2C addr 0x40 (header SDA GPIO 23 / SCL GPIO 22).");
  }

  if (PIN_SW1  >= 0) pinMode(PIN_SW1, INPUT_PULLUP);
  if (PIN_LED1 >= 0) { pinMode(PIN_LED1, OUTPUT); digitalWrite(PIN_LED1, LOW); }

  const bool mprOk = mpr.begin(MPRLS_I2C_ADDR, Wire);
  if (!mprOk) Serial.println("PneumaticSystem: MPRLS pressure sensor not found on Qwiic bus.");
  return pwmOk && mprOk;
}

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
void PneumaticSystem::stopBlow() { pumpsOffNow(); valveSuck(); manualBlowing = false; }

void PneumaticSystem::suck(int power) {
  // Match blow(0): zero means stop, not a zero-drive latched manual mode.
  if (power == 0) { systemOff(); return; }
  manualBlowing = false;
  pumpSet(PUMP_2, false); valveSuck();
  delay(VALVE_SETTLE_MS);
  pumpSetPct(PUMP_1, power);
  latchedSuck = true;
}
void PneumaticSystem::stopSuck()   { pumpsOffNow(); latchedSuck = false; }
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
void PneumaticSystem::valveOn()  { valveBlow(); }
void PneumaticSystem::valveOff() { valveSuck(); }
void PneumaticSystem::systemOff() {
  purgeActive = false;
  purgeEndsAt = 0;
  timedAirActive = false;
  timedAirStopsAt = 0;
  pumpsOffNow(); valveSuck();
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
  timedAirActive = false;
  timedAirStopsAt = 0;
}

void PneumaticSystem::updateAuxiliary() {
  const unsigned long now = millis();
  if (purgeActive && static_cast<long>(now - purgeEndsAt) >= 0) {
    pumpsOff();
    valveSuck();
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
    Serial.println("PneumaticSystem: HAL init failed (PCA9685 and/or MPRLS not found).");
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
