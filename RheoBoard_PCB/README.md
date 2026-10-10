# RheoBoard_v1 - PCB

This folder is RheoBoard_v1 - PCB: a separate KiCad board. Everyday power is a 12 V plug. Shared firmware is in `../code/firmware/`.

Public notes are on the [Main (PCB)](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Main-(PCB)) wiki page. That page leads to the BOM and PCB KiCad design pages.

License: hardware CERN-OHL-W-2.0 — see [`../LICENSE`](../LICENSE).

## Files in this folder

Open `RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pro` in KiCad 10.

- Schematic: `RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_sch`
- Board: `RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pcb`
- Libraries: `RheoBoard_v1/Symbol/`, `RheoBoard_v1/Footprint/`, `RheoBoard_v1/3D/`
- Gerbers: `RheoBoard_v1/Gerber/`
- Drills: `RheoBoard_v1/Drill/`
- Pick-and-place: `RheoBoard_v1/CPL/CPL.csv`
- Archived v1 assembly workbook: `RheoBoard_v1.05/Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx`

The other `.kicad_sch` files in `RheoBoard_v1/RheoboardV1/` are separate block drawings, not hierarchical sheets of `RheoBoard_v1.kicad_pro`. `3_MICROPROCESSOR.kicad_sch` and `Switch TCA9534.kicad_sch` are empty stubs.

The v1.05 project is `RheoBoard_v1.05/RheoboardV1/RheoBoard_v1.05.kicad_pro`. Its schematic and board use that same stem. Notes for that revision are in `RheoBoard_v1.05/README.md`. Current purchasing files are `RheoBoard_v1.05/BOM/BOM_RheoBoard_v1.05.xlsx` and `RheoBoard_v1.05/BOM/BOM_RheoBoard_v1.05_JLCPCB.csv`, with placement `RheoBoard_v1.05/Manufacturing/CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv`.

Both footprint libraries name the gate-driver footprint `DBV0006A_N.kicad_mod`. Rename validation is `RheoBoard_v1.05/Verification/Rename_2026-10-10/validation.json`.

The retained Altium import is `RheoBoard_v0.05-altium/RheoBoard_Altium_Reimport.kicad_pro`. Its import report `RheoBoard_v0.05-altium/IMPORT_REPORT.txt` records the recovered schematic sheets.

JLCPCB's corrected top placement for the submitted Rheoboard V1 assembly is `RheoBoard_v1.05/Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png`. Order 8815214A_Y73, SMT job SMT026093063736, stamp 20261008110423663. See `RheoBoard_v1.05/Archive_v1_outputs/README.txt`.

The block diagram in this folder is `images/custom-pcb-block-diagram.png`.
