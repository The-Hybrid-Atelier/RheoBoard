# RheoBoardPCB firmware — original v1

Open `RheoBoardPCB.ino` in Arduino IDE, or build with `bash code/RheoBoardPCB/init.sh` from the repository root. The sketch runs on the **classic SparkFun ESP32 Thing Plus**, connected to the original **RheoBoard_v1** PCB. It advertises as **RheoBoardPCB**. This folder is a dedicated PCB adaptation of the RheoBoardOTS firmware; the OTS sketch remains separate.

This firmware targets [the original v1 design](../../RheoBoard_PCB/RheoBoard_v1/RheoboardV1/). **Do not use it for v1.05**, which has different valve wiring. It retains the OTS REP, triads, bubble routines, programmable routines, BLE command protocol, and USB controls. This configuration uses **both valves: U9 for pressure and U10 for vacuum**, with both off at rest. All four PCB buttons have assigned controls. Pump roles remain configurable because tubing has not yet been assigned.

## Build and upload

Verified build environment:

- Arduino ESP32 core **2.0.11**, board `esp32:esp32:esp32thing_plus`.
- **OSC 1.3.7** and **ThingPlusBLEOSC 1.0.2**. Install the latter from [its source repository](https://github.com/cearto/ThingPlusBLEOSC). Its `BLEHandler.h`, `OSCHandler.h`, and `MemoryStream.h` must be available to Arduino.
- This PCB sketch implements its I2C devices directly; it does not require the Adafruit PWM, SparkFun MicroPressure, Qwiic Button, or programmable_air libraries.

The build uses the installed libraries and does not download or replace them. Core 3.x is not a verified target; the BLE interfaces differ from the tested core.

```sh
# Compile; no hardware is changed.
bash code/RheoBoardPCB/init.sh

# Run host tests with a C++17 compiler and the installed OSC libraries.
bash code/RheoBoardPCB/tests/smoke.sh

# Identify the USB port, then substitute its actual name below.
arduino-cli board list
arduino-cli upload --fqbn esp32:esp32:esp32thing_plus \
  --port YOUR_USB_PORT code/RheoBoardPCB
```

In Arduino IDE, select **SparkFun ESP32 Thing Plus**, the correct USB port, and Serial Monitor **115200 baud**, with newline endings. Compile before uploading. Disconnect the PCB/load power during the first host upload. No automatic pump or valve pulses occur at startup.

## PCB connections and configuration

The following electrical mapping was checked against a freshly exported v1 schematic netlist and the saved PCB pads on 2026-11-02. Pump direction and valve port function still depend on tubing.

| Function | Device and electrical path | Default role |
|---|---|---|
| Pump output 0 | U7 PCA9685 `0x40`, PWM0 → R35 → Q3 → U8 | Pressure / `PUMP2` |
| Pump output 1 | U7 PCA9685 `0x40`, PWM1 → R42 → Q4 → U11 | Vacuum / `PUMP1` |
| Valve output 0 | U4 TCA9534A `0x3F`, P0 → R55 → Q10 → U10 | Vacuum valve |
| Valve output 1 | U4 TCA9534A `0x3F`, P1 → R36 → Q8 → U9 | Pressure valve |
| Buttons | U4 P4, P5, P6, P7 | SW1, SW2, SW3, SW4; active low |
| Pressure | U5 MPRLS0025PA00001A `0x18` | Absolute pressure, pascals |
| Host I2C | Thing Plus SDA GPIO23, SCL GPIO22 | 100 kHz |

PCA address jumpers must remain open for `0x40`; the saved TCA address inputs are pulled high for `0x3F`. Pump PWM is nominally 1017 Hz. Valves use steady on/off drive. PWM2–PWM15 remain off, including the expansion outputs.

Edit `HardwareProfile.h` to change `RHEO_V1_VACUUM_CHANNEL` (0/U8 or 1/U11); pressure uses the other pump. Valve states are separate masks, with bit 0 controlling U10 and bit 1 controlling U9:

| Setting | Default | U9 | U10 |
|---|---|---|---|
| `RHEO_V1_PRESSURE_VALVES` | `2` | On | Off |
| `RHEO_V1_VACUUM_VALVES` | `1` | Off | On |
| `RHEO_V1_REST_VALVES` | `0` | Off | Off |

Each mask can be 0 (both off), 1 (U10 on), 2 (U9 on), or 3 (both on), supporting other tubing arrangements. These are **coil states**, not claims about which pneumatic ports are open. Confirm the actual pressure/vacuum paths and the three-way valves' released paths before connecting a sample. The old `RHEO_V1_VALVE_PORT` setting is replaced by these masks and produces a build error if still supplied.

Baseline, relaxation, gaps between pressure pulses, and normal routine completion use the rest mask. Pressure and vacuum stages use their respective masks. Startup, STOP, manual-button release, BLE disconnection, cancellation, and faults always request **both valves off**, regardless of the rest mask. Thus a nonzero rest mask is never allowed to override a stop. There is no software promise that de-energizing the valves vents trapped pressure.

`RHEO_V1_SENSOR_RESET_PIN=-1` means no extra reset wire. Only set it to a spare GPIO after physically wiring that pin to TP3/RES and reviewing the connection.

**Host power connection:** the original v1 Qwiic sockets connect their 3.3 V pin to the PCB regulator output. A standard four-wire cable to a USB-powered Thing Plus therefore ties two regulator outputs together. Use a verified power arrangement; the existing v1 bench arrangement omits the host-to-PCB 3.3 V conductor and connects GND, SDA and SCL. This does not eliminate signal-line backfeeding when one side is off. Do not hot-plug. J3's electrical order is **1 GND, 2 +3V3, 3 SDA, 4 SCL**; its printed SDA/SCL labels are reversed. The sensor's stock reset is only pulled high, so cold-start reliability must be tested physically.

## Controls

| PCB button | Action |
|---|---|
| SW1 | Press to start one REP when idle |
| SW2 | STOP: cancel work and release all outputs |
| SW3 | Hold for vacuum; release to stop |
| SW4 | Hold for pressure; release to stop |

Starts require a stable press for 25 ms. STOP and manual release take effect on the next button poll. Held-at-boot buttons cannot start work until released. SW1 does not repeat while held. SW3/SW4 use the configured REP vacuum/pressure power settings (100% defaults). Pressing both direction buttons stops work; release both before starting again.

While SW2 is held, BLE/serial commands are discarded. A stop invalidates held buttons so releasing STOP cannot restart a pump. Start buttons are ignored during routines, triad gaps, pump tests, purges, and timed-air commands; release after the operation finishes before pressing again. The ESP32's GPIO0 button retains the OTS behavior: start REP when idle, stop active work.

USB commands are case sensitive:

| Command | Action |
|---|---|
| `REP` | Start one REP when idle |
| `STOP` | Cancel every routine and release both pumps and valves |
| `PUMP1 0` … `PUMP1 100` | Vacuum pump bench test, with pressure streaming |
| `PUMP2 0` … `PUMP2 100` | Pressure pump bench test, with pressure streaming |
| `VALVES PRESS` / `VALVES VAC` / `VALVES REST` | Select the configured state without running a pump; idle only |
| `U9 ON` / `U9 OFF` | Power/release U9 while preserving U10; idle only |
| `U10 ON` / `U10 OFF` | Power/release U10 while preserving U9; idle only |
| `VALVES OFF` | Cancel work and release all outputs, like `STOP` |
| `VALVE ON` / `VALVE OFF` | Legacy aliases for pressure / vacuum state; idle only |
| `BUTTONS` | Report `#BUTTONS,<binary mask>`; bit 0 is SW1 |
| `PRESSURE` | Read absolute pressure without starting a pump: `#PRESSURE,<Pa>` |

`PUMP1`/`PUMP2` tests run until stopped. Use `STOP` to cancel active work before starting another test. Bench samples are `#S,<elapsed_ms>,<absolute_Pa>`. REP reports buffered phase markers and a sample count after completion. A normal REP selects the configured rest state during relaxation and completion. This replaces the one-valve firmware’s retained pressure state; the recording messages and sample grid stay the same.


BLE valve commands are `/valves/pressure`, `/valves/vacuum`, `/valves/rest`, `/valves/off`, and `/valve/u9/on|off`, `/valve/u10/on|off`. Individual ON/OFF controls change one physical output without disturbing the other. State selection and individual controls are ignored during running work; `/valves/off` has stop priority even with a full queue. For compatibility, `/valve/on` selects pressure and `/valve/off` selects vacuum; **`/valve/off` does not mean both coils off**. Use `/valves/off` or `/system/off` for that.


BLE service UUID: `71c978dc-2d05-4a35-b56f-12f4fee4ee31`. RX: `43d4940a-3037-4040-9c3d-6ecd138325ca`. TX: `78d88040-6306-40bd-b47d-e1feb75d6482`.

The inherited ThingPlus transport receives **text**, such as `/pump/blow 30` or `/rheo/rep`, on RX, and sends binary OSC notifications on TX. RX is not a binary OSC packet decoder. `/rheo/api` lists the command interface. BLE writes must fit within 127 bytes. A stop bypasses a full command queue and discards older commands. Disconnect cancels work before pending commands are processed. The device name changes to RheoBoardPCB; host applications that filter by the old name will need to accept this name.

Commands retained include `/pump/blow`, `/pump/suck`, `/pump/off`, `/pump/purge`, `/valve/on`, `/valve/off`, `/system/off`, `/rheo/rep`, `/rheo/rep/triad`, `/rheo/bubble`, `/rheo/bubble/triad`, `/rheo/stop`, `/rheo/purge`, `/rheo/sense/rate`, `/rheo/program/*`, and the existing `/slip/*` compatibility aliases. REP calibration remains available under `/rheo/rep/pull/*`, `/rheo/rep/push/*`, `/rheo/rep/baseline/time`, and `/rheo/rep/interval`; `/rheo/api` lists their arguments. Calibration and stored routines are RAM-only and reset on reboot.

Default REP timing is 250 ms baseline, 250 ms vacuum at 100%, then 250 ms pressure at 100%, followed by relaxation within a 1200 ms recording window. Its 66 pressure samples use the inherited 18 ms grid. Delayed conversions can fill multiple slots with the last reading; this is not a guarantee of 66 independent conversions. `/db/start`, `/rheo/sense/air`, and `/db/save` preserve the recording protocol.

## Fault handling and bench acceptance

Startup clears both expanders, configures the valve latches before their output directions, checks register readback, and requires a valid pressure conversion. Bus or sensor faults latch actuation disabled until restart and attempt to shut off both chips. Sensor busy polling has a 30 ms deadline; individual I2C operations have a 25 ms timeout, so total response time can be longer. A broken I2C bus or stalled host can leave outputs energized: **removing the 12 V load supply is the independent stop**. Firmware cannot add a hardware watchdog or repair the sensor supply-ramp/reset requirement.

Software verification is complete only for the documented configuration and test environment. No assembled-board operation is claimed. Before a loaded test, a person must:

- Check unpowered polarity, shorts, assembly, and the host connection, then measure unloaded 3.3 V, 4.5 V and 6 V rails using appropriate current limiting.
- Confirm boot leaves all loads off, including an ESP32-only restart, and pressure readings match ambient absolute pressure through repeated cold starts.
- Confirm `BUTTONS` reports each physical switch and the chosen pump/valve roles match tubing. SW1, SW3 and SW4 can actuate; keep loads disconnected for button-only checks.
- Test each pump at low duty and each valve separately, then verify `STOP`, BLE disconnect, and one REP before increasing load.
- Validate actual pressure traces, timing, supply behavior, and host application compatibility. Keep physical results separate from simulated test results.

Register semantics were checked against the [NXP PCA9685 datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf), [TI TCA9534A datasheet](https://www.ti.com/lit/ds/symlink/tca9534a.pdf), and [Honeywell MPR datasheet](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf).

Firmware is MIT licensed; see [LICENSE](../../LICENSE).
