# RheoBoard v1.05 manufacturing files

Order from the files named on the [BOM](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/BOM) page. `../Verification/REVIEW_REPORT.md` records the check of these exports. Files in `../Archive_v1_outputs/` and the v1 JLCPCB placement `../Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png` belong to v1.

## Files

- Fabrication archive `RheoBoard_v1.05_fabrication.zip`: nine Gerber layers, matching plated/non-plated drill files and Gerber job file. The same twelve files are in `Gerber/`.
- SMT placement file `CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv` and SMT BOM `../BOM/BOM_RheoBoard_v1.05_JLCPCB.csv`: 135 matching references, all on top. Placement values come from the native JLC export. The separate native positions file is an unfiltered reference, not the SMT order file.
- Complete BOM `../BOM/BOM_RheoBoard_v1.05.xlsx`: 142 fitted parts and separate accessories. Manually fit J2, J3, J19, U8, U9, U10 and U11.
- Assembly drawing `Drawings/RheoBoard_v1.05_assembly.pdf`, schematic `Drawings/RheoBoard_v1.05_schematic.pdf`, front copper `Drawings/RheoBoard_v1.05_front_copper.pdf`, back copper `Drawings/RheoBoard_v1.05_back_copper.pdf`, and solder mask `Drawings/RheoBoard_v1.05_soldermask.pdf`.
- `../Verification/REVIEW_REPORT.md` and `release-file-manifest.json` bind the current sources and exports by SHA-256.

## Supplier and hardware acceptance

The maintainer accepted the retained pressure-sensor startup/reset and valve voltage-margin risks for this prototype and requested no circuit changes for them. File checks do not establish physical qualification or production approval. See `../Verification/REVIEW_REPORT.md`.

Request a fresh supplier placement/stencil preview for this v1.05 revision. Check Q11 pin 1/gate, pin 2/drain/tab and pin 3/source; D37's cathode on VIN_RAW/source; centered F1; U12/U13 pin 1; D36 and flyback-diode cathodes; and all polarized capacitors. U13 must be the DGK 8-pin TCA9517ADGKR; its local footprint follows TI's recommended copper and stencil pattern. Confirm J9 MASTER and J1/J12/J17 SENSOR labels, and that U4/C22/C47/R10/R11/R13/R14 are absent. J19 is a manual part with pin 1 GND; verify the button-header markings and its cable separately. File reconciliation does not establish the supplier's rotation conventions. No rotation correction has been assumed.

Require exact D36 Littelfuse SMBJ13A: the distributor's generic attribute table conflicts with that named manufacturer part. Current stock, substitutions, quantity, copper weight, finish, panelization and assembly/stencil service settings have not been accepted. The fabrication basis remains two-layer 1 oz FR-4; different order settings require their own limits review. Confirm panel/tooling fiducials with the assembler.

`../ASSEMBLY_AND_BRINGUP.md` records manual fitting, connector pin order and first-board measurements. Supplier preview and physical electrical, thermal and mechanical qualification remain open. These files have not been uploaded and no new order has been placed.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
