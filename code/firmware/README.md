# Firmware

**Entry point:** `RheoBoardOTS/RheoBoardOTS.ino`. Arduino requires the sketch folder name to match the `.ino` file. The sketch file and BLE device name are `RheoBoardOTS`.

This sketch is the RheoBoardOTS DIY controller. It is not claimed ready for the v1.05 PCB. That board's PCA9685 uses PWM0–PWM4 for pumps and PWM5–PWM6 for valves. PWM8–PWM10 on that board are unconnected.

Install steps are on the [Firmware](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Firmware) wiki page. RheoData notes are in `../software/README.md`. The download, pairing steps, the connected indicator, and how to start and name a REP are not written yet. That gap stays in the software notes.

Libraries: Adafruit PWM Servo Driver and Adafruit BusIO, SparkFun MicroPressure, SparkFun Qwiic Button, and OSC. ThingPlusBLEOSC supplies `BLEHandler.h`, `OSCHandler.h`, and `MemoryStream.h`.

Firmware is MIT-licensed. The license notice and full text are in [`LICENSE`](../../LICENSE).

## I2C

`Wire.begin()` uses the SparkFun ESP32 Thing Plus defaults: SDA GPIO 23, SCL GPIO 22. The Qwiic connector is that same bus.

| Board | Address | Connection |
|---|---|---|
| Qwiic Button | `0x6F` | Qwiic cable |
| MicroPressure (MPRLS) | `0x18` | Qwiic cable |
| Adafruit PCA9685, product 815 | `0x40` | Four header jumpers. No address jumpers. No Qwiic socket. |

PCA9685 VCC is ESP32 3V3. V+ and OE stay open. Product 815 has 220 ohm resistors in series with its outputs.

## Channel map

These constants are in `RheoBoardOTS/PneumaticSystem.h`. `setPWMFreq(1000)` sets 1000 Hz on every channel.

| Channel | Drives | Load |
|---|---|---|
| `8` | RIGHT L298N `ENA` | Valve, `OUT1`/`OUT2`, full-on / full-off |
| `9` | RIGHT L298N `ENB` | PUMP2, `OUT3`/`OUT4`, PWM |
| `10` | LEFT L298N `ENA` | PUMP1, `OUT1`/`OUT2`, PWM |
| `11` | nothing | Not wired. Parked full-off. |

Full-on is the plastic pole (pressure). Full-off is the metal pole (vacuum). A finished REP leaves channel 8 full-on until the sketch releases it. `stopRep` leaves the valve as it is. `/system/off` and a BLE disconnect release it.

Physical connections are in `BuildYourOwn/RheoBoardOTS/hardware/electronic-wiring/` and on the Assembly instructions page.

## Control API

| OSC address | Description |
|---|---|
| `rheo/rep` | Trigger one REP. Ignored if a REP, bubble, or program is already running. |
| `rheo/rep/triad` | Trigger three REPs, `rheo/rep/interval` ms apart. |
| `rheo/bubble` | One surface-tension pulse: 50 ms at 10% blow, then 200 ms capture. |
| `rheo/bubble/triad` | Three of those pulses, separated by `rheo/rep/interval`. |
| `rheo/stop` | Abort the current REP or bubble and cancel its triad. |
| `rheo/purge` | One-second pressure-pump purge, then park. |
| `rheo/program/clear` | Clear the uploaded program while idle. |
| `rheo/program/step` | Append one REST, RETRACT, PUSH, RAMP, or PULSE step. |
| `rheo/program/run` | Run the uploaded program. |
| `rheo/program/stop` | Cancel the program and park. |
| `rheo/api` | Print supported OSC messages and send each back over BLE. |

`/slip/pa/rep`, `/slip/pa/stop`, `/slip/pa/purge`, `/slip/air/rate`, `/slip/pump/add`, `/slip/pump/remove`, and `/slip/pump/off` are aliases for the same actions.

### Sampling

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/sense/rate` | `[<ms>]` | 10 | Bubble and bench-test spacing, clamped to 10–1000 ms. REP capture stays on an 18 ms grid. Omit the value to read it. |

All parameter addresses below accept an optional value. Omit it to read the current setting. Serial Monitor then prints the address and the value, for example `/rheo/rep/pull/power=100`.

### REP parameters

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/pull/power` | `[<0–100>]` | 100 | Retract pump drive level (%). |
| `rheo/rep/pull/time` | `[<ms>]` | 250 | Retract duration, clamped to 1–5000 ms. |
| `rheo/rep/push/delay` | `[<ms>]` | 0 | Pumps-off delay before extrude. |
| `rheo/rep/push/power` | `[<0–100>]` | 100 | Extrude drive level (%). |
| `rheo/rep/push/time` | `[<ms>]` | 250 | Extrude duration, clamped to 1–5000 ms. |
| `rheo/rep/push/ramp/start` | `[<0–100>]` | 0 | Extrude ramp start. Unused while ramp time is 0. |
| `rheo/rep/push/ramp/time` | `[<ms>]` | 0 | Ramp duration. `0` is instant. |
| `rheo/rep/baseline/time` | `[<ms>]` | 250 | Wait before retract. |
| `rheo/rep/interval` | `[<ms>]` | 500 | Gap between REPs in a triad. |

### Data path

| OSC address | Description |
|---|---|
| `/rheo/sense/air` | Pressure sample in Pa. A REP sends one sample per 18 ms slot over BLE. |
| `/db/start` | Signals the start of a REP recording. |
| `/db/save` | Signals the end of a REP recording. |

## REP timing

A REP runs `WAIT → RETRACT → EXTRUDE → RELAX` in a fixed 1200 ms window (`REP_TIME` in `RheoBoardOTS/RheoSystem.h`). Factory drive is 100% retract and 100% extrude. Ramp time is 0. `VALVE_SETTLE_MS` is 0.

```text
0 ms              250                 500                 750              1200 ms
|──── wait 250 ────|──── retract 250 @ 100% ────|──── push 250 @ 100% ────|── relax ──|
```

Baseline does not move the valve. Retract calls `valveSuck()` with PUMP1. Extrude calls `valveBlow()` with PUMP2. At 750 ms both pumps go full-off and the valve stays full-on. Boot parks the valve full-off, so the first baseline is full-off. After a completed REP the next baseline stays full-on until 250 ms.

The saved waveform is 66 samples, one every 18 ms. USB does not print those samples. `/db/save` goes out before the end-of-REP serial log.

## Bench serial commands

Available over USB when `SERIAL_STREAM` is enabled. A newline or a carriage return ends a command.

| Command | Description |
|---|---|
| `REP` | Trigger one REP. |
| `STOP` | Abort a REP, a bubble, or a pump test. Also stops a purge or timed-air run. |
| `PUMP1 <pct>` | Run PUMP1 (channel 10) continuously and stream `#S` lines. |
| `PUMP2 <pct>` | Run PUMP2 (channel 9) continuously and stream `#S` lines. |
| `VALVE ON` | Channel 8 full-on. Plastic pole, pressure. |
| `VALVE OFF` | Channel 8 full-off. Metal pole, vacuum. |

After a REP, Serial Monitor prints:

```text
#REP_START
#PH,<ms>,<phase>
#REP_SAMPLES,<count>
#REP_END
```

`#PH` lines are buffered during the REP and printed together after it ends. With the factory defaults the phase names are `BASELINE`, `RETRACT_VALVE`, `RETRACT_PUMP`, `EXTRUDE_VALVE`, `EXTRUDE_PUMP`, `RELAX`, and `STOP`. A REP does not print `#S` lines. `#S,<ms>,<Pa>` is the bench pump-test line, with `#TEST_START,<Pa>` and `#TEST_END`.

Boot, when the PCA9685 answers, prints the channel map, pulses channel 9 three times, parks it full-off, pulses channel 8 three times, and parks it full-off. Channel 11 stays full-off. The sketch then prints `RheoBoardOTS initialized. Send rheo/api for supported commands.`

Missing-board lines:

```text
PneumaticSystem: PCA9685 not found on I2C addr 0x40 (header SDA GPIO 23 / SCL GPIO 22).
PneumaticSystem: MPRLS pressure sensor not found on Qwiic bus.
PneumaticSystem: HAL init failed (PCA9685 and/or MPRLS not found).
Qwiic button not found (optional, skipping).
```

The HAL-failed line stops the sketch. The button line does not.
