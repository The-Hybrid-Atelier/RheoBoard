# References

External hardware references and vendored datasheets used to select and verify the BOM.

| Component/Standard | Link | Notes |
|---|---|---|
| SparkFun ESP32 Thing Plus (micro-USB, WRL-15663) | https://www.sparkfun.com/sparkfun-esp32-thing-plus.html | MCU for this design's firmware; plain ESP32-WROOM-32D/E (not S2/S3); [hardware repo](https://github.com/sparkfun/ESP32_Thing_Plus); schematic/graphical datasheet vendored in `datasheets/` |
| SparkFun Qwiic MicroPressure (MPRLS) | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | I2C `0x18`; [hardware repo](https://github.com/sparkfun/MicroPressure_Sensor) |
| SparkFun Qwiic Button (red, BOB-15932) | https://www.sparkfun.com/sparkfun-qwiic-button.html | Default I2C `0x6F`; [hardware repo](https://github.com/sparkfun/Qwiic_Button) |
| Adafruit ATtiny1616 Breakout with seesaw, STEMMA QT/Qwiic (PID 5690) | https://www.adafruit.com/product/5690 | Default I2C `0x49`; [primary guide](https://learn.adafruit.com/adafruit-attiny817-seesaw) |
| Adafruit 4699 air pump / vacuum (ZR370-02PM) | https://www.adafruit.com/product/4699 | ~4.5 V, 2.5 LPM; 58.2 × Ø27.0 mm nominal; flow fixed by port plumbing; datasheet in `datasheets/ZR370-02PM_4.5V.pdf`; user-confirmed drawing in `ZR370-02PM_dimensions.png` |
| Adafruit 4663 air valve (FA0520E) | https://www.adafruit.com/product/4663 | 6 V 3-port flip valve; datasheet in `datasheets/4663_C14660_DC_6V.pdf` |
| Adafruit silicone tubing 3 mm ID | https://www.adafruit.com/product/4664 | For 4699 pumps and 4663 valve ports |
| Honeywell MPR series datasheet | vendored: `datasheets/Honeywell_MPR_Series_Datasheet.pdf` | Source: https://cdn.sparkfun.com/assets/2/e/8/0/9/honeywell-mpr-datasheet.pdf |
| ESP32 Thing Plus schematic | vendored: `datasheets/ESP32_Thing_Plus_Schematic.pdf` | SparkFun v2.0 Eagle schematic export |
| ESP32 Thing Plus graphical datasheet | vendored: `datasheets/ESP32_Thing_Plus_Graphical_Datasheet.pdf` | Pinout diagram, power specs |
| Qwiic Button schematic | vendored: `datasheets/Qwiic_Button_Schematic.pdf` | SparkFun Eagle schematic export |
| L298N (ST) | https://www.st.com/resource/en/datasheet/l298.pdf | Dual H-bridge IC datasheet (generic module, no single canonical vendor page) |
