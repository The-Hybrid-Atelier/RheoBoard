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
| <img src="../images/qwiic-micropressure.jpg" width="100" alt="Qwiic MicroPressure"> | SparkFun Qwiic MicroPressure (MPRLS) | 1 | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | [`references/datasheets/Honeywell_MPR_Series_Datasheet.pdf`](references/datasheets/Honeywell_MPR_Series_Datasheet.pdf) | Honeywell MPR series; I2C `0x18`; daisy-chained on the shared Qwiic bus (device order does not matter) |
| <img src="../images/qwiic-button.jpg" width="100" alt="Qwiic Button"> | SparkFun Qwiic Button, red (BOB-15932) | 1 | https://www.sparkfun.com/sparkfun-qwiic-button.html | [`references/datasheets/Qwiic_Button_Schematic.pdf`](references/datasheets/Qwiic_Button_Schematic.pdf) | I2C, default address `0x6F`; 1-click=REP, hold=momentary pressure; daisy-chained on the same Qwiic bus |
| <img src="../images/adafruit-attiny1616-seesaw.jpg" width="100" alt="Adafruit ATtiny1616 Breakout with seesaw"> | Adafruit ATtiny1616 Breakout with seesaw, STEMMA QT/Qwiic (PID 5690) | 1 | https://www.adafruit.com/product/5690 | [Adafruit seesaw guide](https://learn.adafruit.com/adafruit-attiny817-seesaw) | I2C `0x49` (factory default, no jumpers to change); Qwiic supplies 3.3 V + common GND, so the separate `Vin` header pin is NC. GPIO pins `0`/`1` drive L298N #1 `ENA`/`ENB`, pin `5` drives L298N #2 `ENB`, and pin `4` is reserved/NC. These controls are discrete wires, not Qwiic — see `electronic-wiring/README.md`. Requires **Adafruit seesaw Library** (Arduino Library Manager). |
| <img src="../images/l298n-motor-driver.jpg" width="100" alt="L298N module"> | L298N dual H-bridge module | 2 | https://www.amazon.com/s?k=L298N+motor+driver | https://www.st.com/resource/en/datasheet/l298.pdf | #1 = 2 pumps (both channels used); #2 = valve (Motor A channel NC, Motor B used). On each module: `5V-EN` regulator jumper **ON**, ENA/ENB jumper caps **OFF**. Use each module's local +5 V output for its direction inputs; do not join the two +5 V outputs or apply external 5 V while `5V-EN` is installed. Used enable pins are driven by seesaw. |
| <img src="../images/adafruit-4700-air-pump.jpg" width="100" alt="Adafruit 4700 air pump"> | Air pump / vacuum motor (Adafruit 4700, ZR320-02PM) | 2 | https://www.adafruit.com/product/4700 | [`references/datasheets/ZR320-02PM_4.5V.pdf`](references/datasheets/ZR320-02PM_4.5V.pdf) | ~4.5 V / ~600 mA each; PUMP1 = vacuum (retract), PUMP2 = pressure (extrude); **flow direction is fixed by port plumbing, not motor polarity** |
| <img src="../images/adafruit-4663-air-valve.jpg" width="100" alt="Adafruit 4663 air valve"> | 6 V air valve (Adafruit 4663, FA0520E) | 1 | https://www.adafruit.com/product/4663 | [`references/datasheets/4663_C14660_DC_6V.pdf`](references/datasheets/4663_C14660_DC_6V.pdf) | 3-port flip selector (VALVE2); metal pole = vacuum path, plastic pole = pressure path |
| — | DC power adapter 12 V | 1 | — | — | External supply for both L298N motor rails; ≥ 2 A recommended; share GND with ESP32 |
| — | Silicone tubing 3 mm ID | 1 | https://www.adafruit.com/product/4664 | — | Pneumatic plumbing; see `tube-wiring/README.md` |
| — | Qwiic cables (×3) | 3 | https://www.sparkfun.com/cables.html | — | ESP32 → Button → MicroPressure → ATtiny1616 seesaw (daisy-chained on one I2C bus; order along the chain doesn't matter for I2C) |
| — | micro-USB cable | 1 | — | — | Flash `2P1V_Adafruit.ino`; also powers the ESP32 during upload/bench use |
| — | Acrylic panel, laser-cut (290 × 200 × 3 mm) | 1 | — | — | Mounting platform for every component; see [`../laser-cut/`](../laser-cut/) for cut file + placement map (draft vector file, not yet test-fit — see status there) |
| — | Zip ties, small (~2.5 mm wide) | ~22 | — | — | Every component is zip-tied to the panel, not screwed; see [`../laser-cut/README.md`](../laser-cut/README.md) for tie counts per part (includes 2 ties for the Rev B seesaw breakout) |
| — | Ø10 panel-mount bulkhead fitting | 1 | — | — | Chamber/nozzle mount point on the panel; pairs with the 3 mm ID tubing above |
| — | Rubber/plastic feet | 4 | — | — | Panel corner feet |

## Notes

- Pumps are rated ~4.5–5 V; valve ~6 V. The rig runs from a **12 V adapter** into the L298N motor
  inputs and uses **PWM** (`rheo/rep/*` power levels) to limit effective drive — tune defaults in
  RheoData rather than running channels at 100% duty continuously. Adafruit rates the 4700 for ~50%
  duty cycle.
- The L298N photo is a generic-module stand-in (no single canonical vendor page) — see
  [`../images/README.md`](../images/README.md) for its source/license. Swap it for
  a photo of your exact board if it looks different.
- When swapping a part, update this table, the `electronic-wiring/` and `tube-wiring/` diagrams,
  and
  `../software/2P1V_Adafruit/PneumaticSystem.h`
  pin defines together.
- Record any substitution in `references/README.md` and `PROGRESS.md`.
- **No pull-down resistors listed for the seesaw board's pins.** Earlier direct-GPIO builds needed
  10 kΩ pull-downs on the ESP32 pins driving L298N `ENA`/`ENB` to hold them low during boot, before
  firmware ran. With that signal path now on the seesaw board, those specific ESP32-side resistors
  no longer apply (the pins they protected are unused), and the source firmware/wiring docs for
  this build don't call out an equivalent for the seesaw board's own power-up state — unconfirmed,
  not verified against real hardware (see `AGENTS.md` → "What the agent can and can't verify").
  Flag this during Step 03/06 bring-up if the L298N outputs glitch on power-up.
