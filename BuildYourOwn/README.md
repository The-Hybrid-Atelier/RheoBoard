# Build Your Own Rheometer — Step-by-Step Guide

The [repo root README](../README.md) is the project overview. This file is the step-by-step build
guide, and it is the main copy: the wiki and `docs/` pages link here instead of repeating it. Each
linked subfolder owns its detailed reference information.

License: hardware CERN-OHL-W-2.0, firmware MIT, this guide CC BY-SA 4.0 — see
[repo root README](../README.md) → License, or [`../LICENSE`](../LICENSE).

<img src="images/teaser.jpg" alt="This simple rheometer — assembled bench prototype" width="480">

## Overview

This simple rheometer is a pneumatic retraction-extrusion system with 2 air pumps + 1 valve on a
3D-printed panel, driven by an ESP32 over BLE and sensed by a Qwiic MicroPressure sensor. There
are no laser-cut parts.

The sensing routine runs a REP (retraction-extrusion pulse) on command and streams a pressure trace.

Basic soldering is not required if the boards already have headers and the actuator leads are
prepared. You need Arduino IDE familiarity and the ability to read a wiring diagram — no CAD or
custom PCB work.

**Which version to build:** version 2, the 3D-printed panel in this guide, on
`main`. Version 2 is in progress; open items are marked TODO below. Version 1 is at the
[`v1` tag](https://github.com/The-Hybrid-Atelier/RheoBoard/tree/v1).

## Before you start

- **Parts:** the one parts list is [`hardware/README.md`](hardware/README.md) (parts to buy and
  parts to 3D print). Check it before ordering or substituting parts.
- **Design files:** [`cad/encloser/`](cad/encloser/) (3D-printed panel),
  [`cad/connector/`](cad/connector/) (sensing tube and small connector),
  [`hardware/electronic-wiring/`](hardware/electronic-wiring/) (electronics), and
  [`hardware/tube-wiring/`](hardware/tube-wiring/) (tubing)
- **Firmware and software:** [`../code/firmware/`](../code/firmware/) and [`../code/software/`](../code/software/).
  Both apply to this DIY build, the PCB, and the portable version.
- **Tools** (the one tools list, with where each extra tool is named:
  [`docs/assembly-tools.md`](../docs/assembly-tools.md)):
  - 3D printer (Bambu Lab, PLA, normal profile); Bambu Studio for the STEP files; a slicer or mesh
    viewer for the STL files
  - Flush cutters; wire strippers, small screwdriver, and multimeter; soldering iron + solder only
    if headers or wire leads are not already fitted
  - Computer with a data-capable micro-USB cable; Arduino IDE 2.x; git
  - Phone, tablet, or computer running **RheoData** for BLE control

## Power

Everyday wall power for this DIY build and for the PCB is a 12 V plug. On this DIY build it feeds
the pump and valve supplies. Pump and valve current does not go through the ESP32 5 V pin.

USB uploads firmware to the SparkFun ESP32 Thing Plus ([Step 03](#step-03-install-firmware)). After
upload, USB may be disconnected.

Portable power is a [SparkFun Lithium Ion Battery 1500 mAh, IEC62133 certified (SKU PRT-26059)](https://www.sparkfun.com/lithium-ion-battery-1500mah-iec62133-certified.html): nominal 3.7 V, 1500 mAh, terminated with a 2-pin JST-PH connector (2 mm pin spacing), with built-in protection, and it plugs into the ESP32 Thing Plus JST battery connector ([schematic](hardware/references/datasheets/ESP32_Thing_Plus_Schematic.pdf) V_BATT, 4.2 V maximum).

## Steps

- [Step 01 — Assemble the platform](#step-01-assemble-the-platform)
- [Step 02 — Connect electronics and tubing](#step-02-connect-electronics-and-tubing)
- [Step 03 — Install firmware](#step-03-install-firmware)
- [Step 04 — Power-on test](#step-04-power-on-test)
- [Step 05 — Calibrate and use](#step-05-calibrate-and-use)

---

## Step 01: Assemble the platform

The placement reference is the annotated panel photo
[`cad/encloser/panel-annotated.pdf`](cad/encloser/panel-annotated.pdf).

**You need:** PLA for the panel; the two air pumps, the valve, the pressure sensor, the ESP32
Thing Plus, the Qwiic Button, and the two L298N drivers from the parts list; a
Value Plastics FTLLB220-6005 fitting and [Adafruit 4661](https://www.adafruit.com/product/4661)
silicone tubing (1 m, 3 mm ID, 5 mm OD, for air only) for the sensing-tube test fit; 4 mm zip ties
(a count is not stated). Tools: a 3D printer; Bambu Studio for the STEP
files; a slicer for the STL files. TODO: the material and
print settings for the sensing tube and small connector.

### Print

1. Print the panel parts (the STEP files in
   [`cad/encloser/part/`](cad/encloser/part/)) in PLA with the normal Bambu profile, following
   [`cad/encloser/README.md`](cad/encloser/README.md). Bambu Studio opens STEP files directly.
   TODO: number of copies of each part, print orientation, and supports. The annotated photo
   does not label `part_04.STEP` or `part_05.STEP`.
2. Print the sensing tube [`sensing_tube.stl`](cad/connector/sensing_tube.stl) and the small
   connector [`connector_small.stl`](cad/connector/connector_small.stl) from
   [`cad/connector/`](cad/connector/). Import them at 100% scale (dimensions are in millimetres).
   These two files are not the probe.
   TODO: material, print settings, and number of copies for these two parts.
3. Test-print check, from [`cad/connector/README.md`](cad/connector/README.md): verify the thread,
   luer, tubing, airflow, and leak-tight fit with the real hardware. The sensing tube's luer is
   intended to mate with the FTLLB220-6005 fitting; confirm the fit with the actual fitting.
4. Exactly two models are still to be added, both TODO because the files are not in this
   repository: one GL45 two-port cap, and one tube that connects to that GL45 cap. The probe uses
   those parts with the Adafruit 4661 tube. [`sensing_tube.stl`](cad/connector/sensing_tube.stl)
   and [`connector_small.stl`](cad/connector/connector_small.stl) are not those parts.

File names, sizes, and likely roles are listed under "Print these" in
[`docs/parts-to-3d-print.md`](../docs/parts-to-3d-print.md).

### Mount

5. Place the labeled parts on the panel to match the annotated panel photo
   [`cad/encloser/panel-annotated.pdf`](cad/encloser/panel-annotated.pdf). In that photo, "Air pump"
   appears twice, once on each pump along the top; "Pressure sensor" points at the board between
   those pumps; "ESP32 Thing Plus" points at the board on the left; "Qwiic Button" points at the
   board on the right; "Valve" points at the part in the center; "L298N drivers" points at the two
   boards along the bottom. "3-D-printed panel" points at the flat panel. The callout does not say
   which L298N board is which. Hole positions are not labeled. The same list is on
   [`docs/panel-layout.md`](../docs/panel-layout.md).
6. Fasten with 4 mm zip ties. A count is not stated. The files stay in
   [`cad/encloser/`](cad/encloser/).
7. Confirm cable and tube clearance.

**You should now have:** the thread, luer, tubing, airflow, and leak-tight fit checked on the
printed sensing tube and small connector, and cable and tube clearance confirmed after the parts
are placed as in the annotated panel photo. TODO: which L298N board is the pump driver and which
is the valve driver.

**Next:** [Step 02 — Connect electronics and tubing](#step-02-connect-electronics-and-tubing).

---

## Step 02: Connect electronics and tubing

<a href="hardware/electronic-wiring/wiring-diagram.png"><img src="hardware/electronic-wiring/wiring-diagram.png" alt="Electronic wiring diagram" width="800"></a>

<a href="hardware/tube-wiring/tube-connection.png"><img src="hardware/tube-wiring/tube-connection.png" alt="Pneumatic tube-connection diagram" width="600"></a>

**You need:** 3 Qwiic cables; discrete point-to-point wire for the seesaw-to-driver signals
(TODO: wire type and gauge); the 12 V adapter, left unplugged;
[Adafruit 4661](https://www.adafruit.com/product/4661) silicone tubing (1 m, 3 mm ID, 5 mm OD, for
air only; TODO: length of each run); a tee for the sensor (TODO: the part); the printed sensing
tube, small connector, and FTLLB220-6005 fitting from Step 01. Tools: flush cutters; wire
strippers; small screwdriver;
multimeter; soldering iron and solder only if headers or wire leads are not already fitted.
TODO: a tool to cut the silicone tubing is not named in the repository.

### Wire the electronics

Leave micro-USB and the 12 V adapter disconnected. Each sub-step is one section of
[`hardware/electronic-wiring/README.md`](hardware/electronic-wiring/README.md). Wire every
connection exactly as shown there and in the diagram.

1. [Qwiic chain](hardware/electronic-wiring/README.md#qwiic-chain)
2. [Seesaw to the drivers](hardware/electronic-wiring/README.md#seesaw-to-the-drivers)
3. [L298N #1 (pumps)](hardware/electronic-wiring/README.md#l298n-1-pumps)
4. [L298N #2 (valve)](hardware/electronic-wiring/README.md#l298n-2-valve)
5. [Power](hardware/electronic-wiring/README.md#power). Do not plug the adapter in yet.
   TODO: how the 12 V adapter connects to the L298N inputs (connector or bare leads).
6. Confirm both L298N `5V-EN` jumpers are ON, their used ENA/ENB jumper caps are removed, and the
   unused L298N #2 Motor A channel remains NC.
7. Confirm all grounds are common, each L298N +5 V output remains local to its own module, and no
   pump or valve is powered from the ESP32.
8. Continuity-check the completed electrical wiring before applying power. TODO: which connections
   to check, and the expected result for each.

### Connect the tubing

9. Connect the pneumatic tubing exactly as shown in
   [`hardware/tube-wiring/README.md`](hardware/tube-wiring/README.md). TODO: how the sensor is
   teed into the shared line, and how the sensing tube, small connector, and FTLLB220-6005
   fitting join that line. TODO (maintainer): confirm whether the diagram's "chamber/nozzle" is
   the printed sensing tube.

**You should now have:** both `5V-EN` jumpers ON, the used ENA/ENB jumper caps removed, L298N #2
Motor A NC, all grounds common, each L298N +5 V output local to its own module, no pump or valve
powered from the ESP32, a continuity check done, and tubing connected as in the tube diagram.
Micro-USB and the 12 V adapter are still disconnected. TODO: a leak check, and a photo of the
finished wiring.

**Next:** [Step 03 — Install firmware](#step-03-install-firmware).

---

## Step 03: Install firmware

**You need:** a data-capable micro-USB cable. Tools: a computer with Arduino IDE 2.x and git
(toolchain and manual library install in [`../code/firmware/README.md`](../code/firmware/README.md#toolchain)).

1. Follow the toolchain, library, and upload instructions in
   [`../code/firmware/README.md`](../code/firmware/README.md#toolchain).
2. Upload [`2P1V_Adafruit.ino`](../code/firmware/2P1V_Adafruit/2P1V_Adafruit.ino) using a data-capable
   micro-USB cable.
3. Open Serial Monitor at 115200 and confirm `2P1V_Adafruit initialized`.
4. After upload, USB may be disconnected. Portable power is the battery on the JST
   connector described in [Power](#power).

**You should now see:** `2P1V_Adafruit initialized` in Serial Monitor at 115200.

**Next:** [Step 04 — Power-on test](#step-04-power-on-test).

---

## Step 04: Power-on test

**You need:** the 12 V adapter and the micro-USB cable. Tools: a computer with Serial Monitor; a
multimeter; a phone, tablet, or computer running RheoData. Download, pairing, the connected
indicator, and how to start and name a REP stay TODO in
[`../code/software/README.md`](../code/software/README.md) and the firmware notes. They will be specified later.

1. Place the unit on a stable surface; inspect tubing and leave slack in both power cables.
2. Leave the 12 V adapter unplugged. Confirm ESP32 GND, both L298N GNDs, and the adapter (−) are
   tied together; confirm adapter (+) reaches both L298N motor power inputs.
3. Power the ESP32 via micro-USB. Never back-feed 12 V into it.
4. Open Serial Monitor at 115200 and confirm the MPRLS, Qwiic Button, and seesaw board are
   found (no "not found on Qwiic bus" or HAL-init error).
5. With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/
   valve movement, excessive current draw, or heat. Disconnect immediately if any appears.
6. Confirm the MPRLS reads near ambient, then connect RheoData to `2P1V_Adafruit`. TODO: how to
   read the MPRLS at idle, and what value to expect. TODO: how to pair RheoData, and what it
   shows when it is connected
   ([`../code/software/README.md`](../code/software/README.md)).

**You should now see:** the MPRLS, Qwiic Button, and seesaw board found, with no "not found on
Qwiic bus" or HAL-init error, and no unexpected pump or valve movement, excessive current draw, or
heat after the 12 V adapter is plugged in.

**Next:** [Step 05 — Calibrate and use](#step-05-calibrate-and-use).

---

## Step 05: Calibrate and use

<img src="images/teaser.jpg" alt="Assembled RheoBoard bench prototype" width="480">

RheoBoard uses runtime BLE parameters rather than a one-shot calibration. Parameter definitions
and defaults are maintained in [`../code/firmware/README.md`](../code/firmware/README.md).

**You need:** a sample in a GL45 lab reagent bottle (recommended). A beaker, cup, or any other
fluid container is also fine. TODO: what to prepare, and the amount. Exactly two probe models
are still to be added, both TODO because the files are not in the repo: one GL45 two-port cap,
and one tube that connects to that GL45 cap (see Step 01). The probe also uses the Adafruit 4661
tube. The chamber/nozzle is held at one fixture position. Tools: a
phone, tablet, or computer running RheoData; a computer with Serial Monitor for the serial
controls in [`../code/firmware/README.md`](../code/firmware/README.md).

### Calibrate

1. Position the chamber/nozzle repeatably at the sample. TODO (maintainer): confirm whether the
   chamber/nozzle is the printed sensing tube.
2. Let the MPRLS stabilize, then run several REPs with no sample.
3. Tune baseline, retract, extrude, ramp, and sampling parameters until a triad produces three
   repeatable traces.

### Measure

4. Measure the sample and confirm its pressure trace appears in RheoData. TODO: how to prepare
   the specimen. TODO: how to start and name a REP, in
   [`../code/software/README.md`](../code/software/README.md). How to save a trial is still TODO.
5. Use [`../code/firmware/README.md`](../code/firmware/README.md) for BLE, button, serial, and debug controls.
6. Save the verified settings and use the same fixture position for comparable measurements.

**You should now see:** three repeatable traces from a triad, then the sample's pressure trace in
RheoData. TODO: an example of a good trace.

**Next:** Between samples, rinse the tube with water.
[`docs/maintenance.md`](../docs/maintenance.md) records that.
