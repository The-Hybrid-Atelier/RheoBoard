// SPDX-License-Identifier: MIT
#pragma once
// Original RheoBoard_v1 PCB only. v1.05 has different valve wiring.
#if defined(RHEO_HARDWARE_PROFILE) && RHEO_HARDWARE_PROFILE != 1
#error "RheoBoardPCB targets the original v1 PCB only"
#endif
#define RHEO_PROFILE_PCB_V1 1
#ifndef RHEO_HARDWARE_PROFILE
#define RHEO_HARDWARE_PROFILE RHEO_PROFILE_PCB_V1
#endif
// Physical channels: PCA0 = U8; PCA1 = U11. Pressure uses the other channel.
#ifndef RHEO_V1_VACUUM_CHANNEL
#define RHEO_V1_VACUUM_CHANNEL 1
#endif
// Valve-state masks: bit 0 = U10 (TCA P0), bit 1 = U9 (P1).
// 0: both off; 1: U10 on; 2: U9 on; 3: both on.
// Defaults assume separate vacuum/pressure branches. Match the actual tubing.
#ifndef RHEO_V1_PRESSURE_VALVES
#define RHEO_V1_PRESSURE_VALVES 2
#endif
#ifndef RHEO_V1_VACUUM_VALVES
#define RHEO_V1_VACUUM_VALVES 1
#endif
#ifndef RHEO_V1_REST_VALVES
#define RHEO_V1_REST_VALVES 0
#endif
#ifdef RHEO_V1_VALVE_PORT
#error "Use PRESSURE_VALVES, VACUUM_VALVES and REST_VALVES masks instead of VALVE_PORT"
#endif
#if RHEO_V1_PRESSURE_VALVES < 0 || RHEO_V1_PRESSURE_VALVES > 3 || RHEO_V1_VACUUM_VALVES < 0 || RHEO_V1_VACUUM_VALVES > 3 || RHEO_V1_REST_VALVES < 0 || RHEO_V1_REST_VALVES > 3
#error "Valve masks must be 0..3"
#endif
// Optional physical GPIO-to-TP3 reset wire; disabled on the stock PCB.
#ifndef RHEO_V1_SENSOR_RESET_PIN
#define RHEO_V1_SENSOR_RESET_PIN -1
#endif
#if RHEO_V1_VACUUM_CHANNEL < 0 || RHEO_V1_VACUUM_CHANNEL > 1
#error "Vacuum pump channel must be 0 or 1"
#endif
