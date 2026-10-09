# Build options

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

The DIY build comes first and the custom PCB comes second, matching [`README.md`](../README.md). Board details are in [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md).

## DIY

Build version 2 on `main` (3D-printed panel and enclosure). It is in progress, and [Step 01](../BuildYourOwn/README.md#step-01-assemble-the-platform) of the build guide has TODOs. Version 1 is at the [`v1` tag](https://github.com/The-Hybrid-Atelier/RheoBoard/tree/v1).

Start with the [step-by-step DIY build guide](../BuildYourOwn/README.md).

The DIY version uses two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs a REP (retraction-extrusion pulse) and streams the pressure trace.

Version 2 uses a 3D-printed panel and enclosure ([print files and notes](../BuildYourOwn/cad/encloser/README.md)). There are no laser-cut parts. The version 1 laser-cut panel, certified by OSHWA as US002865, is archived in [`BuildYourOwn/archive/laser-cut-v1/`](../BuildYourOwn/archive/laser-cut-v1/).

## Custom PCB

[`RheoBoard_PCB/`](../RheoBoard_PCB/) is a separate KiCad 10 board: schematic, layout, libraries, Gerbers, assembly BOM, and pick-and-place. The firmware in `BuildYourOwn/software/` drives the DIY module build. Read [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md) before ordering boards.

From that README: this is a separate board from the DIY module build in `BuildYourOwn/`. The ESP32 firmware in `BuildYourOwn/software/` drives that DIY rig. This PCB does not yet have its own sketch in the repository. Open `RheoBoard_v1/RheoboardV1/1.kicad_pro` in KiCad 10. The board is a 1.6 mm, two-copper-layer layout. This board is a prototype.
