# RheoBoard PCB

This folder holds the PCB designs. There are three versions: [`RheoBoard_v0.05-altium`](RheoBoard_v0.05-altium/), [`RheoBoard_v1`](RheoBoard_v1/), and [`RheoBoard_v1.05`](RheoBoard_v1.05/).

[`RheoBoard_v0.05-altium`](RheoBoard_v0.05-altium/) is a KiCad re-import of the Altium design, saved separately from the other two versions.

[`RheoBoard_v1`](RheoBoard_v1/) holds the KiCad schematic and board for that version, plus symbol, footprint, and 3D libraries, Gerbers, drills, pick-and-place files, and the 28 September 2026 JLCPCB BOM.

[`RheoBoard_v1.05`](RheoBoard_v1.05/) holds the v1.05 KiCad project, with revision notes, a BOM, assembly notes, manufacturing files, verification records, and an archive of the historical v1 outputs.

## Files in this folder

Open [`RheoBoard_v1.kicad_pro`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pro) in KiCad 10.

- Schematic: [`RheoBoard_v1.kicad_sch`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_sch)
- Board: [`RheoBoard_v1.kicad_pcb`](RheoBoard_v1/RheoboardV1/RheoBoard_v1.kicad_pcb)
- Libraries: [`Symbol`](RheoBoard_v1/Symbol/), [`Footprint`](RheoBoard_v1/Footprint/), [`3D`](RheoBoard_v1/3D/)
- Gerbers: [`Gerber`](RheoBoard_v1/Gerber/)
- Drills: [`Drill`](RheoBoard_v1/Drill/)
- Pick-and-place: [`CPL.csv`](RheoBoard_v1/CPL/CPL.csv) and [`CPL_JLCSMT.xlsx`](RheoBoard_v1/CPL/CPL_JLCSMT.xlsx)
- JLCPCB BOM: [`BOM_RheoboardV1_JLCSMT.xlsx`](RheoBoard_v1/BOM/BOM_RheoboardV1_JLCSMT.xlsx), restored from the 28 September 2026 workbook. The same file remains in the [v1.05 archive](RheoBoard_v1.05/Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx).

License: hardware CERN-OHL-W-2.0 — see [`LICENSE`](../LICENSE).
