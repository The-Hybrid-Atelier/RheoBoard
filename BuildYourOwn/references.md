# References

External datasheets, standards, and third-party library notes the design depends on. Link out
rather than duplicating content, and note the revision/date you relied on since datasheets and
standards change over time.

| Component/Standard | Link | Notes |
|---|---|---|
| SparkFun ESP32 Thing Plus | https://www.sparkfun.com/sparkfun-esp32-thing-plus.html | MCU for 2P1VX firmware |
| SparkFun Qwiic MicroPressure (MPRLS) | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | I2C `0x18` |
| SparkFun Qwiic Button | https://www.sparkfun.com/sparkfun-qwiic-button.html | Optional; firmware degrades gracefully if absent |
| Adafruit 4700 air pump / vacuum (ZR320-02PM) | https://www.adafruit.com/product/4700 | ~4.5 V, 1.8 LPM; flow fixed by port plumbing; datasheet in `references/datasheets/ZR320-02PM_4.5V.pdf` |
| Adafruit 4663 air valve (FA0520E) | https://www.adafruit.com/product/4663 | 6 V 3-port flip valve; datasheet in `references/datasheets/4663_C14660_DC_6V.pdf` |
| Adafruit silicone tubing 3 mm ID | https://www.adafruit.com/product/4664 | For 4700 pumps and 4663 valve ports |
| SparkFun Qwiic MicroPressure datasheet (MPR series) | https://cdn.sparkfun.com/assets/1/1/3/3/1/MPRLS0025PA00001A.pdf | Honeywell MPRLS0025PA00001A |
| L298N (ST) | https://www.st.com/resource/en/datasheet/l298.pdf | Dual H-bridge module datasheet |
| ESP32 Arduino core (Espressif) | https://espressif.github.io/arduino-esp32/package_esp32_index.json | Boards Manager URL |
| SparkFun Qwiic Button library | https://github.com/sparkfun/SparkFun_Qwiic_Button_Arduino_Library | Arduino Library Manager |
| SparkFun MicroPressure library | https://github.com/sparkfun/SparkFun_MicroPressure_Arduino_Library | Arduino Library Manager |
| ThingPlusBLEOSC (local) | Developer sketchbook: `Arduino/libraries/ThingPlusBLEOSC` | BLE + OSC transport for RheoData; **not** on Library Manager — copy/symlink into Arduino `libraries/` |
| Calico (primary doc model) | https://github.com/jsli96/calico | Single README: features, hardware, IDE setup, connect/use, tips + `3D print models/` + `PCB files/` + `main_app.ino`. Closest reference for RheoBoard BYO layout. |
| OpenTheremin V4 (secondary) | https://github.com/GaudiLabs/OpenThereminV4 | Assembly/calibration step flow; GPL-3.0. |
| OpenTheremin V4 download / flash guide | https://www.gaudi.ch/OpenTheremin/index.php/download | Website-side firmware upload steps → our `tutorial/steps/04-install-firmware/`. |
| OpenTheremin V4 product page | https://www.gaudi.ch/OpenTheremin/ | Product context / feature list reference. |
| OpenTheremin V4 assembly instructions (PDF) | Local: `Instructions_OpenThereminV4.pdf` | 7-step photo assembly guide; mapped to tutorial steps 01–08. Not in GitHub repo. CC BY 4.0 (Urs Gaudenz, 2021). |
