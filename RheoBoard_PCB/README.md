# RheoBoard PCB

KiCad 10 design for the custom RheoBoard. This is a separate board from the DIY module build in
[`BuildYourOwn/`](../BuildYourOwn/). The ESP32 firmware in
[`BuildYourOwn/software/`](../BuildYourOwn/software/) drives that DIY rig. This PCB does not yet
have its own sketch in the repository.

License: hardware CERN-OHL-W-2.0 — see [`../LICENSE`](../LICENSE).

## Open the design

Open [`RheoBoard_v1/RheoboardV1/1.kicad_pro`](RheoBoard_v1/RheoboardV1/1.kicad_pro) in KiCad 10.

- [`1.kicad_sch`](RheoBoard_v1/RheoboardV1/1.kicad_sch) is the root schematic.
- [`1.kicad_pcb`](RheoBoard_v1/RheoboardV1/1.kicad_pcb) is the board. It is a 1.6 mm, two-copper-layer layout.
- Symbol, footprint, and 3D sources sit beside the project in
  [`RheoBoard_v1/Symbol/`](RheoBoard_v1/Symbol/),
  [`RheoBoard_v1/Footprint/`](RheoBoard_v1/Footprint/), and
  [`RheoBoard_v1/3D/`](RheoBoard_v1/3D/).

The other `.kicad_sch` files in `RheoboardV1/` are separate block drawings (power, MCU,
peripheral, MPRLS, and the TCA9534 switch). They are not hierarchical sheets of `1.kicad_pro`.
`3_MICROPROCESSOR.kicad_sch` and `Switch TCA9534.kicad_sch` are empty stubs.

## Manufacturing files

These outputs match the current KiCad board. Re-export them from `1.kicad_pcb` after any
layout change.

| Output | Path |
|---|---|
| Gerbers | [`RheoBoard_v1/Gerber/`](RheoBoard_v1/Gerber/) |
| Drills | [`RheoBoard_v1/Drill/`](RheoBoard_v1/Drill/) |
| Assembly BOM | [`BOM/BOM_RheoboardV1_JLCSMT.xlsx`](BOM/BOM_RheoboardV1_JLCSMT.xlsx) |
| Pick-and-place | [`RheoBoard_v1/CPL/CPL.csv`](RheoBoard_v1/CPL/CPL.csv) |

This board is a prototype.
