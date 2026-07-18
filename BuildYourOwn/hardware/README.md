# Hardware and bill of materials

Parts list for the **2 pumps + 1 valve** DIY bench rig. Wiring:
[`electronic-wiring/wiring-diagram.png`](electronic-wiring/wiring-diagram.png) and
[`tube-wiring/`](tube-wiring/). Firmware:
[`../software/`](../software/). Component photo sources/licenses:
[`../images/README.md`](../images/README.md).

License: CERN-OHL-W-2.0 — see [`../../LICENSE`](../../LICENSE).

| Photo | Part | Qty | Source/link | Datasheet | Notes |
|---|---|---|---|---|---|
| <img src="../images/esp32-thing-plus.jpg" width="100" alt="ESP32 Thing Plus"> | SparkFun ESP32 Thing Plus (micro-USB, WRL-15663) | 1 | https://www.sparkfun.com/sparkfun-esp32-thing-plus.html | [`references/datasheets/ESP32_Thing_Plus_Schematic.pdf`](references/datasheets/ESP32_Thing_Plus_Schematic.pdf), [`ESP32_Thing_Plus_Graphical_Datasheet.pdf`](references/datasheets/ESP32_Thing_Plus_Graphical_Datasheet.pdf) | MCU; plain ESP32-WROOM-32D/E (not S2/S3); powers + programs over micro-USB; Qwiic port for sensor chain |
| <img src="../images/qwiic-micropressure.jpg" width="100" alt="Qwiic MicroPressure"> | SparkFun Qwiic MicroPressure (MPRLS) | 1 | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | [`references/datasheets/Honeywell_MPR_Series_Datasheet.pdf`](references/datasheets/Honeywell_MPR_Series_Datasheet.pdf) | I2C `0x18`; Qwiic |
| <img src="../images/qwiic-button.jpg" width="100" alt="Qwiic Button"> | SparkFun Qwiic Button, red (BOB-15932) | 1 | https://www.sparkfun.com/sparkfun-qwiic-button.html | [`references/datasheets/Qwiic_Button_Schematic.pdf`](references/datasheets/Qwiic_Button_Schematic.pdf) | I2C `0x6F`; Qwiic |
| <img src="../images/adafruit-attiny1616-seesaw.jpg" width="100" alt="Adafruit ATtiny1616 Breakout with seesaw"> | Adafruit ATtiny1616 Breakout with seesaw, STEMMA QT/Qwiic (PID 5690) | 1 | https://www.adafruit.com/product/5690 | [Adafruit seesaw guide](https://learn.adafruit.com/adafruit-attiny817-seesaw) | I2C `0x49`; 3.3 V Qwiic logic; PWM/GPIO output |
| <img src="../images/l298n-motor-driver.jpg" width="100" alt="L298N module"> | L298N dual H-bridge module | 2 | https://www.amazon.com/s?k=L298N+motor+driver | https://www.st.com/resource/en/datasheet/l298.pdf | #1 drives two pumps; #2 drives one valve |
| <img src="../images/adafruit-4699-air-pump.jpg" width="100" alt="Adafruit 4699 air pump"> | Air pump / vacuum motor (Adafruit 4699, ZR370-02PM) | 2 | https://www.adafruit.com/product/4699 | [`references/datasheets/ZR370-02PM_4.5V.pdf`](references/datasheets/ZR370-02PM_4.5V.pdf) | ~4.5 V / ~500 mA each; 2.5 LPM; 58.2 × Ø27.0 mm nominal |
| <img src="../images/adafruit-4663-air-valve.jpg" width="100" alt="Adafruit 4663 air valve"> | 6 V air valve (Adafruit 4663, FA0520E) | 1 | https://www.adafruit.com/product/4663 | [`references/datasheets/4663_C14660_DC_6V.pdf`](references/datasheets/4663_C14660_DC_6V.pdf) | 3-port flip selector |
| — | DC power adapter 12 V | 1 | — | — | External supply for both L298N motor rails; ≥ 2 A recommended; share GND with ESP32 |
| — | Silicone tubing 3 mm ID | 1 | https://www.adafruit.com/product/4664 | — | Pneumatic plumbing; see `tube-wiring/README.md` |
| — | Qwiic cables | 3 | https://www.sparkfun.com/cables.html | — | Four Qwiic boards daisy-chained on one I2C bus |
| — | micro-USB cable | 1 | — | — | Flash `2P1V_Adafruit.ino`; also powers the ESP32 during upload/bench use |
| — | Acrylic panel, laser-cut (230 × 200 × 3 mm) | 1 | — | — | See [`../laser-cut/`](../laser-cut/) |
| — | Zip ties, small (~2.5 mm wide) | ~18 | — | — | Component mounting |
| — | Rubber/plastic feet | 4 | — | — | Panel corner feet |

## Notes

- Pumps are rated ~4.5–5 V and the valve ~6 V. Adafruit's ~50% pump duty-cycle recommendation
  describes intermittent run time, not a 50% PWM ceiling; firmware may use brief higher-PWM pulses,
  but the pumps should not run continuously.
- The L298N photo is a generic-module stand-in (no single canonical vendor page) — see
  [`../images/README.md`](../images/README.md) for its source/license. Swap it for
  a photo of your exact board if it looks different.
- Before substituting a part, confirm its electrical, pneumatic, mechanical, and firmware
  compatibility against the linked folder documentation.
