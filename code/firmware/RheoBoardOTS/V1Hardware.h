// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#pragma once
#include "HardwareProfile.h"
#if RHEO_HARDWARE_PROFILE == RHEO_PROFILE_PCB_V1
#include <stdint.h>

namespace v1Hardware {
bool begin();
bool ready();
void pump(uint8_t channel, int percent);
void pumpsOff();
void valve(bool on);
// Bits 0..3 are SW1..SW4, 1 = pressed. False means an I2C fault.
bool buttons(uint8_t &pressed);
float pressurePa();
}
#endif
