// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE
#pragma once
#include "HardwareProfile.h"
#include <stdint.h>

namespace v1Hardware {
bool begin();
bool ready();
void pump(uint8_t channel, int percent);
void pumpsOff();
// Physical valve outputs: bit/port 0 = U10 (P0), 1 = U9 (P1).
// Write both atomically, or change one while preserving the other.
void valves(uint8_t mask);
void valvePort(uint8_t port, bool on);
// Bits 0..3 are SW1..SW4, 1 = pressed. False means an I2C fault.
bool buttons(uint8_t &pressed);
float pressurePa();
}
