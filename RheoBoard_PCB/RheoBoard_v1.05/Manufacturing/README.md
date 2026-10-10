# RheoBoard v1.05 manufacturing files

The current package includes the dedicated J9 Qwiic master interface, U13 and its five supporting components, as well as the earlier input-protection and footprint corrections. Use the sources, BOM and manufacturing exports in this revision together; the [current review](../Verification/REVIEW_REPORT.md) records their verification. The copied files in `../Archive_v1_outputs/` and the [v1 JLCPCB placement](../Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) belong to v1.

## Files

- [Fabrication archive](RheoBoard_v1.05_fabrication.zip): nine Gerber layers, matching plated/non-plated drill files and Gerber job file. The same twelve files are in `Gerber/`; drill files are in that folder too.
- [SMT placement file](CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv) and [SMT BOM](../BOM/BOM_RheoBoard_v1.05_JLCPCB.csv): 140 matching references, all on top. Placement values are preserved from the native JLC export. The separate native positions file is an unfiltered reference, not the SMT order file.
- [Complete BOM](../BOM/BOM_RheoBoard_v1.05.xlsx): 146 fitted parts and separate accessories. Manually fit J2, J3, U8, U9, U10 and U11. J18 has been removed; the fabrication archive and drawings have been regenerated.
- [Assembly drawing](Drawings/RheoBoard_v1.05_assembly.pdf), [schematic](Drawings/RheoBoard_v1.05_schematic.pdf), [front copper](Drawings/RheoBoard_v1.05_front_copper.pdf), [back copper](Drawings/RheoBoard_v1.05_back_copper.pdf) and [solder mask](Drawings/RheoBoard_v1.05_soldermask.pdf).
- [Review report](../Verification/REVIEW_REPORT.md) records checks and remaining limitations. The release manifest binds sources and exports by SHA-256.

## Supplier and hardware acceptance

The current U5 startup/reset finding remains unresolved. The files pass the documented geometry and consistency checks; they are not production-approved. See the [current review](../Verification/REVIEW_REPORT.md).

Request a new supplier placement preview for v1.05. Check the centered F1 fuse, U12/U13 pin 1, D1/D36 and flyback-diode cathodes, and all polarized capacitors against the drawing and footprint pad numbers. U13 must be the DGK 8-pin TCA9517ADGKR; its local footprint follows TI's recommended copper and stencil pattern. Confirm J9's new MASTER label and J1/J12/J17 SENSOR roles. File reconciliation does not establish the supplier's rotation conventions. No orientation correction has been assumed.

Require the exact Littelfuse SMBJ13A for D36: the distributor attribute table conflicts with the named manufacturer part. Do not approve a replacement based only on that generic table. Current stock, substitutions, quantity, copper weight, finish, panelization and assembly/stencil service settings have not been accepted. The checked fabrication limits use a two-layer 1 oz FR-4 basis; different order settings require their own limits review.

Konnect's raw JLC package can include mechanical features and manual parts differently in its BOM and CPL. The delivered SMT pair contains only the 140 fitted SMT parts; any raw package warnings and the independent reconciliation are retained in the current review evidence. Filtering population does not prove supplier acceptance.

The [assembly and bringup guide](../ASSEMBLY_AND_BRINGUP.md) records manual fitting, connector pin order and first-board measurements. Supplier preview and physical electrical, thermal and mechanical qualification remain open. These files have not been uploaded and no new order has been placed.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
