# Build Your Own Rheometer — Step-by-Step Guide

The [repo root README](../README.md) is the project overview. This file is only the step-by-step
build guide; each linked subfolder owns its detailed reference information.

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

- **Materials:** [`hardware/README.md`](hardware/README.md)
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
- [Step 02 — Assemble the platform](#step-02-assemble-the-platform)
- [Step 03 — Wire the electronics](#step-03-wire-the-electronics)
- [Step 04 — Install firmware](#step-04-install-firmware)
- [Step 05 — Mount and set up](#step-05-mount-and-set-up) — blocked on RheoMap's fixture spec
- [Step 06 — Power and data connections](#step-06-power-and-data-connections)
- [Step 07 — Calibrate](#step-07-calibrate)
- [Step 08 — Ready to use](#step-08-ready-to-use)

---

## Step 01: Kit contents and tools

1. Unpack everything and check it against [`hardware/README.md`](hardware/README.md).
2. Resolve any missing or incompatible substitutions before assembly.
3. Gather the tools listed above. Do not start wiring until the BOM is fully accounted for.

---

## Step 02: Assemble the platform

1. Follow [`laser-cut/README.md`](laser-cut/README.md) to test-fit and cut the draft panel.
2. Attach the four corner feet and install the Ø10 bulkhead fitting at **CHAMBER**.
3. Zip-tie each component per
   [`laser-cut/panel-placement-map.png`](laser-cut/panel-placement-map.png): PUMP1/PUMP2 lying
   flat, VALVE2 (leave VALVE1 empty), MPRLS + Button next to the ESP32, both L298N boards clear of
   their heatsinks, ESP32, **ATtiny1616 seesaw** (right of ESP32 / above PWR — label `10 SEESAW`),
   power terminal block.
4. Confirm cable and tube clearance, tighten and trim the ties, then continue to wiring.

---

## Step 03: Wire the electronics

1. Leave micro-USB and the 12 V adapter disconnected.
2. Wire every electrical connection exactly as shown in
   [`hardware/electronic-wiring/README.md`](hardware/electronic-wiring/README.md).
3. Confirm both L298N `5V-EN` jumpers are ON, their used ENA/ENB jumper caps are removed, and the
   unused L298N #2 Motor A channel remains NC.
4. Confirm all grounds are common, each L298N +5 V output remains local to its own module, and no
   pump or valve is powered from the ESP32.
5. Continuity-check the completed electrical wiring before applying power.
6. Connect the pneumatic tubing exactly as shown in
   [`hardware/tube-wiring/README.md`](hardware/tube-wiring/README.md).

---

## Step 04: Install firmware

1. Follow the toolchain, library, and upload instructions in
   [`software/README.md`](software/README.md).
2. Upload [`2P1V_Adafruit.ino`](software/2P1V_Adafruit/2P1V_Adafruit.ino) using a data-capable
   micro-USB cable.
3. Open Serial Monitor at 115200 and confirm `2P1V_Adafruit initialized`.

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

---

## Step 06: Power and data connections

Parts: 12 V adapter (≥ 2 A), micro-USB cable — see [`hardware/README.md`](hardware/README.md).

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

RheoBoard uses runtime BLE parameters rather than a one-shot calibration. Parameter definitions
and defaults are maintained in [`software/README.md`](software/README.md).

1. Let the MPRLS stabilize, then run several REPs with no sample.
2. Adjust baseline time until the pre-pulse trace is stable.
3. Tune retract power/time for a clean pull without prolonged pump operation.
4. Tune extrude power/time and ramp for a controlled push.
5. Set the sampling rate for the required capture resolution.
6. Run a triad and confirm three repeatable traces before measuring samples.

---

## Step 08: Ready to use

1. Connect RheoData to BLE device `2P1V_Adafruit`.
2. Trigger a REP and confirm the pressure trace appears in the capture view.
3. Repeat the measurement and confirm the trace is stable enough for the intended experiment.
4. Use [`software/README.md`](software/README.md) for all BLE, button, serial, and debug controls.
5. Complete the human sign-off in [`VERIFICATION.md`](VERIFICATION.md) before treating the build
   as verified.
