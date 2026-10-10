# RheoBoard_v1 - PCB

KiCad 10 design for the custom RheoBoard. This is a separate board from RheoBoardOTS - DIY in
[`BuildYourOwn/`](../BuildYourOwn/). Everyday wall power for this board is a 12 V plug. The
firmware in [`../code/firmware/`](../code/firmware/) and the RheoData notes in [`../code/software/`](../code/software/)
apply to RheoBoardOTS - DIY, RheoBoard_v1 - PCB, and RheoBoardPipette - Portable.

License: hardware CERN-OHL-W-2.0 — see [`../LICENSE`](../LICENSE).

## Open the design

Open [`RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pro`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pro) in KiCad 10.

- [`RheoBoard_v1.kicad_sch`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_sch) is the root schematic.
- [`RheoBoard_v1.kicad_pcb`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pcb) is the board. It is a 1.6 mm, two-copper-layer layout.
- Symbol, footprint, and 3D sources sit beside the project in
  [`RheoBoard_v1/Symbol/`](RheoBoard_v1/Symbol/),
  [`RheoBoard_v1/Footprint/`](RheoBoard_v1/Footprint/), and
  [`RheoBoard_v1/3D/`](RheoBoard_v1/3D/).

The other `.kicad_sch` files in `RheoboardV1/` are separate block drawings (power, MCU,
peripheral, MPRLS, and the TCA9534 switch). They are not hierarchical sheets of `RheoBoard_v1.kicad_pro`.
`3_MICROPROCESSOR.kicad_sch` and `Switch TCA9534.kicad_sch` are empty stubs.

Both projects now use versioned main filenames: `RheoBoard_v1.*` and `RheoBoard_v1.05.*`. The v1.05 project is [here](RheoBoard_v1.05/RheoboardV1/RheoBoard_v1.05.kicad_pro). On 2026-10-10, the project-instance references were renamed with the files; all circuit connections, PCB bytes and footprint geometry were preserved. Both copies of the mismatched `UCC27511A-Q1 .kicad_mod` filename are now `DBV0006A_N.kicad_mod`, matching their existing internal footprint name. Other footprint filenames already match their internal names. [Rename validation](RheoBoard_v1.05/Verification/Rename_2026-10-10/validation.json) covers both versions.

## Manufacturing files

These outputs match the current KiCad board. Re-export them from `RheoBoard_v1.kicad_pcb` after any
layout change.

| Output | Path |
|---|---|
| Gerbers | [`RheoBoard_v1/Gerber/`](RheoBoard_v1/Gerber/) |
| Drills | [`RheoBoard_v1/Drill/`](RheoBoard_v1/Drill/) |
| Assembly BOM | [`RheoBoard_v1.05/Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx`](RheoBoard_v1.05/Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx) |
| Pick-and-place | [`RheoBoard_v1/CPL/CPL.csv`](RheoBoard_v1/CPL/CPL.csv) |

## Supplier placement

JLCPCB's corrected top-side part placement for the submitted Rheoboard V1 assembly is
[`RheoBoard_v1.05/Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png`](RheoBoard_v1.05/Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png).
Order 8815214A_Y73, SMT job SMT026093063736, stamp 20261008110423663. See
[`RheoBoard_v1.05/Archive_v1_outputs/README.txt`](RheoBoard_v1.05/Archive_v1_outputs/README.txt).

The custom-board block diagram is [`images/custom-pcb-block-diagram.png`](images/custom-pcb-block-diagram.png). It shows 12 V in, the ESP32 Thing Plus, the Qwiic and I2C devices, a 6 V valve rail, and a 4.5 V pump rail.

This board is a prototype.
