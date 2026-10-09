# Firmware

**Entry point:** [`2P1V_Adafruit/2P1V_Adafruit.ino`](2P1V_Adafruit/2P1V_Adafruit.ino) —
this design's 2 pumps, 1 valve bench firmware for **RheoData** (SparkFun ESP32 Thing Plus,
micro-USB, WRL-15663 + 2× L298N + Qwiic MicroPressure + Qwiic Button + Adafruit ATtiny1616
seesaw breakout). The ESP32 commands the seesaw over Qwiic; seesaw pins `0`, `1`, and `5` then
drive the L298N `ENA`/`ENB` inputs through discrete wires instead of native ESP32 GPIO. The sketch
file/BLE device name in code: `2P1V_Adafruit`.

This firmware is shared by the DIY build, the PCB, and the portable version. Sketch sources live
in `2P1V_Adafruit/` so the sketch folder matches the primary `.ino` filename as Arduino requires.
Firmware is MIT-licensed; the license notice and full text are in the repository-root
[`LICENSE`](../LICENSE). RheoData, the app used with this firmware, is noted in
[`../software/README.md`](../software/README.md). Download, pairing, the connected indicator, and
how to start and name a REP stay TODO and will be specified later in that file and in these notes.

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
4. Builder walkthrough: [`../BuildYourOwn/README.md`](../BuildYourOwn/README.md) → Step 03.

After upload, USB may be disconnected. Portable power is in [Power](#power) below.

## Power

Everyday wall power for the DIY build and for the PCB is a 12 V plug. Pump and valve current
does not go through the ESP32 5 V pin.

USB uploads this firmware to the SparkFun ESP32 Thing Plus. After upload, USB may be disconnected.

Portable power is a [SparkFun Lithium Ion Battery 1500 mAh, IEC62133 certified (SKU PRT-26059)](https://www.sparkfun.com/lithium-ion-battery-1500mah-iec62133-certified.html): nominal 3.7 V, 1500 mAh, terminated with a 2-pin JST-PH connector (2 mm pin spacing), with built-in protection, and it plugs into the ESP32 Thing Plus JST battery connector ([schematic](../BuildYourOwn/hardware/references/datasheets/ESP32_Thing_Plus_Schematic.pdf) V_BATT, 4.2 V maximum).

## Connect and use (summary)

- **BLE:** device advertises as `2P1V_Adafruit`; control via RheoData bridge / OSC (`rheo/rep`, etc.).
- **USB serial (bench):** commands `REP`, `STOP`, `PUMP1 <pct>`, `PUMP2 <pct>` when `SERIAL_STREAM`
  is enabled (default).
- **Qwiic button:** 1-click = REP, 2-click = latched vacuum, hold = momentary pressure.

Builder workflow: [Step 05 — Calibrate and use](../BuildYourOwn/README.md#step-05-calibrate-and-use).

## Firmware pin map

The firmware expects these I2C addresses:

- Qwiic Button: `0x6F`
- MicroPressure (MPRLS): `0x18`
- ATtiny1616 seesaw: `0x49`

| Seesaw pin | Signal | Drives | Pin type |
|---|---|---|---|
| `0` | `PUMP1_EN` | L298N #1 `ENA` — vacuum pump | PWM |
| `1` | `PUMP2_EN` | L298N #1 `ENB` — pressure pump | PWM |
| `4` | `VALVE1_EN` | Reserved for L298N #2 `ENA`; physically NC | digital |
| `5` | `VALVE2_EN` | L298N #2 `ENB` — the only valve driven | digital |

These constants are defined in `2P1V_Adafruit/PneumaticSystem.h`. Physical connections and power
rules are maintained in
[`../BuildYourOwn/hardware/electronic-wiring/README.md`](../BuildYourOwn/hardware/electronic-wiring/README.md).

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
