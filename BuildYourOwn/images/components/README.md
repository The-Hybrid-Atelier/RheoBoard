# Component photos

Reference photos for each electronic part in [`../../BOM.md`](../../BOM.md), pulled from the
manufacturer/vendor's own product photography (or a freely-licensed source where no vendor photo
exists). These are for builder identification only — not renders of *our* build.

| File | Part | Source |
|---|---|---|
| `esp32-thing-plus.jpg` | SparkFun ESP32 Thing Plus (micro-USB), WRL-15663 | [SparkFun product page](https://www.sparkfun.com/sparkfun-esp32-thing-plus.html) / [hardware repo](https://github.com/sparkfun/ESP32_Thing_Plus) |
| `qwiic-micropressure.jpg` | SparkFun Qwiic MicroPressure Sensor, SEN-16476 | [SparkFun product page](https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html) / [hardware repo](https://github.com/sparkfun/MicroPressure_Sensor) |
| `qwiic-button.jpg` | SparkFun Qwiic Button, red, BOB-15932 | [SparkFun product page](https://www.sparkfun.com/sparkfun-qwiic-button.html) / [hardware repo](https://github.com/sparkfun/Qwiic_Button) |
| `adafruit-4700-air-pump.jpg` | Adafruit 4700 — Air Pump and Vacuum DC Motor (ZR320-02PM) | [Adafruit product page](https://www.adafruit.com/product/4700) |
| `adafruit-4663-air-valve.jpg` | Adafruit 4663 — 6V Air Valve (FA0520E) | [Adafruit product page](https://www.adafruit.com/product/4663) |
| `l298n-motor-driver.jpg` | Generic L298N dual H-bridge module | Cropped from [Wikimedia Commons: *Dosmotorsl298n.jpg*](https://commons.wikimedia.org/wiki/File:Dosmotorsl298n.jpg) by Quel.soler, [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/) — no single canonical vendor page exists for this generic module, so this is the best-effort stand-in; swap for your exact board's photo if it differs. |

## Note on the ESP32 Thing Plus variant

There are two similarly-named SparkFun boards — make sure you have the right one:

- **SparkFun ESP32 Thing Plus (micro-USB, WRL-15663)** — used by this build. Programs and powers
  over micro-USB.
- **SparkFun Thing Plus – ESP32 WROOM (USB-C, WRL-20168)** — a newer, different board with a
  USB-C connector. **Not** what the firmware in this repo targets; do not substitute without
  checking pinout differences first.

## Notes

- SparkFun and Adafruit images are used here for **build identification purposes** (this is what
  the part looks like when it arrives) — not redistributed as marketing material. Link to the
  product page for authoritative specs.
- If a part is substituted, replace the photo here and update `BOM.md`'s row accordingly.
