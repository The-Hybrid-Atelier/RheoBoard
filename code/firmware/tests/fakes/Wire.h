// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <vector>

// Register-level model based on NXP PCA9685, TI TCA9534A and Honeywell MPR
// datasheets. It does not model analog behavior, actual wiring or bus timing.
struct Transaction {
  uint8_t address;
  std::vector<uint8_t> bytes;
  bool read, failed;
};
struct FakeWire {
  std::array<uint8_t, 256> pca{};
  std::array<uint8_t, 4> tca{{0xFF, 0xFF, 0, 0xFF}};
  std::array<uint8_t, 128> pointer{};
  std::vector<Transaction> transactions;
  std::vector<uint8_t> tx;
  std::deque<uint8_t> rx;
  uint8_t address = 0, sensorStatus = 0x40;
  uint32_t sensorRaw = 0x800000;
  int transactionCount = 0, failAt = -1, corruptAt = -1, shortAt = -1, busyPolls = 0;
  bool disconnected = false, failBuffer = false;
  unsigned long frequency = 0;
  unsigned timeout = 0;
  FakeWire() { pca[0] = 0x11; pca[1] = 0x04; pca[0xFE] = 0x1E; }
  void begin() {}
  void setClock(unsigned long hz) { frequency = hz; }
  void setTimeOut(unsigned ms) { timeout = ms; }
  void beginTransmission(uint8_t addr) { address = addr; tx.clear(); }
  size_t write(uint8_t value) { if (failBuffer) return 0; tx.push_back(value); return 1; }
  bool fail() { return ++transactionCount == failAt || disconnected; }
  uint8_t endTransmission(bool = true) {
    const bool failed = fail();
    transactions.push_back({address, tx, false, failed});
    if (failed) return 2;
    if (tx.empty()) return 0;
    if (address == 0x18) {
      assert((tx == std::vector<uint8_t>{0xAA, 0, 0}));
      return 0;
    }
    assert(address == 0x40 || address == 0x3F);
    pointer[address] = tx[0];
    if (tx.size() == 1) return 0;
    uint8_t reg = tx[0];
    for (size_t i = 1; i < tx.size(); ++i) {
      if (address == 0x3F) { tca[reg & 3] = tx[i]; ++reg; continue; }
      const uint8_t value = tx[i];
      const bool ai = pca[0] & 0x20;
      if (reg == 0) {
        // RESTART is W1C; EXTCLK is sticky until power cycle/reset.
        const uint8_t restart = (value & 0x80) ? 0 : (pca[0] & 0x80);
        pca[0] = (value & 0x7F) | (pca[0] & 0x40) | restart;
      } else if (reg == 0xFE) {
        assert(pca[0] & 0x10); // Prescaler may only change while sleeping.
        pca[reg] = value;
      } else if (reg >= 0xFA && reg <= 0xFD) {
        for (int ch = 0; ch < 16; ++ch) pca[6 + ch * 4 + reg - 0xFA] = value;
        if (reg == 0xFD && (value & 0x10)) pca[0] &= ~0x80;
      } else {
        pca[reg] = value;
        if (reg >= 6 && reg <= 0x45 && !(pca[1] & 0x08)) pca[0] &= ~0x80;
      }
      if (ai) ++reg;
    }
    return 0;
  }
  uint8_t requestFrom(uint8_t addr, uint8_t length) {
    rx.clear();
    const bool failed = fail();
    transactions.push_back({addr, {length}, true, failed});
    if (failed) return 0;
    if (addr == 0x18) {
      assert(length == 1 || length == 4);
      if (length == 1 && busyPolls != 0) {
        rx.push_back(sensorStatus | 0x20);
        if (busyPolls > 0) --busyPolls;
      } else {
        rx.push_back(sensorStatus);
        if (length == 4) {
          rx.push_back(sensorRaw >> 16); rx.push_back(sensorRaw >> 8); rx.push_back(sensorRaw);
        }
      }
    } else {
      for (int i = 0; i < length; ++i) {
        uint8_t reg = pointer[addr];
        rx.push_back(addr == 0x40 ? pca[reg] : tca[reg & 3]);
        if (addr == 0x3F || (pca[0] & 0x20)) ++pointer[addr];
      }
    }
    if (transactionCount == corruptAt) rx[0] ^= 0x01;
    if (transactionCount == shortAt && !rx.empty()) rx.pop_back();
    return rx.size();
  }
  int available() { return rx.size(); }
  int read() { assert(!rx.empty()); const auto value = rx.front(); rx.pop_front(); return value; }
  bool pumpOff(int channel) const { return pca[6 + channel * 4 + 3] & 0x10; }
};
inline FakeWire Wire;
