# RheoBoard v1.05

Ordering notes for this revision are on the [BOM](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/BOM) page. Open `RheoboardV1/RheoBoard_v1.05.kicad_pro`. The main schematic and PCB use that same stem. Other schematics beside them are standalone block references or stubs, not the project hierarchy.

- Revision notes: `REVISION_NOTES.md`
- BOM: `BOM/README.md`
- Assembly and bringup: `ASSEMBLY_AND_BRINGUP.md` (button cable, GPIO map, and PCA register settings)
- Manufacturing files: `Manufacturing/README.md`
- Current review: `Verification/REVIEW_REPORT.md`
- Change evidence: `Verification/PCA_Control_2026-10-10/README.md`
- Filename-cleanup evidence: `Verification/Rename_2026-10-10/validation.json`
- J18-removal evidence: `Verification/Remove_J18_2026-10-09/README.md`
- `Archive_v1_outputs/` holds the historical v1 outputs, including the original assembly workbook `Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx`

This remains an untested hardware revision. Supplier placement review and physical electrical, thermal, and mechanical qualification remain open. The maintainer accepted the retained sensor-reset and valve-voltage-margin risks for this prototype; that acceptance is not a measured hardware result or repeated-production approval. The repository DIY sketch is not verified compatible with this actuator interface.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
