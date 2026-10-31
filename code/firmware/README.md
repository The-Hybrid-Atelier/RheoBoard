# Firmware

**Entry point:** `RheoBoardOTS/RheoBoardOTS.ino`. Arduino requires the sketch folder name to match the `.ino` file. The sketch file and BLE device name are `RheoBoardOTS`.

This sketch defaults to the RheoBoardOTS DIY controller. An experimental profile for the **older v1 PCB** is described below. It is not claimed ready for the v1.05 PCB. That board's PCA9685 uses PWM0–PWM4 for pumps and PWM5–PWM6 for valves. PWM8–PWM10 on that board are unconnected.

Upload, startup checks, and the [First REP test](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Firmware#first-rep-test) are on the [Firmware](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Firmware) wiki page. RheoData notes are in `../software/README.md`. The download, pairing steps, the connected indicator, and how to start and name a REP are not written yet. That gap stays in the software notes.

Libraries: Adafruit PWM Servo Driver and Adafruit BusIO, SparkFun MicroPressure, SparkFun Qwiic Button, and OSC. ThingPlusBLEOSC supplies `BLEHandler.h`, `OSCHandler.h`, and `MemoryStream.h`.

Firmware is MIT-licensed. The license notice and full text are in [`LICENSE`](../../LICENSE).

## DIY I2C

`Wire.begin()` uses the SparkFun ESP32 Thing Plus defaults: SDA GPIO 23, SCL GPIO 22. The Qwiic connector is that same bus.

| Board | Address | Connection |
|---|---|---|
| Qwiic Button | `0x6F` | Qwiic cable |
| MicroPressure (MPRLS) | `0x18` | Qwiic cable |
| Adafruit PCA9685, product 815 | `0x40` | Qwiic-to-jumper cable from the MicroPressure sensor. No address jumpers. |

PCA9685 VCC receives 3.3 V through the sensor’s Qwiic-to-jumper cable; black is GND, red is VCC, blue is SDA, and yellow is SCL. V+ and OE stay open. Product 815 has 220 ohm resistors in series with its outputs.

## DIY channel map

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
| `PUMP1 <pct>` | Run PUMP1 continuously and stream `#S` lines. DIY channel 10; v1 default channel 1/U11. |
| `PUMP2 <pct>` | Run PUMP2 continuously and stream `#S` lines. DIY channel 9; v1 default channel 0/U8. |
| `VALVE ON` | Valve powered. Plastic pole, pressure. DIY channel 8; v1 default TCA P1/U9. |
| `VALVE OFF` | Valve released. Metal pole, vacuum. |
| `BUTTONS` | v1 only: print `#BUTTONS,<binary mask>` for SW1–SW4; least significant bit is SW1, 1 means pressed. |

After a REP, Serial Monitor prints:

```text
#REP_START
#PH,<ms>,<phase>
#REP_SAMPLES,<count>
#REP_END
```

`#PH` lines are buffered during the REP and printed together after it ends. With the factory defaults the phase names are `BASELINE`, `RETRACT_VALVE`, `RETRACT_PUMP`, `EXTRUDE_VALVE`, `EXTRUDE_PUMP`, `RELAX`, and `STOP`. A REP does not print `#S` lines. `#S,<ms>,<Pa>` is the bench pump-test line, with `#TEST_START,<Pa>` and `#TEST_END`.

DIY boot, when the PCA9685 answers, prints the channel map, pulses channel 9 three times, parks it full-off, pulses channel 8 three times, and parks it full-off. Channel 11 stays full-off. The v1 profile has no automatic output pulses. The sketch then prints `RheoBoardOTS initialized. Send rheo/api for supported commands.`

Missing-board lines:

```text
PneumaticSystem: PCA9685 not found on I2C addr 0x40 (header SDA GPIO 23 / SCL GPIO 22).
PneumaticSystem: MPRLS pressure sensor not found on Qwiic bus.
PneumaticSystem: hardware initialization failed; actuation disabled. Check the selected hardware profile, supplies and I2C devices.
Qwiic button not found (optional, skipping).
```

The HAL-failed line stops the sketch. The button line does not.

## Older v1 PCB profile (experimental)

This profile targets `RheoBoard_PCB/RheoBoard_v1`, using its own MOSFET stages instead of L298N modules. It has passed host protocol/fault tests and an ESP32 Thing Plus build, but **has not been tested on an assembled PCB**. A successful build does not qualify the power supplies, assembly, sensor startup or pneumatic behavior. The DIY wiki's wiring and startup-pulse instructions do not apply to this profile.

Select `RHEO_PROFILE_PCB_V1` in `RheoBoardOTS/HardwareProfile.h`, or build without changing the default:

```sh
arduino-cli compile --fqbn esp32:esp32:esp32thing_plus \
  --build-property compiler.cpp.extra_flags=-DRHEO_HARDWARE_PROFILE=1 \
  code/firmware/RheoBoardOTS
```

Run that command from the repository root. It compiles only; it does not upload. The verified local toolchain is ESP32 Arduino core 2.0.11 with the libraries listed above. Omitting the flag builds DIY. Keep the profile selection consistent when compiling and uploading.

| v1 device | Address / output | Connection and default role |
|---|---|---|
| PCA9685 | `0x40`, channel 0 | Q3 → U8, PUMP2 / pressure |
| PCA9685 | `0x40`, channel 1 | Q4 → U11, PUMP1 / vacuum |
| TCA9534A | `0x3F`, P1 | Q8 → U9, selected valve |
| TCA9534A | `0x3F`, P0 | Q10 → U10, unused valve held off |
| TCA9534A | P4–P7 | SW1–SW4, active low |
| Honeywell MPRLS0025PA00001A | `0x18` | Absolute pressure in Pa, transfer function A |

These addresses assume the PCA address jumpers are open and the TCA address straps match the saved design. `RHEO_V1_VACUUM_CHANNEL` selects 0 or 1; the pressure pump uses the other channel. `RHEO_V1_VALVE_PORT` selects 0/U10 or 1/U9. Confirm tubing direction before assigning pressure/vacuum roles. SW1 uses the existing single-click REP, double-click suck, and hold-to-blow behavior. SW2–SW4 are reported by `BUTTONS` but have no assigned action. Testing SW1 can trigger a routine: leave loads disconnected during button checks.

At initialization, the profile parks all PCA outputs, writes the TCA output latch low before setting P0/P1 as outputs, checks register readback, and requires a valid pressure conversion. Pump PWM is approximately 1.02 kHz with the nominal 25 MHz PCA oscillator. Both valve states are steady DC. The opposite pump is turned off before a pump is energized. Unused PCA outputs and the second valve stay off.

For v1, serial `STOP`, `/system/off`, `/pump/off`, `/slip/pump/off`, `/rheo/stop`, `/slip/pa/stop`, and `/rheo/program/stop` cancel every active routine and release both pumps and the valve. Zero-power `/pump/blow`, `/pump/suck`, `/slip/pump/add`, and `/slip/pump/remove` commands do the same even while busy. A pending button gesture cannot restart an output after a stop; release the button before another gesture. BLE stop requests take priority over a full queue and discard older queued commands. These remain cooperative software stops, dependent on a running controller and functioning I2C bus.

Sensor busy polling has a 30 ms deadline and each ESP32 Wire transaction has a 25 ms timeout; a failed transaction can extend the total call beyond the polling deadline. Sensor faults or failed I2C transfers latch actuation disabled until restart and attempt to switch both expanders off. A failed bus can prevent that shutdown. The expanders can also retain output state during an ESP32-only reset, until initialization reaches them. **Removing the 12 V supply is the independent bench stop.** This firmware does not add a hardware watchdog.

Before connecting a host or load, verify the unpowered board and its unloaded 3.3 V, 4.5 V and 6 V rails. With a USB-powered Thing Plus, do not connect the v1 regulator's 3.3 V output to the host regulator output. A continuity-verified connection using only GND, SDA and SCL avoids that direct tie; it does not prevent power-off backfeeding through SDA/SCL. Do not hot-plug. J3's printed SDA/SCL labels are reversed: its electrical pin order is **1 GND, 2 +3V3, 3 SDA, 4 SCL**. The Qwiic sockets have normal signal order but still connect the two supply outputs with a standard four-wire cable.

The stock v1 sensor reset is only pulled up. Reliable cold startup remains to be established because the Honeywell sensor specifies a supply ramp/reset requirement. `RHEO_V1_SENSOR_RESET_PIN` defaults to `-1`; an optional, reviewed physical wire from a suitable spare ESP32 GPIO to TP3 enables an open-drain reset pulse. Setting a number without that wire cannot fix sensor startup. Do not use an occupied GPIO or a board test point as ground. Repeated boot checks and measurement against ambient absolute pressure are required.

Run software checks from the repository root with `bash code/firmware/tests/run-tests.sh`. They compile the actual v1 driver against a register model and exercise output ordering, transfer-function conversion, buttons, stale-state restart, failures at every startup transaction, corrupt/short reads, and latched bus/sensor faults. These tests do not model electrical transients or certify the physical board. After electrical checks, test each output unloaded, then one load at a time, then the shared REP routine and stop behavior.

`bash code/firmware/tests/run-application-tests.sh` additionally compiles the actual sketch and all its controller sources with the installed OSC and ThingPlusBLEOSC parser. Only the Arduino hardware, radio transport and I2C devices are simulated. It checks serial/BLE commands, REP phases and completion, programs, disconnect, sensor faults, button gestures, queue saturation, and a 70-case stop matrix. Set `ARDUINO_LIBRARIES` if the libraries are outside `~/Documents/Arduino/libraries`. Run again with `RHEO_TEST_ALTERNATE=1` to exercise swapped pump roles and the other valve output. This host simulation does not prove physical behavior, radio delivery, real-time scheduling, or ESP32 timer rollover.
