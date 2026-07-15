# Build Your Own Rheometer — Step-by-Step Guide

Project overview and reference material (BOM, wiring, firmware API) live in the
[repo root README](../README.md); this file is the build guide.

License: hardware CERN-OHL-W-2.0, firmware MIT, this guide CC BY-SA 4.0 — see
[repo root README](../README.md) → License, or [`../LICENSE`](../LICENSE).

<img src="images/teaser.jpg" alt="This simple rheometer — assembled bench prototype" width="480">

## Overview

This simple rheometer is a pneumatic retraction-extrusion system with 2 air pumps + 1 valve on a
laser-cut acrylic panel, driven by an ESP32 over BLE and sensed by a Qwiic MicroPressure sensor.

The sensing routine runs a REP (retraction-extrusion pulse) on command and streams a pressure trace.

Basic soldering is not required if the boards already have headers and the actuator leads are
prepared. You need Arduino IDE familiarity and the ability to read a wiring diagram — no CAD or
custom PCB work.

## Before you start

- **Materials:** [`hardware/BOM.md`](hardware/BOM.md)
- **Design files:** [`laser-cut/`](laser-cut/) (platform),
  [`hardware/electronic-wiring/`](hardware/electronic-wiring/) (electronics), and
  [`hardware/tube-wiring/`](hardware/tube-wiring/) (tubing)
- **Software:** [`software/`](software/) (flash procedure is Step 04)
- **Tools:** laser cutter or cut-to-order service (`laser-cut/panel.svg`/`.dxf` — draft, not yet
  test-fit against real parts, see [`laser-cut/README.md`](laser-cut/README.md)); zip-tie/flush
  cutters; wire strippers, small screwdriver, and multimeter; soldering iron + solder only if
  headers or wire leads are not already fitted; computer with data-capable micro-USB cable;
  phone/tablet or computer running **RheoData** for BLE control

## Steps

- [Step 01 — Kit contents and tools](#step-01-kit-contents-and-tools)
- [Step 02 — Assemble the platform](#step-02-assemble-the-platform) — laser-cut vector file is a draft, not yet test-fit
- [Step 03 — Wire the electronics](#step-03-wire-the-electronics)
- [Step 04 — Install firmware](#step-04-install-firmware)
- [Step 05 — Mount and set up](#step-05-mount-and-set-up) — blocked on RheoMap's fixture spec
- [Step 06 — Power and data connections](#step-06-power-and-data-connections)
- [Step 07 — Calibrate](#step-07-calibrate)
- [Step 08 — Ready to use](#step-08-ready-to-use)

---

## Step 01: Kit contents and tools

1. Unpack everything and check it against [`hardware/BOM.md`](hardware/BOM.md).
2. Note anything missing or substituted in `PROGRESS.md` — don't silently substitute.
3. Gather the tools listed above.

**Tip:** don't start wiring until the BOM is fully accounted for.

---

## Step 02: Assemble the platform

Parts: acrylic panel, ~22 zip ties, Ø10 bulkhead fitting, 4 feet — see
[`hardware/BOM.md`](hardware/BOM.md). Design file: [`laser-cut/panel.svg`](laser-cut/panel.svg)
(draft vector cut file — not yet test-fit against real parts, see
[`laser-cut/README.md`](laser-cut/README.md) before cutting).

1. Cut the panel (290 × 200 × 3 mm acrylic) per
   [`laser-cut/panel.svg`](laser-cut/panel.svg) — test-fit real components against the geometry
   first (see the status note in `laser-cut/README.md`).
2. Attach the 4 corner feet.
3. Install the Ø10 bulkhead fitting at the CHAMBER position.
4. Zip-tie each component per
   [`laser-cut/panel-placement-map.png`](laser-cut/panel-placement-map.png): PUMP1/PUMP2 lying
   flat, VALVE2 (leave VALVE1 empty), MPRLS + Button next to the ESP32, both L298N boards clear of
   their heatsinks, ESP32, **ATtiny1616 seesaw** (right of ESP32 / above PWR — label `10 SEESAW`),
   power terminal block.
5. Don't wire anything yet — that's Step 03.

**Tip:** leave the VALVE1 zip-tie slot empty — reserved for a future 2-valve variant.

---

## Step 03: Wire the electronics

Parts: electronics rows in [`hardware/BOM.md`](hardware/BOM.md). Diagram:
[`hardware/electronic-wiring/wiring-diagram.png`](hardware/electronic-wiring/wiring-diagram.png) (electrical
only — pneumatic plumbing is separate, below).

1. Tie ESP32 GND, both L298N GNDs, and the 12 V adapter (−) together.
2. Connect the 12 V adapter (+) to both L298N motor power inputs. Never power pumps/valve from the
   ESP32 5 V pin.
3. **L298N #1 (pumps):** keep `5V-EN` ON and remove the ENA/ENB jumper caps. Use this module's
   local +5 V output: +5V→IN1/IN3; GND→IN2/IN4. ENA→seesaw pin `0`, ENB→seesaw pin `1`.
   OUT1/OUT2→PUMP1, OUT3/OUT4→PUMP2.
4. **L298N #2 (valve):** keep `5V-EN` ON and remove the ENA/ENB jumper caps. Leave the unused
   Motor A channel **NC**: ENA, IN1, IN2, and OUT1/OUT2. For Motor B, use this module's local
   +5 V output: +5V→IN3; GND→IN4; ENB→seesaw pin `5`; OUT3/OUT4→VALVE2. Seesaw pin `4` is
   reserved for a future VALVE1 channel but is NC here. Do not join the two modules' +5 V outputs
   or connect an external 5 V source while `5V-EN` is installed.
5. **Qwiic chain:** ESP32 → Qwiic Button (`0x6F`) → MicroPressure (`0x18`) → Adafruit ATtiny1616
   seesaw breakout (`0x49`), daisy-chained (order doesn't matter for I2C). The seesaw board's 3
   connected pins from steps 3–4 are **separate point-to-point wires to the L298N boards, not carried over
   Qwiic**. Qwiic supplies I2C, 3.3 V, and common GND to the seesaw board, so leave its separate
   `Vin` header pin NC; its `GND` pin belongs to the same common-ground net as step 1.
6. Continuity-check everything before applying 12 V.

⚠️ Earlier direct-GPIO builds needed 10 kΩ pull-downs on the ESP32 pins driving `ENA`/`ENB` to hold
them low at boot. That's no longer applicable to the (now unused) ESP32 pins, but this build's
source docs don't call out an equivalent for the seesaw board's own power-up state — unconfirmed,
watch for L298N output glitches when first powering up in Step 06.

Pin map must match
[`software/rheometer-firmware/PneumaticSystem.h`](software/rheometer-firmware/PneumaticSystem.h).

**Pneumatic plumbing:** plumb per
[`hardware/tube-wiring/`](hardware/tube-wiring/). PUMP1 port →
valve metal pole (vacuum); PUMP2 port → valve plastic pole (pressure) — motor polarity doesn't
flip air direction.

**Tip:** on each L298N, keep the regulator's `5V-EN` jumper ON, but remove the separate ENA/ENB
jumper caps so seesaw can control the enable pins.

---

## Step 04: Install firmware

Flash the BLE firmware:
[`software/rheometer-firmware/2P1V_Adafruit.ino`](software/rheometer-firmware/2P1V_Adafruit.ino).

1. Install [Arduino IDE](https://www.arduino.cc/en/software) 2.x.
2. Boards Manager: add the ESP32 URL from
   [`hardware/references/README.md`](hardware/references/README.md), install **esp32 by
   Espressif Systems**.
3. Library Manager: install SparkFun Qwiic Button, SparkFun MicroPressure, **Adafruit seesaw
   Library**, OSC (Adrian Freed).
4. Clone **ThingPlusBLEOSC** into Arduino `libraries/` (not on Library Manager):

   ```bash
   cd ~/Documents/Arduino/libraries
   git clone https://github.com/cearto/ThingPlusBLEOSC.git
   ```

5. Open `2P1V_Adafruit.ino`, select board **SparkFun ESP32 Thing Plus** (or **ESP32 Dev Module**)
   and the micro-USB port.
6. Upload. Open Serial Monitor @ 115200 — look for `2P1V_Adafruit initialized`.

**Tip:** use a data-capable micro-USB cable — charge-only cables won't show a serial port.

---

## Step 05: Mount and set up

1. Place the panel on a flat, stable, level surface.
2. Trace every pneumatic line for kinks, pinches, or tension.
3. Position the chamber/nozzle at your sample. **Exact fixture geometry is TBD** (depends on
   RheoMap's spec — see [`product-specs.md`](product-specs.md)); keep the position repeatable
   between runs for now.
4. Leave slack in the micro-USB and 12 V cables so plugging in doesn't stress the panel.
5. Keep drafts and heat sources away from the chamber — the REP's baseline phase is brief but
   sensitive to disturbance.

**Tip:** a kinked tube looks like a sensor problem — trace every line before powering on.

---

## Step 06: Power and data connections

Parts: 12 V adapter (≥ 2 A), micro-USB cable — see [`hardware/BOM.md`](hardware/BOM.md).

1. Leave the 12 V adapter unplugged. Confirm ESP32 GND, both L298N GNDs, and the adapter (−) are
   tied together; confirm adapter (+) reaches both L298N motor power inputs.
2. Power the ESP32 via micro-USB. Never back-feed 12 V into it.
3. Open Serial Monitor @ 115200 and confirm the MPRLS, required Qwiic Button, and seesaw board are
   found (no "not found on Qwiic bus" or HAL-init error).
4. With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/
   valve movement, excessive current draw, or heat. Disconnect immediately if any appears.
5. Confirm MPRLS reads near ambient, then connect RheoData to BLE device **`2P1V_Adafruit`**.

**Tip:** 12 V is the motor supply rail — actual drive to the ~4.5 V pumps / ~6 V valve is set by
firmware PWM (`rheo/rep/pull/power`, `push/power`), not adapter voltage.

---

## Step 07: Calibrate

RheoBoard DIY uses runtime BLE parameters, not a one-shot calibration.

1. Run a few idle REPs, or wait for MPRLS to stabilize.
2. Tune `rheo/rep/baseline/time` (default 420 ms) if ambient drift is visible.
3. Tune `rheo/rep/pull/power` (54%) and `pull/time` (315 ms) for a clean retract.
4. Tune `rheo/rep/push/power`, `push/time`, `push/ramp/start`, `push/ramp/time` (defaults 100%,
   345 ms, 40%, 300 ms) for extrude.
5. Set `rheo/sense/rate` (default 10 ms) to match your capture needs.
6. Trigger `rheo/rep/triad` and confirm 3 repeatable traces.

Full parameter list:
[`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**Tip:** if extrude feels too aggressive, lengthen `push/ramp/time` before shortening `push/time`.

---

## Step 08: Ready to use

Trigger a REP (retract → extrude → relax, 1500 ms total) four ways:

| Path | How |
|---|---|
| BLE | OSC `rheo/rep` |
| Onboard button | Press once (GPIO 0) |
| Qwiic Button | Single click |
| USB serial | Type `REP` |

The onboard LED (GPIO 13) lights for the duration.

**Other controls:** `rheo/rep/triad` (3 REPs back to back), `rheo/stop` / `STOP` (abort),
`rheo/purge` (clear the line), Qwiic double-click (latched vacuum), Qwiic hold (momentary blow),
`PUMP1 <pct>` / `PUMP2 <pct>` (serial bench debug), `rheo/api` (list commands). Full reference:
[`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**Read a measurement:** via RheoData, connect to `2P1V_Adafruit` and trigger `rheo/rep` — the
pressure trace shows in its capture view. Via serial, type `REP` and watch `#S,<ms>,<Pa>` samples
between `#REP_START`/`#REP_END`.

**Sign off:** run [`VERIFICATION.md`](VERIFICATION.md) and complete the human sign-off block.

**Tip:** the onboard button and Qwiic single-click do the exact same thing — use whichever's
within reach.

---

## Finished

_Fill in once there's a working build: final photos/video, FAQ, calibration notes._

## Tips (all steps, at a glance)

- **(01)** Check the full BOM before wiring.
- **(02)** Leave VALVE1 empty — reserved for a future 2-valve variant.
- **(03)** L298N ENA/ENB jumpers must be removed. Don't run pumps at 100% duty continuously
  (~50% max per Adafruit). ENA/ENB are wired from the seesaw board, not native ESP32 pins.
- **(04)** Use a data-capable micro-USB cable; Arduino Library Manager needs the Adafruit seesaw
  Library added alongside the usual SparkFun libraries.
- **(04)/(06)** MPRLS/Button/seesaw issues are usually a Qwiic cable, connector, or address
  problem, not the part itself; physical order along the I2C chain does not matter.
- **(06)** Never power motors from the ESP32 5 V pin.
- **(07)** Lengthen `push/ramp/time` before shortening `push/time` if extrude feels aggressive.
- **(08)** Onboard button, Qwiic Button, BLE, and serial all trigger the same REP — use
  whichever's convenient.
