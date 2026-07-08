# References

External datasheets, standards, and third-party library notes the design depends on. Link out
rather than duplicating content, and note the revision/date you relied on since datasheets and
standards change over time.

| Component/Standard | Link | Notes |
|---|---|---|
| SparkFun ESP32 Thing Plus (micro-USB, WRL-15663) | https://www.sparkfun.com/sparkfun-esp32-thing-plus.html | MCU for 2P1VX firmware; plain ESP32-WROOM-32D/E (not S2/S3); [hardware repo](https://github.com/sparkfun/ESP32_Thing_Plus); schematic/graphical datasheet vendored in `references/datasheets/` |
| SparkFun Qwiic MicroPressure (MPRLS) | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | I2C `0x18`; [hardware repo](https://github.com/sparkfun/MicroPressure_Sensor) |
| SparkFun Qwiic Button (red, BOB-15932) | https://www.sparkfun.com/sparkfun-qwiic-button.html | Required in 2P1VX system (1-click=REP, hold=blow); default I2C `0x6F`; [hardware repo](https://github.com/sparkfun/Qwiic_Button) |
| Adafruit 4700 air pump / vacuum (ZR320-02PM) | https://www.adafruit.com/product/4700 | ~4.5 V, 1.8 LPM; flow fixed by port plumbing; datasheet in `references/datasheets/ZR320-02PM_4.5V.pdf` |
| Adafruit 4663 air valve (FA0520E) | https://www.adafruit.com/product/4663 | 6 V 3-port flip valve; datasheet in `references/datasheets/4663_C14660_DC_6V.pdf` |
| Adafruit silicone tubing 3 mm ID | https://www.adafruit.com/product/4664 | For 4700 pumps and 4663 valve ports |
| Honeywell MPR series datasheet | vendored: `references/datasheets/Honeywell_MPR_Series_Datasheet.pdf` | Source: https://cdn.sparkfun.com/assets/2/e/8/0/9/honeywell-mpr-datasheet.pdf |
| ESP32 Thing Plus schematic | vendored: `references/datasheets/ESP32_Thing_Plus_Schematic.pdf` | SparkFun v2.0 Eagle schematic export |
| ESP32 Thing Plus graphical datasheet | vendored: `references/datasheets/ESP32_Thing_Plus_Graphical_Datasheet.pdf` | Pinout diagram, power specs |
| Qwiic Button schematic | vendored: `references/datasheets/Qwiic_Button_Schematic.pdf` | SparkFun Eagle schematic export |
| L298N (ST) | https://www.st.com/resource/en/datasheet/l298.pdf | Dual H-bridge IC datasheet (generic module, no single canonical vendor page) |
| ESP32 Arduino core (Espressif) | https://espressif.github.io/arduino-esp32/package_esp32_index.json | Boards Manager URL |
| SparkFun Qwiic Button library | https://github.com/sparkfun/SparkFun_Qwiic_Button_Arduino_Library | Arduino Library Manager |
| SparkFun MicroPressure library | https://github.com/sparkfun/SparkFun_MicroPressure_Arduino_Library | Arduino Library Manager |
| **ThingPlusBLEOSC** | https://github.com/cearto/ThingPlusBLEOSC | BLE + OSC transport for RheoData; MIT license; not on Library Manager — `git clone` into Arduino `libraries/`. Depends on **OSC** (Adrian Freed, Library Manager) and **ESP32 BLE Arduino** (Neil Kolban, usually bundled with the `esp32` core). |
