# Step 03: Wire the electronics

- **Time:** ~2–4 h (breadboard/perfboard, first build)
- **Difficulty:** moderate (soldering pull-downs; L298N screw terminals)

## What you'll need for this step

- **Parts:** electronics rows in [`../../../BOM.md`](../../../BOM.md).
- **Design files:** [`../../../wiring/2P1V-wiring-diagram.png`](../../../wiring/2P1V-wiring-diagram.png) —
  follow section **A — Electrical Wiring**.
- **Tools:** soldering iron, wire strippers, multimeter, small screwdriver (L298N terminals)

## Instructions

1. **Common GND:** tie ESP32 GND, both L298N GND pins, and 12 V adapter (−) together.
2. **Pull-downs:** solder 10 kΩ from GPIO **14, 15, 32, 33** each to GND on the ESP32.
3. **12 V motor power:** connect adapter (+) to both L298N motor power inputs (+12V / VCC pins per
   module label). Do **not** power pumps or valve from the ESP32 5 V pin.
4. **L298N #1 (pumps):** remove ENA and ENB jumpers. Loop IN1→+5V, IN3→+5V; IN2→GND, IN4→GND.
   Connect ENA→GPIO 32, ENB→GPIO 33. OUT1/OUT2→PUMP1 (4700), OUT3/OUT4→PUMP2 (4700).
5. **L298N #2 (valve):** same direction wiring on the active channel. ENA→GPIO 14 (VALVE2).
   OUT1/OUT2→VALVE2 (4663). ENB/GPIO 15/OUT3/OUT4 may stay unwired.
6. **Qwiic:** [MicroPressure sensor](https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html)
   to ESP32 Thing Plus Qwiic port (I2C address `0x18`).
7. Optional: Qwiic Button on the same bus.
8. Continuity-check new connections **before** applying 12 V (see
   [`../../../VERIFICATION.md`](../../../VERIFICATION.md)).

GPIO map must match [`../../../software/2P1VX/PneumaticSystem.h`](../../../software/2P1VX/PneumaticSystem.h).

## Pneumatic plumbing (same step or next)

After electrical wiring, plumb per [`../../../wiring/2P1V-tube-connection.png`](../../../wiring/2P1V-tube-connection.png)
and [`../../../wiring/pneumatic-plumbing.md`](../../../wiring/pneumatic-plumbing.md).

**4700 port orientation matters:** PUMP1 side port → valve metal pole (vacuum); PUMP2 tubing port →
valve plastic pole (pressure). Motor polarity does not flip air direction.

## Media

Diagram reference + photo of actual wiring from the same angle as the diagram.

## Tips / common mistakes

- L298N ENA/ENB jumpers must be **OFF** when using PWM from the ESP32.
- Pumps are ~4.5 V parts on a 12 V rail — use firmware PWM defaults; avoid 100% duty for long runs
  (Adafruit recommends ~50% duty for the 4700).
- Any deviation from the published diagram must be documented (update `wiring/` or note in
  exec-plan).

## Check before moving on

- [ ] Wiring matches `wiring/2P1V-wiring-diagram.png`
- [ ] Pneumatic plumbing matches `wiring/2P1V-tube-connection.png` (4700 port orientation)
- [ ] Continuity/short check passed (12 V not applied yet)
