# Software / firmware

**Entry point:** [`2P1V_Adafruit/2P1V_Adafruit.ino`](2P1V_Adafruit/2P1V_Adafruit.ino) —
this design's 2 pumps, 1 valve bench firmware for **RheoData** (SparkFun ESP32 Thing Plus,
micro-USB, WRL-15663 + 2× L298N + Qwiic MicroPressure + Qwiic Button + Adafruit ATtiny1616
seesaw breakout). The ESP32 commands the seesaw over Qwiic; seesaw pins `0`, `1`, and `5` then
drive the L298N `ENA`/`ENB` inputs through discrete wires instead of native ESP32 GPIO. The sketch
file/BLE device name in code: `2P1V_Adafruit`.

This folder contains one README/API reference. Arduino sketch sources live in `2P1V_Adafruit/` so
the sketch folder matches the primary `.ino` filename as Arduino requires. Firmware is MIT-licensed;
the license notice and full text are in the repository-root [`LICENSE`](../../LICENSE).

## Toolchain

| Item | Value |
|---|---|
| IDE | [Arduino IDE](https://www.arduino.cc/en/software) 2.x |
| Board | **SparkFun ESP32 Thing Plus** (or generic **ESP32 Dev Module** — plain ESP32-WROOM-32D/E, not S2/S3) |
| ESP32 core | Install via Boards Manager: `esp32` by **Espressif Systems** (add URL below) |
| Baud | 115200 |
| Upload port | micro-USB (same port powers the board and flashes firmware) |

**Additional Boards Manager URL** (Arduino IDE → Settings):

```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

(Espressif may also publish `https://dl.espressif.com/dl/package_esp32_index.json` — use whichever
your core version documents.)

## Arduino libraries

| Library | Install via | Purpose |
|---|---|---|
| [SparkFun Qwiic Button](https://github.com/sparkfun/SparkFun_Qwiic_Button_Arduino_Library) | Library Manager | External Qwiic button gestures (REP trigger, latched suck/blow) |
| [SparkFun MicroPressure](https://github.com/sparkfun/SparkFun_MicroPressure_Arduino_Library) | Library Manager | MPRLS sensor |
| [Adafruit seesaw Library](https://github.com/adafruit/Adafruit_Seesaw) | Library Manager (search "Adafruit seesaw Library") | Drives the ATtiny1616 seesaw breakout's PWM/GPIO pins (L298N `ENA`/`ENB`) |
| **[ThingPlusBLEOSC](https://github.com/cearto/ThingPlusBLEOSC)** | Manual (below) | BLE + OSC transport — not on Library Manager |
| OSC (by Adrian Freed) | Library Manager | Required by ThingPlusBLEOSC |
| ESP32 BLE Arduino (by Neil Kolban) | Usually bundled with the `esp32` core | Required by ThingPlusBLEOSC |

**Installing ThingPlusBLEOSC manually:**

```bash
cd ~/Documents/Arduino/libraries
git clone https://github.com/cearto/ThingPlusBLEOSC.git
```

Restart the Arduino IDE afterward so it picks up the new library.

## Upload

1. Open `2P1V_Adafruit/2P1V_Adafruit.ino` in Arduino IDE (from this repo, or your sketchbook copy).
2. Select board **SparkFun ESP32 Thing Plus** (or **ESP32 Dev Module**) and the micro-USB port.
3. Upload. Serial Monitor @ 115200 should print `2P1V_Adafruit initialized`.
4. Builder walkthrough: [`../README.md`](../README.md) → Step 04.

Add a screenshot of correct board/port settings to `../images/ide-settings.png` when captured.

## Developer sketchbook copy

Firmware may also be edited from:

`/Users/charlievuong/Documents/Arduino/RheoData/thingplus/2P1V_Adafruit`

**`BuildYourOwn/software/` in this repo is the copy to commit.** Sync changes between the
sketchbook and repo before committing so they don't drift.

## Connect and use (summary)

- **BLE:** device advertises as `2P1V_Adafruit`; control via RheoData bridge / OSC (`rheo/rep`, etc.).
- **USB serial (bench):** commands `REP`, `STOP`, `PUMP1 <pct>`, `PUMP2 <pct>` when `SERIAL_STREAM`
  is enabled (default).
- **Qwiic button:** 1-click = REP, 2-click = latched vacuum, hold = momentary pressure.

Full connect/use section: [root README → Connect and use](../../README.md#connect-and-use).

## Hardware / wiring

The ESP32 Thing Plus's Qwiic (I2C) bus is daisy-chained to three boards:

1. **SparkFun Qwiic Button** — manual REP / suck-toggle / blow gestures, address `0x6F`
2. **SparkFun MicroPressure (MPRLS)** — REP pressure sensing, address `0x18`
3. **Adafruit ATtiny1616 Breakout (seesaw)** — pump/valve control, address `0x49`

The addresses do not collide and physical order along the Qwiic chain does not matter.

The seesaw board supplies three control signals that earlier builds sourced from the ESP32.
These are discrete point-to-point wires: seesaw pins `0`, `1`, and `5` run directly to the L298N
boards. Qwiic carries I2C commands and 3.3 V logic power, not the L298N control signals or motor
current. Pin `4` is reserved in firmware but physically NC in this single-valve build.

| Seesaw pin | Signal | Drives | Pin type |
|---|---|---|---|
| `0` | `PUMP1_EN` | L298N #1 `ENA` — vacuum pump | PWM |
| `1` | `PUMP2_EN` | L298N #1 `ENB` — pressure pump | PWM |
| `4` | `VALVE1_EN` | Reserved for L298N #2 `ENA`; physically NC | digital |
| `5` | `VALVE2_EN` | L298N #2 `ENB` — the only valve driven | digital |

L298N #2 uses only Motor B: `ENB` → seesaw pin `5`, `IN3` → local +5 V, `IN4` → GND, and
`OUT3/OUT4` → VALVE2. Its Motor A terminals (`ENA`, `IN1`, `IN2`, `OUT1/OUT2`) are NC.
On both L298N modules, keep `5V-EN` ON and remove the ENA/ENB jumper caps. Do not parallel the
modules' +5 V outputs or apply external 5 V while `5V-EN` is installed.

The seesaw logic is powered from Qwiic 3.3 V and GND; its separate `Vin` header is NC. The pumps
and valve remain powered through the L298N boards from the 12 V motor rail.

See the
[`../hardware/electronic-wiring/wiring-diagram.png`](../hardware/electronic-wiring/wiring-diagram.png)
for the full connection diagram.

## Control API

| OSC address | Description |
|---|---|
| `rheo/rep` | Trigger one REP. Ignored if a REP is already running. |
| `rheo/rep/triad` | Trigger three REPs, `rheo/rep/interval` ms apart. |
| `rheo/stop` | Abort the current REP and cancel its triad. |
| `rheo/purge` | Push the line clear with the pressure pump, then stop. |
| `rheo/api` | Print supported OSC messages and send each back over BLE. |

### Sampling

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/sense/rate` | `[<ms>]` | 10 | Pressure-sampling interval, clamped to 10–1000 ms. Omit the value to read it. |

All parameter addresses below accept an optional value. Omit it to read the current setting.

### REP parameters

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/pull/power` | `[<0–100>]` | 54 | Retract pump drive level (%). |
| `rheo/rep/pull/time` | `[<ms>]` | 315 | Retract pump run duration, clamped to 1–5000 ms. |
| `rheo/rep/push/power` | `[<0–100>]` | 100 | Extrude ramp ceiling (%). |
| `rheo/rep/push/time` | `[<ms>]` | 345 | Extrude pump run duration, clamped to 1–5000 ms. |
| `rheo/rep/push/ramp/start` | `[<0–100>]` | 40 | Extrude drive level at pump-on. |
| `rheo/rep/push/ramp/time` | `[<ms>]` | 300 | Linear ramp duration; `0` disables the ramp. |
| `rheo/rep/baseline/time` | `[<ms>]` | 420 | Ambient-sampling window before retract. |
| `rheo/rep/interval` | `[<ms>]` | 500 | Gap between REPs in a triad. |

The extrude drive ramps from `push/ramp/start` to `push/power` over `push/ramp/time`, then holds
at `push/power` for the remainder of `push/time`.

### Data path

| OSC address | Description |
|---|---|
| `/rheo/sense/air` | Pressure sample in Pa, streamed while recording. |
| `/db/start` | Signals the start of a REP recording. |
| `/db/save` | Signals the end of a REP recording. |

## REP timing

A REP runs `BASELINE → RETRACT → EXTRUDE → RELAX` in a fixed 1500 ms window (`REP_TIME` in
`2P1V_Adafruit/RheoSystem.h`). The relax tail is the time remaining after baseline, valve-settle,
pull, and push.

```text
0 ms        baseline       settle + retract       settle + extrude       1500 ms
|──────── pumps off ───────|──── vacuum ────|──────── pressure ────────|─ relax ─|
```

## Bench serial commands

Available over USB when `SERIAL_STREAM` is enabled:

| Command | Description |
|---|---|
| `REP` | Trigger one REP. |
| `STOP` | Abort a REP or pump test. |
| `PUMP1 <pct>` | Run the vacuum pump continuously and stream pressure samples. |
| `PUMP2 <pct>` | Run the pressure pump continuously and stream pressure samples. |

Serial output uses `#REP_START`, `#REP_END`, `#PH,<ms>,<phase>`, `#S,<ms>,<Pa>`,
`#TEST_START`, and `#TEST_END`.
