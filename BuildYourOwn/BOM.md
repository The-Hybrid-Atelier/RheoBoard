# Bill of Materials — Build Your Own (2P1V rig)

Parts list for the **2 pumps + 1 valve** BYO bench rig. Wiring:
[`wiring/2P1V-wiring-diagram.png`](wiring/2P1V-wiring-diagram.png). Firmware:
[`software/2P1VX/`](software/2P1VX/).

| Part | Qty | Source/link | Datasheet | Notes |
|---|---|---|---|---|
| SparkFun ESP32 Thing Plus | 1 | https://www.sparkfun.com/sparkfun-esp32-thing-plus.html | https://cdn.sparkfun.com/assets/4/3/1/4/9/ESP32_Thing_Plus_Full_Hookup_Guide.pdf | MCU; Qwiic port for pressure sensor |
| SparkFun Qwiic MicroPressure (MPRLS) | 1 | https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html | https://cdn.sparkfun.com/assets/4/c/2/4/SparkFun_Qwiic_MicroPressure_Sensor_Hookup_Guide.pdf | Honeywell MPR series; I2C `0x18`; Qwiic cable to ESP32 |
| SparkFun Qwiic Button (optional) | 1 | https://www.sparkfun.com/sparkfun-qwiic-button.html | https://cdn.sparkfun.com/assets/6/6/2/0/3/Qwiic_Button_Hookup_Guide.pdf | 1-click=REP, 2-click=latched suck, hold=blow; firmware degrades gracefully if missing |
| L298N dual H-bridge module | 2 | https://www.amazon.com/s?k=L298N+motor+driver | https://www.st.com/resource/en/datasheet/l298.pdf | #1 = 2 pumps; #2 = valve; ENA/ENB jumpers **OFF** |
| Air pump / vacuum motor (Adafruit 4700, ZR320-02PM) | 2 | https://www.adafruit.com/product/4700 | [`references/datasheets/ZR320-02PM_4.5V.pdf`](references/datasheets/ZR320-02PM_4.5V.pdf) | ~4.5 V / ~600 mA each; PUMP1 = vacuum (retract), PUMP2 = pressure (extrude); **flow direction is fixed by port plumbing, not motor polarity** |
| 6 V air valve (Adafruit 4663, FA0520E) | 1 | https://www.adafruit.com/product/4663 | [`references/datasheets/4663_C14660_DC_6V.pdf`](references/datasheets/4663_C14660_DC_6V.pdf) | 3-port flip selector (VALVE2); metal pole = vacuum path, plastic pole = pressure path |
| Resistor 10 kΩ | 4 | — | — | Pull-down on GPIO 14, 15, 32, 33 to GND |
| DC power adapter 12 V | 1 | — | — | External supply for both L298N motor rails; ≥ 2 A recommended; share GND with ESP32 |
| Silicone tubing 3 mm ID | 1 | https://www.adafruit.com/product/4664 | — | Pneumatic plumbing; see `wiring/pneumatic-plumbing.md` |
| Qwiic cable | 1 | https://www.sparkfun.com/cables.html | — | ESP32 Thing Plus ↔ MicroPressure |
| USB cable (programming/power) | 1 | — | — | Flash `2P1VX.ino`; ESP32 Thing Plus USB-C |

## Notes

- Pumps are rated ~4.5–5 V; valves ~6 V. The rig runs from a **12 V adapter** into the L298N motor
  inputs and uses **PWM** (`rheo/rep/*` power levels) to limit effective drive — tune defaults in
  RheoData rather than running channels at 100% duty continuously. Adafruit rates the 4700 for ~50%
  duty cycle.
- When swapping a part, update this table, `wiring/` diagrams, and `software/2P1VX/PneumaticSystem.h`
  GPIO defines together.
- Record any substitution in `references.md` and the relevant exec-plan decision log.
- Laser-cut platform parts (if used) are not listed here yet — add rows when `laser-cut/` design
  files land.
