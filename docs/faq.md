# FAQ

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

Answers restate the [build guide](../BuildYourOwn/README.md). Other questions are headings only.

## Do I need soldering, CAD, or a custom PCB?

Basic soldering is not required if the boards already have headers and the actuator leads are prepared. You need Arduino IDE familiarity and the ability to read a wiring diagram — no CAD or custom PCB work.

## Where are the parts, design files, and software?

Check the bill of materials before ordering or substituting parts: [`hardware/README.md`](../BuildYourOwn/hardware/README.md).

Design files named in the build guide:

- [`laser-cut/`](../BuildYourOwn/laser-cut/) (platform)
- [`cad/`](../BuildYourOwn/cad/) (sensing tube)
- [`hardware/electronic-wiring/`](../BuildYourOwn/hardware/electronic-wiring/) (electronics)
- [`hardware/tube-wiring/`](../BuildYourOwn/hardware/tube-wiring/) (tubing)

Software: [`software/`](../BuildYourOwn/software/).

## What tools does the build guide name?

- Laser cutter or cut-to-order service (`laser-cut/panel.svg` / `.dxf`; see the [laser-cut panel notes](../BuildYourOwn/laser-cut/README.md))
- Zip-tie/flush cutters
- Wire strippers, small screwdriver, and multimeter
- Soldering iron and solder only if headers or wire leads are not already fitted
- Computer with a data-capable micro-USB cable
- Phone/tablet or computer running **RheoData** for BLE control

## How is the version 1 panel assembled?

Follow the laser-cut panel notes to test-fit and cut the panel. Attach the four corner feet. Zip-tie each component per the placement map: PUMP1 and PUMP2 rotated 90° with VALVE2 centered between them; L298N #1 (both pumps) on the left; L298N #2 (VALVE2) on the right; MPRLS directly below VALVE2; and the ATtiny1616 seesaw, ESP32, and Button in the rear row, with the Button beside the ESP32. Version 1 has no PWR, VALVE1, or chamber mounting position. Confirm cable and tube clearance, tighten and trim the ties, then continue to wiring.

See [Step 01](../BuildYourOwn/README.md#step-01-assemble-the-platform).

## What has to be true before power is applied?

Leave micro-USB and the 12 V adapter disconnected. Wire every electrical connection as shown in the electronic-wiring notes. Confirm both L298N `5V-EN` jumpers are ON, their used ENA/ENB jumper caps are removed, and the unused L298N #2 Motor A channel remains NC. Confirm all grounds are common, each L298N +5 V output remains local to its own module, and no pump or valve is powered from the ESP32. Continuity-check the completed electrical wiring before applying power. Connect the pneumatic tubing as shown in the tube-wiring notes.

See [Step 02](../BuildYourOwn/README.md#step-02-connect-electronics-and-tubing).

## How does the power-on test proceed?

Place the panel on a stable surface; inspect tubing and leave slack in both power cables. Leave the 12 V adapter unplugged. Confirm ESP32 GND, both L298N GNDs, and the adapter (−) are tied together; confirm adapter (+) reaches both L298N motor power inputs. Power the ESP32 via micro-USB. Never back-feed 12 V into it. Open Serial Monitor at 115200 and confirm the MPRLS, Qwiic Button, and seesaw board are found (no "not found on Qwiic bus" or HAL-init error). With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/valve movement, excessive current draw, or heat. Disconnect immediately if any appears. Confirm the MPRLS reads near ambient, then connect RheoData to `2P1V_Adafruit`.

See [Step 04](../BuildYourOwn/README.md#step-04-power-on-test).

## Which sketch is uploaded?

Follow the toolchain, library, and upload instructions in the [firmware reference](../BuildYourOwn/software/README.md). Upload [`2P1V_Adafruit.ino`](../BuildYourOwn/software/2P1V_Adafruit/2P1V_Adafruit.ino) using a data-capable micro-USB cable. Open Serial Monitor at 115200 and confirm `2P1V_Adafruit initialized`.

See [Step 03](../BuildYourOwn/README.md#step-03-install-firmware).

## How do I calibrate and measure?

RheoBoard uses runtime BLE parameters rather than a one-shot calibration. Parameter definitions and defaults are maintained in the firmware reference.

Position the chamber/nozzle repeatably at the sample. Let the MPRLS stabilize, then run several REPs with no sample. Tune baseline, retract, extrude, ramp, and sampling parameters until a triad produces three repeatable traces. Measure the sample and confirm its pressure trace appears in RheoData. Use the firmware reference for BLE, button, serial, and debug controls. Save the verified settings and use the same fixture position for comparable measurements.

See [Step 05](../BuildYourOwn/README.md#step-05-calibrate-and-use).

## Where are the assembly packets?

TODO

## What tools are required beyond the build guide?

TODO

## What modifications are documented?

TODO

## How do I troubleshoot a build?

TODO

## Where is the logo?

TODO

## Who is acknowledged?

TODO

## Who sells a kit?

TODO

## Where is the community?

TODO

## How do I cite this project?

TODO

## Where is the assembly video?

TODO
