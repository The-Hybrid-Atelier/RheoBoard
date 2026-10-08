# Build Your Own Rheometer — Step-by-Step Guide

The [repo root README](../README.md) is the project overview. This file is only the step-by-step
build guide; each linked subfolder owns its detailed reference information.

License: hardware CERN-OHL-W-2.0, firmware MIT, this guide CC BY-SA 4.0 — see
[repo root README](../README.md) → License, or [`../LICENSE`](../LICENSE).

<img src="images/teaser.jpg" alt="This simple rheometer — assembled bench prototype" width="480">

## Overview

This simple rheometer is a pneumatic retraction-extrusion system with 2 air pumps + 1 valve on a
3D-printed panel inside a 3D-printed enclosure, driven by an ESP32 over BLE and sensed by a Qwiic
MicroPressure sensor. There are no laser-cut parts.

The sensing routine runs a REP (retraction-extrusion pulse) on command and streams a pressure trace.

Basic soldering is not required if the boards already have headers and the actuator leads are
prepared. You need Arduino IDE familiarity and the ability to read a wiring diagram — no CAD or
custom PCB work.

## Before you start

- Check the [`hardware/README.md`](hardware/README.md) BOM before ordering or substituting parts.
- **Design files:** [`cad/encloser/`](cad/encloser/) (3D-printed panel and enclosure),
  [`cad/connector/`](cad/connector/) (sensing tube),
  [`hardware/electronic-wiring/`](hardware/electronic-wiring/) (electronics), and
  [`hardware/tube-wiring/`](hardware/tube-wiring/) (tubing)
- **Software:** [`software/`](software/)
- **Tools:** 3D printer (Bambu Lab, PLA, normal profile; see
  [`cad/encloser/README.md`](cad/encloser/README.md)); flush
  cutters; wire strippers, small screwdriver, and multimeter; soldering iron + solder only if
  headers or wire leads are not already fitted; computer with data-capable micro-USB cable;
  phone/tablet or computer running **RheoData** for BLE control

## Steps

- [Step 01 — Assemble the platform](#step-01-assemble-the-platform)
- [Step 02 — Connect electronics and tubing](#step-02-connect-electronics-and-tubing)
- [Step 03 — Install firmware](#step-03-install-firmware)
- [Step 04 — Power-on test](#step-04-power-on-test)
- [Step 05 — Calibrate and use](#step-05-calibrate-and-use)

---

## Step 01: Assemble the platform

<a href="cad/encloser/encloser_pic1.jpg"><img src="cad/encloser/encloser_pic1.jpg" alt="3D-printed panel and enclosure render" width="700"></a>

1. Print the panel, pump holders, bracket, enclosure body, and lid in PLA with the normal Bambu
   profile, following [`cad/encloser/README.md`](cad/encloser/README.md).
2. Mount the two pumps upright in the printed holders and the valve between them, as in the
   renders.
3. TODO: fastening hardware, and where the L298N drivers, MPRLS, seesaw, ESP32, and Button mount
   on the printed panel.
4. Confirm cable and tube clearance, then continue to wiring.

---

## Step 02: Connect electronics and tubing

<a href="hardware/electronic-wiring/wiring-diagram.png"><img src="hardware/electronic-wiring/wiring-diagram.png" alt="Electronic wiring diagram" width="800"></a>

<a href="hardware/tube-wiring/tube-connection.png"><img src="hardware/tube-wiring/tube-connection.png" alt="Pneumatic tube-connection diagram" width="600"></a>

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

## Step 03: Install firmware

1. Follow the toolchain, library, and upload instructions in
   [`software/README.md`](software/README.md).
2. Upload [`2P1V_Adafruit.ino`](software/2P1V_Adafruit/2P1V_Adafruit.ino) using a data-capable
   micro-USB cable.
3. Open Serial Monitor at 115200 and confirm `2P1V_Adafruit initialized`.

---

## Step 04: Power-on test

1. Place the unit on a stable surface; inspect tubing and leave slack in both power cables.
2. Leave the 12 V adapter unplugged. Confirm ESP32 GND, both L298N GNDs, and the adapter (−) are
   tied together; confirm adapter (+) reaches both L298N motor power inputs.
3. Power the ESP32 via micro-USB. Never back-feed 12 V into it.
4. Open Serial Monitor at 115200 and confirm the MPRLS, Qwiic Button, and seesaw board are
   found (no "not found on Qwiic bus" or HAL-init error).
5. With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/
   valve movement, excessive current draw, or heat. Disconnect immediately if any appears.
6. Confirm the MPRLS reads near ambient, then connect RheoData to `2P1V_Adafruit`.

---

## Step 05: Calibrate and use

<img src="images/teaser.jpg" alt="Assembled RheoBoard bench prototype" width="480">

RheoBoard uses runtime BLE parameters rather than a one-shot calibration. Parameter definitions
and defaults are maintained in [`software/README.md`](software/README.md).

1. Position the chamber/nozzle repeatably at the sample.
2. Let the MPRLS stabilize, then run several REPs with no sample.
3. Tune baseline, retract, extrude, ramp, and sampling parameters until a triad produces three
   repeatable traces.
4. Measure the sample and confirm its pressure trace appears in RheoData.
5. Use [`software/README.md`](software/README.md) for BLE, button, serial, and debug controls.
6. Save the verified settings and use the same fixture position for comparable measurements.
