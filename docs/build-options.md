# Build options

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

The DIY build comes first and the custom PCB comes second, matching [`README.md`](../README.md). Board details are in [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md).

## DIY

Start with the [step-by-step DIY build guide](../BuildYourOwn/README.md).

The DIY version uses two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs a REP (retraction-extrusion pulse) and streams the pressure trace.

The current panel files are Rev C ([laser-cut panel](../BuildYourOwn/laser-cut/README.md)). A future enclosure concept — STEP exports and photos — is in [`BuildYourOwn/cad/encloser/`](../BuildYourOwn/cad/encloser/); the editable source and print files are not in the repo yet.

## Custom PCB

[`RheoBoard_PCB/`](../RheoBoard_PCB/) is a separate KiCad 10 board: schematic, layout, libraries, Gerbers, assembly BOM, and pick-and-place. The firmware in `BuildYourOwn/software/` drives the DIY module build. Read [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md) before ordering boards.

From that README: this is a separate board from the DIY module build in `BuildYourOwn/`. The ESP32 firmware in `BuildYourOwn/software/` drives that DIY rig. This PCB does not yet have its own sketch in the repository. Open `RheoBoard_v1/RheoboardV1/1.kicad_pro` in KiCad 10. The board is a 1.6 mm, two-copper-layer layout. This board is a prototype.
