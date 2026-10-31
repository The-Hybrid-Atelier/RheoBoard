// SPDX-License-Identifier: MIT
#include <Arduino.h>
#include <Wire.h>
#include "V1Hardware.h"
#include <cmath>
#include <iostream>

void resetModel() { Wire = FakeWire{}; fakeMillis = 0; pinEvents.clear(); }
void outputsOff() {
  for (int channel = 0; channel < 16; ++channel) assert(Wire.pumpOff(channel));
  assert(Wire.tca[1] == 0);
}
void noRearm() {
  assert(!v1Hardware::ready());
  const auto count = Wire.transactions.size();
  v1Hardware::pump(0, 100); v1Hardware::pump(1, 100); v1Hardware::valve(true);
  uint8_t buttons = 0xFF;
  assert(!v1Hardware::buttons(buttons) && buttons == 0);
  assert(std::isnan(v1Hardware::pressurePa()));
  assert(Wire.transactions.size() == count);
}
int main() {
  resetModel();
  assert(v1Hardware::begin());
  const auto startup = Wire.transactions;
  assert(Wire.frequency == 100000 && Wire.timeout == 25);
  assert(Wire.tca[3] == 0xFC && Wire.tca[2] == 0);
  assert(Wire.pca[0] == 0x20 && Wire.pca[1] == 4 && Wire.pca[0xFE] == 5);
  outputsOff();
  assert(std::abs(v1Hardware::pressurePa() - 86184.46f) < 0.1f);
  for (auto sample : {std::pair<uint32_t, float>{1677722, 0}, {15099494, 172368.93f}}) {
    Wire.sensorRaw = sample.first;
    assert(std::abs(v1Hardware::pressurePa() - sample.second) < 0.1f);
  }
  // Real ALL_LED writes load channel registers. A subsequent channel write
  // must turn one output on without accidentally re-enabling other outputs.
  v1Hardware::pump(0, 50);
  assert(!Wire.pumpOff(0) && Wire.pumpOff(1));
  assert(Wire.pca[8] == 0xFF && Wire.pca[9] == 7);
  v1Hardware::pumpsOff(); outputsOff();
  auto start = Wire.transactions.size();
  v1Hardware::pump(1, 150);
  assert(Wire.transactions[start].bytes == (std::vector<uint8_t>{6, 0, 0, 0, 0x10}));
  assert(Wire.pumpOff(0) && Wire.pca[11] == 0x10 && Wire.pca[13] == 0);
  for (int ch = 2; ch < 16; ++ch) assert(Wire.pumpOff(ch));
  v1Hardware::pump(1, -10); outputsOff();
  v1Hardware::valve(true); assert(Wire.tca[1] == (1 << RHEO_V1_VALVE_PORT));
  v1Hardware::valve(false); outputsOff();
  for (int mask = 0; mask < 16; ++mask) {
    Wire.tca[0] = static_cast<uint8_t>(~(mask << 4));
    uint8_t buttons = 0;
    assert(v1Hardware::buttons(buttons) && buttons == mask);
  }
  // An ESP32-only reboot must first clear stale outputs on both expanders.
  Wire.pca[0] = 0xA0;
  Wire.pca[7] = 0x10; Wire.pca[9] = 0;
  Wire.tca[1] = 0xFF; Wire.tca[3] = 0xFC;
  start = Wire.transactions.size();
  assert(v1Hardware::begin()); outputsOff();
  assert(Wire.transactions[start].bytes == (std::vector<uint8_t>{0xFD, 0x10}));
  assert(Wire.transactions[start + 1].bytes == (std::vector<uint8_t>{1, 0}));
  // Inject NACK/short-read at every startup transaction, including readback
  // and sensor conversion. Each case must refuse all subsequent actuation.
  for (size_t transaction = 1; transaction <= startup.size(); ++transaction) {
    resetModel(); Wire.failAt = transaction;
    assert(!v1Hardware::begin()); outputsOff(); noRearm();
  }
  // Corrupt every expander readback independently; do not accept ACK alone.
  int corruptChecks = 0;
  bool initialModeRead = true;
  for (size_t i = 0; i < startup.size(); ++i) {
    if (!startup[i].read || startup[i].address == 0x18) continue;
    // Initial MODE1 is not an equality check; EXTCLK is tested separately.
    if (startup[i].address == 0x40 && startup[i - 1].bytes[0] == 0 && initialModeRead) {
      initialModeRead = false;
      continue;
    }
    resetModel(); Wire.corruptAt = i + 1;
    assert(!v1Hardware::begin()); outputsOff(); noRearm(); ++corruptChecks;
  }
  resetModel(); Wire.pca[0] |= 0x40;
  assert(!v1Hardware::begin()); noRearm();
  resetModel(); Wire.failBuffer = true;
  assert(!v1Hardware::begin()); noRearm();
  // A partial multi-byte read is not valid sensor or register data.
  for (size_t i = 0; i < startup.size(); ++i) {
    if (!startup[i].read || startup[i].bytes[0] < 2) continue;
    resetModel(); Wire.shortAt = i + 1;
    assert(!v1Hardware::begin()); outputsOff(); noRearm();
  }
  for (uint8_t status : {0xFF, 0x44, 0x41, 0x00, 0x60}) {
    resetModel(); assert(v1Hardware::begin());
    v1Hardware::pump(0, 100); v1Hardware::valve(true);
    Wire.sensorStatus = status;
    assert(std::isnan(v1Hardware::pressurePa())); outputsOff(); noRearm();
  }
  resetModel(); assert(v1Hardware::begin());
  Wire.busyPolls = 3; assert(std::isfinite(v1Hardware::pressurePa()));
  Wire.busyPolls = -1; const auto before = millis();
  assert(std::isnan(v1Hardware::pressurePa()));
  assert(millis() - before <= 31); outputsOff(); noRearm();
  // Failure turning the opposite pump off must prevent the new ON command.
  resetModel(); assert(v1Hardware::begin()); v1Hardware::pump(0, 100);
  Wire.failAt = Wire.transactionCount + 1; start = Wire.transactions.size();
  v1Hardware::pump(1, 100);
  assert(Wire.transactions.size() == start + 3); outputsOff(); noRearm();
  resetModel(); assert(v1Hardware::begin());
  Wire.failAt = Wire.transactionCount + 1; v1Hardware::valve(true);
  outputsOff(); noRearm();
  resetModel(); assert(v1Hardware::begin());
  Wire.failAt = Wire.transactionCount + 2; uint8_t buttons;
  assert(!v1Hardware::buttons(buttons)); outputsOff(); noRearm();
  // A disconnected bus CANNOT turn off retained expander outputs. The
  // application must latch a fault, and bus recovery must not silently rearm.
  resetModel(); assert(v1Hardware::begin()); v1Hardware::pump(1, 100);
  Wire.disconnected = true; v1Hardware::pumpsOff();
  assert(!Wire.pumpOff(1)); noRearm(); Wire.disconnected = false; noRearm();
  resetModel(); assert(v1Hardware::begin()); v1Hardware::pump(2, 100);
  outputsOff(); noRearm();
#if RHEO_V1_SENSOR_RESET_PIN >= 0
  assert(pinEvents.size() == 4);
  assert(pinEvents[0].value == HIGH && !pinEvents[0].mode);
  assert(pinEvents[1].mode && pinEvents[1].value == OUTPUT_OPEN_DRAIN);
  assert(pinEvents[2].value == LOW && pinEvents[3].value == HIGH);
  for (const auto &event : pinEvents) assert(event.pin == RHEO_V1_SENSOR_RESET_PIN);
#endif
  std::cout << "PASS: v1 outputs, transfer function, buttons, restart, " << startup.size()
            << " startup failures, " << corruptChecks << " corrupt readbacks, sensor/bus faults\n";
}
