// SPDX-License-Identifier: MIT
// Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE
#pragma once

#define RHEO_PROFILE_DIY 0
#define RHEO_PROFILE_PCB_V1 1

// Select here or with a compiler -D flag. This profile is for the older v1,
// NOT v1.05. DIY remains the default for the existing assembled DIY systems.
#ifndef RHEO_HARDWARE_PROFILE
#define RHEO_HARDWARE_PROFILE RHEO_PROFILE_DIY
#endif
#if RHEO_HARDWARE_PROFILE != RHEO_PROFILE_DIY && RHEO_HARDWARE_PROFILE != RHEO_PROFILE_PCB_V1
#error "Unknown RheoBoard hardware profile"
#endif

// Match tubing to these choices before a loaded test: channel 0 = U8,
// channel 1 = U11. The pressure pump uses the other channel.
#ifndef RHEO_V1_VACUUM_CHANNEL
#define RHEO_V1_VACUUM_CHANNEL 1
#endif
// TCA P0 = U10; P1 = U9. The unused valve is always held OFF.
#ifndef RHEO_V1_VALVE_PORT
#define RHEO_V1_VALVE_PORT 1
#endif
// Optional physical rework: spare ESP32 GPIO wired to TP3 (sensor RES).
// -1 means no reset wire; software alone cannot fix the stock slow-ramp risk.
#ifndef RHEO_V1_SENSOR_RESET_PIN
#define RHEO_V1_SENSOR_RESET_PIN -1
#endif
#if RHEO_V1_VACUUM_CHANNEL < 0 || RHEO_V1_VACUUM_CHANNEL > 1 || RHEO_V1_VALVE_PORT < 0 || RHEO_V1_VALVE_PORT > 1
#error "v1 pump channel and valve port must each be 0 or 1"
#endif
