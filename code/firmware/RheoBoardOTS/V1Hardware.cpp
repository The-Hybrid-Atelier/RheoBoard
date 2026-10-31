// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#include "V1Hardware.h"
#if RHEO_HARDWARE_PROFILE == RHEO_PROFILE_PCB_V1
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

namespace {
constexpr uint8_t PCA = 0x40, TCA = 0x3F, MPR = 0x18;
constexpr uint8_t MODE1 = 0x00, MODE2 = 0x01, LED0 = 0x06;
constexpr uint8_t ALL_OFF_HIGH = 0xFD, PRESCALE = 0xFE;
constexpr uint8_t TCA_INPUT = 0, TCA_OUTPUT = 1, TCA_POLARITY = 2, TCA_CONFIG = 3;
constexpr uint8_t FULL = 0x10;
bool readyFlag = false;

bool writeBytes(uint8_t address, uint8_t reg, const uint8_t *data, uint8_t length) {
  Wire.beginTransmission(address);
  bool buffered = Wire.write(reg) == 1;
  for (uint8_t i = 0; i < length; ++i) buffered = (Wire.write(data[i]) == 1) && buffered;
  const uint8_t result = Wire.endTransmission();
  return buffered && result == 0;
}
bool writeReg(uint8_t address, uint8_t reg, uint8_t value) {
  return writeBytes(address, reg, &value, 1);
}
bool readBytes(uint8_t address, uint8_t reg, uint8_t *data, uint8_t length) {
  Wire.beginTransmission(address);
  if (Wire.write(reg) != 1 || Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(address, length) != length) return false;
  for (uint8_t i = 0; i < length; ++i) {
    if (!Wire.available()) return false;
    data[i] = static_cast<uint8_t>(Wire.read());
  }
  return true;
}
bool matches(uint8_t address, uint8_t reg, uint8_t expected) {
  uint8_t value = 0;
  return readBytes(address, reg, &value, 1) && value == expected;
}
// ALL_LED registers load each channel register; they are not a separate
// output-enable latch. Individual channel writes can subsequently re-enable it.
bool parkBothChips() {
  const bool pumps = writeReg(PCA, ALL_OFF_HIGH, FULL);
  const bool valves = writeReg(TCA, TCA_OUTPUT, 0);
  return pumps && valves; // Always attempt both, even if one device is absent.
}
void fault() {
  const bool wasReady = readyFlag;
  readyFlag = false;
  const bool stopped = parkBothChips();
  if (wasReady || !stopped) {
    Serial.println("v1 hardware fault: actuation disabled until restart. Remove 12 V if outputs remain on; I2C cannot guarantee shutdown.");
  }
}
bool writePump(uint8_t channel, int percent) {
  const uint16_t count = percent <= 0 || percent >= 100 ? 0 :
      static_cast<uint16_t>((static_cast<uint32_t>(percent) * 4095U) / 100U);
  const uint8_t values[4] = {0, static_cast<uint8_t>(percent >= 100 ? FULL : 0),
      static_cast<uint8_t>(count & 0xFF),
      static_cast<uint8_t>(percent <= 0 ? FULL : percent >= 100 ? 0 : count >> 8)};
  return writeBytes(PCA, LED0 + 4 * channel, values, 4);
}

float measurePressure() {
  // Honeywell MPR command AA 00 00. Check ACK and every read; the stock
  // SparkFun reader has an unbounded busy loop, unsuitable during actuation.
  const uint8_t command[2] = {0, 0};
  if (!writeBytes(MPR, 0xAA, command, 2)) return NAN;
  const unsigned long started = millis();
  uint8_t status = 0x20;
  do {
    if (millis() - started >= 30UL) return NAN;
    delay(1);
    if (Wire.requestFrom(MPR, static_cast<uint8_t>(1)) != 1 || !Wire.available()) return NAN;
    status = static_cast<uint8_t>(Wire.read());
    if (status == 0xFF || (status & 0x05)) return NAN; // Integrity or math fault.
  } while (status & 0x20);
  if (Wire.requestFrom(MPR, static_cast<uint8_t>(4)) != 4 || Wire.available() < 4) return NAN;
  status = static_cast<uint8_t>(Wire.read());
  if ((status & 0x25) || !(status & 0x40)) return NAN;
  uint32_t raw = 0;
  for (uint8_t i = 0; i < 3; ++i) raw = (raw << 8) | static_cast<uint8_t>(Wire.read());
  // MPRLS0025PA00001A: absolute 0..25 psi, transfer function A (10..90%).
  return (static_cast<float>(raw) - 1677722.0f) * (25.0f * 6894.757f) / 13421772.0f;
}
}

namespace v1Hardware {
bool ready() { return readyFlag; }

bool begin() {
  readyFlag = false;
  Wire.begin(); // SparkFun classic ESP32 Thing Plus: SDA23, SCL22.
  Wire.setClock(100000);
  Wire.setTimeOut(25);
  if (!parkBothChips()) { fault(); return false; }
  // Latch LOW before P0/P1 become outputs, including an ESP32-only restart.
  if (!writeReg(TCA, TCA_POLARITY, 0) || !writeReg(TCA, TCA_CONFIG, 0xFC) ||
      !matches(TCA, TCA_OUTPUT, 0) || !matches(TCA, TCA_CONFIG, 0xFC) ||
      !matches(TCA, TCA_POLARITY, 0)) {
    fault(); return false;
  }
  uint8_t mode = 0;
  if (!readBytes(PCA, MODE1, &mode, 1) || (mode & 0x40)) {
    // EXTCLK is sticky until power-on/software reset. Never enable a stage
    // with the wrong clock source; require a board power cycle instead.
    fault(); return false;
  }
  if (!writeReg(PCA, MODE1, 0x30) || !writeReg(PCA, MODE2, 0x04) ||
      !writeReg(PCA, PRESCALE, 5)) { fault(); return false; }
  // AI enabled, sleeping, non-inverted push-pull. Initialize every output OFF.
  const uint8_t off[4] = {0, 0, 0, FULL};
  if (!writeBytes(PCA, 0xFA, off, 4) || !writeReg(PCA, MODE1, 0x20)) {
    fault(); return false;
  }
  delay(1); // Internal oscillator startup: datasheet maximum 500 us.
  if (!matches(PCA, MODE1, 0x20) || !matches(PCA, MODE2, 0x04) ||
      !matches(PCA, PRESCALE, 5)) { fault(); return false; }
  for (uint8_t channel = 0; channel < 16; ++channel) {
    uint8_t values[4];
    if (!readBytes(PCA, LED0 + channel * 4, values, 4) ||
        values[0] != 0 || values[1] != 0 || values[2] != 0 || values[3] != FULL) {
      fault(); return false;
    }
  }
#if RHEO_V1_SENSOR_RESET_PIN >= 0
  digitalWrite(RHEO_V1_SENSOR_RESET_PIN, HIGH);
  pinMode(RHEO_V1_SENSOR_RESET_PIN, OUTPUT_OPEN_DRAIN);
  digitalWrite(RHEO_V1_SENSOR_RESET_PIN, LOW);
  delay(5);
  digitalWrite(RHEO_V1_SENSOR_RESET_PIN, HIGH); // Release to board pull-up.
#endif
  delay(5);
  if (!isfinite(measurePressure())) { fault(); return false; }
  readyFlag = true;
  Serial.println("v1 hardware ready: PWM0=U8, PWM1=U11; TCA P0=U10, P1=U9. All outputs OFF, no startup pulses.");
  return true;
}
void pump(uint8_t channel, int percent) {
  if (!readyFlag) return;
  if (channel > 1) { fault(); return; }
  percent = constrain(percent, 0, 100);
  // Stop the opposite pump before energizing this one.
  if ((percent > 0 && !writePump(1 - channel, 0)) || !writePump(channel, percent)) fault();
}
void pumpsOff() {
  if (readyFlag && !writeReg(PCA, ALL_OFF_HIGH, FULL)) fault();
}
void valve(bool on) {
  if (readyFlag && !writeReg(TCA, TCA_OUTPUT, on ? (1U << RHEO_V1_VALVE_PORT) : 0)) fault();
}
bool buttons(uint8_t &pressed) {
  pressed = 0;
  if (!readyFlag) return false;
  uint8_t input = 0;
  if (!readBytes(TCA, TCA_INPUT, &input, 1)) { fault(); return false; }
  pressed = static_cast<uint8_t>((static_cast<uint8_t>(~input) >> 4) & 0x0F);
  return true;
}
float pressurePa() {
  if (!readyFlag) return NAN;
  const float pressure = measurePressure();
  if (!isfinite(pressure)) fault();
  return pressure;
}
}
#endif
