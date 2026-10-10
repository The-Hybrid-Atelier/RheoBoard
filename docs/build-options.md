# Build options

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

A builder starts on the wiki at [Build options](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Build-options). Each section below repeats that path's page. The separate pages stay: [DIY](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/DIY), [PCB](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/PCB), and [Portable](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Portable). Board details are in [`RheoBoard_PCB/README.md`](../RheoBoard_PCB/README.md).

## DIY

Build version 2, the flat 3D-printed panel. It is in progress. Open items are marked TODO in the assembly instructions. Version 1 is the archived laser-cut panel. There are no laser-cut parts. The version 1 laser-cut panel, certified by OSHWA as US002865, is archived in [`BuildYourOwn/archive/laser-cut-v1/`](../BuildYourOwn/archive/laser-cut-v1/). On the wiki it is in the Helpful group of the sidebar.

The DIY version uses two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs a REP (retraction-extrusion pulse) and streams the pressure trace.

<img src="img/panel/diy-annotated.jpg" width="480" alt="Annotated photo of the DIY panel">

**Next:** [Parts to buy](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Parts-to-buy). Those assembly steps are also in the [DIY build guide](../BuildYourOwn/README.md).

## PCB

This is a separate KiCad 10 board: schematic, layout, libraries, Gerbers, assembly BOM, and pick-and-place. Everyday wall power is a 12 V plug. The board is a 1.6 mm, two-copper-layer layout. It is a prototype.

<img src="../RheoBoard_PCB/images/pcb-v105-schematic-and-board.png" width="720" alt="v1.05 schematic on the left and v1.05 board on the right">

Left is the v1.05 schematic. Right is the v1.05 board.

**Next:** [BOM](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/BOM).

## Portable

The same firmware and RheoData notes apply. USB uploads firmware to the SparkFun ESP32 Thing Plus. After upload, USB may be disconnected. Portable power is a [SparkFun Lithium Ion Battery 1500 mAh, IEC62133 certified (PRT-26059)](https://www.sparkfun.com/lithium-ion-battery-1500mah-iec62133-certified.html): nominal 3.7 V, and it plugs into that board's JST battery connector (schematic V_BATT, 4.2 V maximum).

<img src="img/portable/portable-concept.jpg" width="480" alt="Packaging concept for a compact stack">

Packaging concept. Board, battery, and connector dimensions must be measured. There is no separate portable enclosure in the repository.

**Next:** [Portable parts to buy](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Portable-parts-to-buy).
