# Build options

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

The DIY build comes first and the custom PCB comes second, matching [`README.md`](../README.md). Board details are in [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md).

## DIY

Build version 2 on `main` (3D-printed panel and enclosure). It is in progress, and [Step 01](../BuildYourOwn/README.md#step-01-assemble-the-platform) of the build guide has TODOs. Version 1 is at the [`v1` tag](https://github.com/The-Hybrid-Atelier/RheoBoard/tree/v1).

Start with the [step-by-step DIY build guide](../BuildYourOwn/README.md).

The DIY version uses two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs a REP (retraction-extrusion pulse) and streams the pressure trace.

Version 2 uses a 3D-printed panel and enclosure ([print files and notes](../BuildYourOwn/cad/encloser/README.md)). There are no laser-cut parts. The version 1 laser-cut panel, certified by OSHWA as US002865, is archived in [`BuildYourOwn/archive/laser-cut-v1/`](../BuildYourOwn/archive/laser-cut-v1/).

## Custom PCB

[`RheoBoard_PCB/`](../RheoBoard_PCB/) is a separate KiCad 10 board: schematic, layout, libraries, Gerbers, assembly BOM, and pick-and-place. Everyday wall power for this board is a 12 V plug. Read [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md) before ordering boards.

From that README: this is a separate board from the DIY module build in `BuildYourOwn/`. The firmware in [`firmware/`](../firmware/) and the RheoData notes in [`software/`](../software/) apply to the DIY build, this PCB, and the portable version. Open `RheoBoard_v1/RheoboardV1/1.kicad_pro` in KiCad 10. The board is a 1.6 mm, two-copper-layer layout. This board is a prototype.

## Portable

The same firmware and RheoData notes apply. USB uploads firmware to the SparkFun ESP32 Thing Plus. After upload, USB may be disconnected. Portable power is a single-cell LiPo on that board's JST connector. The vendored schematic says V_BATT should be a single-cell LiPo, 4.2 V maximum. TODO: the maintainer said 3–5 V; the datasheet maximum is 4.2 V. Confirm. No battery part number is specified.
