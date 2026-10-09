# RheoBoard v1.05 manufacturing files

Generated from the checked v1.05 PCB on 2026-10-09, including the enlarged TP2–TP9 probe pads. After the schematic warning cleanup, the schematic PDF was refreshed; unchanged fabrication layers, placement and purchasing selections retain their verified Gerbers, drills, BOM and CPL. Use the files in this revision together. The copied files in `../Archive_v1_outputs/` and the old JLCPCB placement image belong to v1.

## Files

- [Fabrication archive](RheoBoard_v1.05_fabrication.zip): nine Gerber layers, matching plated/non-plated drill files and Gerber job file. The same twelve files are in `Gerber/`; drill files are in that folder too.
- [SMT placement file](CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv) and [SMT BOM](../BOM/BOM_RheoBoard_v1.05_JLCPCB.csv): exactly 134 matching references, all on top. Placement values are preserved from the native JLC export. The separate native positions file is an unfiltered reference, not the SMT order file.
- [Complete BOM](../BOM/BOM_RheoBoard_v1.05.xlsx): 141 fitted parts and separate accessories. Manually fit J2, J3, J18, U8, U9, U10 and U11.
- [Assembly drawing](Drawings/RheoBoard_v1.05_assembly.pdf), [schematic](Drawings/RheoBoard_v1.05_schematic.pdf), [front copper](Drawings/RheoBoard_v1.05_front_copper.pdf), [back copper](Drawings/RheoBoard_v1.05_back_copper.pdf) and [solder mask](Drawings/RheoBoard_v1.05_soldermask.pdf).
- [Review report](../Verification/REVIEW_REPORT.md) records checks and remaining limitations. The release manifest binds sources and exports by SHA-256.

## Supplier and hardware acceptance

The current U5 startup/reset finding remains unresolved. The files pass the documented geometry and consistency checks; they are not production-approved. See the [current review](../Verification/REVIEW_REPORT.md).

Request a new supplier placement preview for v1.05. Check the centered F1 fuse, U12 pin 1, D1/D36 and flyback-diode cathodes, and all polarized capacitors against the drawing and footprint pad numbers. File reconciliation does not establish the supplier's rotation conventions. No orientation correction has been assumed.

Require the exact Littelfuse SMBJ13A for D36: the distributor attribute table conflicts with the named manufacturer part. Do not approve a replacement based only on that generic table. Current stock, substitutions, quantity, copper weight, finish, panelization and assembly/stencil service settings have not been accepted. The checked fabrication limits use a two-layer 1 oz FR-4 basis; different order settings require their own limits review.

The raw Konnect JLC package reports a BOM/CPL population mismatch because it includes mechanical features and manual parts differently. The delivered SMT pair resolves that mismatch by filtering to the 134 fitted SMT parts; it does not change the raw tool verdict or prove supplier acceptance.

The [assembly and bringup guide](../ASSEMBLY_AND_BRINGUP.md) records manual fitting, connector pin order and first-board measurements. Supplier preview and physical electrical, thermal and mechanical qualification remain open. These files have not been uploaded and no new order has been placed.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
