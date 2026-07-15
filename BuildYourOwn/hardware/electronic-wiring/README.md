# Electronic wiring

**This simple rheometer** — 2 pumps, 1 valve (switched-port "flip" plumbing). Firmware:
[`../../software/`](../../software/) (`2P1V_Adafruit.ino`).

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

<a href="wiring-diagram.png"><img src="wiring-diagram.png" alt="Electrical wiring diagram" width="800"></a>

Summary (see diagram for full detail):

- **MCU:** SparkFun ESP32 Thing Plus — single Qwiic (I2C) connector, daisy-chained to the Qwiic
  Button (`0x6F`), MPRLS pressure sensor (`0x18`), and an **Adafruit ATtiny1616 Breakout with
  seesaw** (`0x49`). Order along the chain doesn't matter for I2C. Qwiic supplies the seesaw
  board's 3.3 V and common GND, so its separate `Vin` header pin is **NC**; its `GND` pin is the
  same common-ground net shown throughout the diagram.
- **Adafruit ATtiny1616 (seesaw)** sits between the ESP32 and the two L298N drivers: its pins
  `0`/`1` (PWM) → L298N #1 `ENA`/`ENB` (pumps), and pin `5` (digital) → L298N #2 `ENB`
  (valve). Pin `4` is reserved for a future VALVE1 channel but is **NC** in this build.
  **These 3 control signals are discrete point-to-point wires, not carried over Qwiic** — Qwiic
  only carries I2C commands + 3.3V logic to the seesaw board itself.
- **L298N #1 (pumps):** keep the `5V-EN` regulator jumper ON; remove the ENA/ENB jumper caps.
  Use that module's local +5 V output for IN1/IN3, and connect IN2/IN4 to GND (direction
  hardwired). OUT1/2 → PUMP1 (vacuum), OUT3/4 → PUMP2 (pressure).
- **L298N #2 (valve):** keep `5V-EN` ON and remove the ENA/ENB jumper caps. Motor A is unused:
  ENA, IN1, IN2, and OUT1/OUT2 are
  **NC**. Motor B drives the only valve: ENB → seesaw pin `5`, IN3 → +5V, IN4 → GND, and
  OUT3/OUT4 → VALVE2. Keep each module's +5 V output local; do not tie the two outputs together
  or feed external 5 V while `5V-EN` is installed.
- **Power:** external **12 V DC adapter** (≥ 2 A recommended) to both L298N motor power inputs;
  common GND with ESP32. PWM on ENA/ENB limits effective voltage to pumps (~4.5 V rated) and valve
  (~6 V rated) — see [`../README.md`](../README.md) notes.

Pin map matches `PneumaticSystem.h` in firmware: `SS_PUMP1_EN=0`, `SS_PUMP2_EN=1`, and
`SS_VALVE2_EN=5`. `SS_VALVE1_EN=4` is reserved in firmware but physically **NC** in this
single-valve build (all are seesaw pins, not native ESP32 GPIO).

⚠️ **Open question, not verified against real hardware:** earlier direct-GPIO builds needed 10 kΩ
pull-downs on the ESP32 pins driving `ENA`/`ENB` to hold them low during boot. With that signal
path now on the seesaw board, those specific resistors no longer apply — but this build's source
docs don't call out an equivalent for the seesaw board's own power-up state. Watch for L298N
output glitches on power-up during Step 03/06 bring-up; see `hardware/README.md` notes.

## Component photos

See [`../README.md`](../README.md) and [`../../images/`](../../images/) for a photo of
every part in this diagram (ESP32 Thing Plus, MPRLS sensor, Qwiic Button, Adafruit ATtiny1616
seesaw breakout, L298N, Adafruit 4700 pumps, Adafruit 4663 valve).

## Conventions

- Electronic wiring is drawn as a labeled schematic with component blocks, named pins,
  orthogonal wire routing, junction dots, and a net-color legend.
- When updating wiring, update the image **and** the pin defines in
  `../../software/2P1V_Adafruit/PneumaticSystem.h`
  together — keep them in sync.
- A photo of the finished build from the same angle as the diagram is a useful supplement.

## Regenerating the electrical diagram

`wiring-diagram.png` is a rendered export — [`generate_wiring_diagram.py`](generate_wiring_diagram.py)
is the actual editable source (requires `matplotlib`; `Pillow` optional, used to shrink the PNG).
Edit the script, not the PNG directly:

```bash
python3 generate_wiring_diagram.py
```
