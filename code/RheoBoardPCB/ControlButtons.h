// SPDX-License-Identifier: MIT
#pragma once
#include <stdint.h>

// Active-high pressed mask, bits 0..3 = SW1..SW4. A button must be
// observed released after startup or inhibit() before it can start work.
class ControlButtons {
 public:
  void inhibit() { blocked_ = 0x0F; pressed_ = 0; }
  void update(uint8_t raw, uint32_t now) {
    pressed_ = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      const uint8_t bit = 1U << i;
      if ((raw ^ candidate_) & bit) {
        candidate_ ^= bit;
        changedAt_[i] = now;
      }
      if (((candidate_ ^ stable_) & bit) &&
          static_cast<uint32_t>(now - changedAt_[i]) >= 25U) {
        stable_ ^= bit;
        if ((stable_ & bit) && !(blocked_ & bit)) pressed_ |= bit;
      }
      if (!((raw | stable_) & bit)) blocked_ &= static_cast<uint8_t>(~bit);
    }
  }
  uint8_t pressed() const { return pressed_; }
  uint8_t stable() const { return stable_; }

 private:
  uint8_t candidate_ = 0, stable_ = 0, blocked_ = 0x0F, pressed_ = 0;
  uint32_t changedAt_[4] = {};
};
