# Software / firmware

**Entry point:** [`2P1VX/2P1VX.ino`](2P1VX/2P1VX.ino) — 2 pumps, 1 valve bench firmware for
**RheoData** (SparkFun ESP32 Thing Plus + L298N + MPRLS). BLE device name: `2P1VX`.

API reference: [`2P1VX/README.md`](2P1VX/README.md) (OSC commands, REP parameters, serial bench
commands).

## Toolchain

| Item | Value |
|---|---|
| IDE | [Arduino IDE](https://www.arduino.cc/en/software) 2.x |
| Board | **ESP32S3 Dev Module** or **SparkFun ESP32 Thing Plus** (ESP32-S3) |
| ESP32 core | Install via Boards Manager: `esp32` by **Espressif Systems** (add URL below) |
| Baud | 115200 |

**Additional Boards Manager URL** (Arduino IDE → Settings):

```
https://espressif.github.io/arduino-esp32/package_esp32_index.json
```

(Espressif may also publish `https://dl.espressif.com/dl/package_esp32_index.json` — use whichever
your core version documents.)

## Arduino libraries (install via Library Manager unless noted)

| Library | Purpose |
|---|---|
| [SparkFun Qwiic Button](https://github.com/sparkfun/SparkFun_Qwiic_Button_Arduino_Library) | Optional onboard gestures |
| [SparkFun MicroPressure](https://github.com/sparkfun/SparkFun_MicroPressure_Arduino_Library) | MPRLS sensor |
| **ThingPlusBLEOSC** | BLE + OSC transport — **local library** (not on Library Manager). Install by copying into Arduino `libraries/` or symlink from your dev machine. |

## Upload

1. Open `2P1VX/2P1VX.ino` in Arduino IDE (from this repo, or your sketchbook copy — see below).
2. Select board **ESP32S3 Dev Module** (or Thing Plus equivalent) and the correct USB port.
3. Upload. Serial Monitor @ 115200 should print `2P1VX initialized`.
4. Builder walkthrough: [`../tutorial/steps/04-install-firmware/`](../tutorial/steps/04-install-firmware/).

Add a screenshot of correct board/port settings to `images/ide-settings.png` when captured (Calico
pattern).

## Developer sketchbook copy

Firmware may also be edited from:

`/Users/charlievuong/Documents/Arduino/RheoData/thingplus/2P1VX`

**`BuildYourOwn/software/2P1VX/` in this repo is the copy to commit.** Sync changes between
sketchbook and repo before committing so they don't drift.

## Connect and use (summary)

- **BLE:** device advertises as `2P1VX`; control via RheoData bridge / OSC (`rheo/rep`, etc.).
- **USB serial (bench):** commands `REP`, `STOP`, `PUMP1 <pct>`, `PUMP2 <pct>` when `SERIAL_STREAM`
  is enabled (default).
- **Qwiic button:** 1-click = REP, 2-click = latched vacuum, hold = momentary pressure.

Full connect/use section: [`../README.md`](../README.md) → Connect and use.
