# Electronic wiring

**This simple rheometer** — 2 pumps, 1 valve (switched-port "flip" plumbing). Firmware:
[`../../../code/firmware/2P1V_Adafruit/2P1V_Adafruit.ino`](../../../code/firmware/2P1V_Adafruit/2P1V_Adafruit.ino).
The same firmware applies to RheoBoardOTS - DIY, RheoBoard_v1 - PCB, and RheoBoardPipette - Portable.

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

<a href="wiring-diagram.png"><img src="wiring-diagram.png" alt="Electrical wiring diagram" width="800"></a>

## Wiring steps

Wire with micro-USB and the 12 V adapter disconnected.
[Step 02 of the build guide](../../README.md#step-02-connect-electronics-and-tubing)
follows these sections. See the diagram for full detail.

**You need:** 3 Qwiic cables; discrete point-to-point wire (TODO: type and gauge); the 12 V
adapter, left unplugged. Tools: wire strippers, a small screwdriver, and a multimeter; a soldering
iron and solder only if headers or wire leads are not already fitted. Lists:
[`../README.md`](../README.md) (parts) and
[`../../../docs/assembly-tools.md`](../../../docs/assembly-tools.md) (tools).

<a id="qwiic-chain"></a>

### Qwiic chain

**MCU:** SparkFun ESP32 Thing Plus — single Qwiic (I2C) connector, daisy-chained to the Qwiic
Button (`0x6F`), MPRLS pressure sensor (`0x18`), and an **Adafruit ATtiny1616 Breakout with
seesaw** (`0x49`). Order along the chain doesn't matter for I2C. Qwiic supplies the seesaw
board's 3.3 V and common GND, so its separate `Vin` header pin is **NC**; its `GND` pin is the
same common-ground net shown throughout the diagram.

<a id="seesaw-to-the-drivers"></a>

### Seesaw to the drivers

**Adafruit ATtiny1616 (seesaw)** sits between the ESP32 and the two L298N drivers: its pins
`0`/`1` (PWM) → L298N #1 `ENA`/`ENB` (pumps), and pin `5` (digital) → L298N #2 `ENB`
(valve). Pin `4` is reserved for a future VALVE1 channel but is **NC** in this build.
**These 3 control signals are discrete point-to-point wires, not carried over Qwiic** — Qwiic
only carries I2C commands + 3.3V logic to the seesaw board itself.

<a id="l298n-1-pumps"></a>

### L298N #1 (pumps)

Keep the `5V-EN` regulator jumper ON; remove the ENA/ENB jumper caps. Use that module's local
+5 V output for IN1/IN3, and connect IN2/IN4 to GND (direction hardwired). OUT1/2 → PUMP1
(vacuum), OUT3/4 → PUMP2 (pressure).

<a id="l298n-2-valve"></a>

### L298N #2 (valve)

Keep `5V-EN` ON and remove the ENA/ENB jumper caps. Motor A is unused: ENA, IN1, IN2, and
OUT1/OUT2 are **NC**. Motor B drives the only valve: ENB → seesaw pin `5`, IN3 → +5V, IN4 → GND,
and OUT3/OUT4 → VALVE2. Keep each module's +5 V output local; do not tie the two outputs together
or feed external 5 V while `5V-EN` is installed.

<a id="power"></a>

### Power

External **12 V DC adapter** (≥ 2 A recommended) to both L298N motor power inputs; common GND
with ESP32. That 12 V plug is everyday wall power for RheoBoardOTS - DIY and for RheoBoard_v1 - PCB. Pump and
valve current does not go through the ESP32 5 V pin. PWM on ENA/ENB limits effective voltage to
pumps (~4.5 V rated) and valve (~6 V rated). The pump manufacturer's intermittent-duty
recommendation limits run time, not the instantaneous PWM setting; see
[`../README.md`](../README.md). Do not plug the adapter in until
[Step 04](../../README.md#step-04-power-on-test). TODO: how the adapter connects to the L298N
inputs (connector or bare leads).

USB uploads firmware to the ESP32 Thing Plus. After upload, USB may be disconnected. Portable
power is the SparkFun PRT-26059 named in [Power](../../README.md#power) (nominal 3.7 V). It plugs
into that board's JST battery connector (schematic V_BATT, 4.2 V maximum).

**You should now have:** both `5V-EN` jumpers ON, the used ENA/ENB jumper caps removed, L298N #2
Motor A NC, all grounds common, and each L298N +5 V output local to its own module. Then
continuity-check the wiring (TODO: which connections to check, and the expected result for each).

**Next:** connect the tubing ([`../tube-wiring/README.md`](../tube-wiring/README.md)).

Pin map matches `PneumaticSystem.h` in firmware: `SS_PUMP1_EN=0`, `SS_PUMP2_EN=1`, and
`SS_VALVE2_EN=5`. `SS_VALVE1_EN=4` is reserved in firmware but physically **NC** in this
single-valve build (all are seesaw pins, not native ESP32 GPIO).

The earlier direct-GPIO build's 10 kΩ pull-down resistors are not used with the seesaw-controlled
signal path.

## Conventions

- Electronic wiring is drawn as a labeled schematic with component blocks, named pins,
  orthogonal wire routing, junction dots, and a net-color legend.
- When updating wiring, update the image **and** the pin defines in
  `../../../code/firmware/2P1V_Adafruit/PneumaticSystem.h`
  together — keep them in sync.

## Regenerating the electrical diagram

`wiring-diagram.png` is a rendered export — [`generate_wiring_diagram.py`](generate_wiring_diagram.py)
is the actual editable source (requires `matplotlib`; `Pillow` optional, used to shrink the PNG).
Edit the script, not the PNG directly:

```bash
python3 generate_wiring_diagram.py
```
