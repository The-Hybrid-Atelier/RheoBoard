# RheoBoard v1.05

The current revision uses **U7 PCA9685 for both pumps and valves**. Four buttons connect to the ESP32 through the new **J19 five-pin GPIO header**. U4 TCA9534 and its dedicated support parts are removed. Valves use steady FULL ON/OFF; pumps retain independent duty control at one shared frequency. The BOM has 142 fitted parts, including 135 SMT placements.

**J9 is the Qwiic master port** for the USB/battery-powered ESP32 Thing Plus. U13 separates the host and local bus supply domains. J1, J12 and J17 remain locally powered Qwiic sensor ports. All four retain standard Qwiic pin order. J19 is a separate button cable, not Qwiic; using the buttons requires both J9 and J19 connections. J3 remains the four-pin local debug port. The redundant J18 remains removed.

Open [the KiCad project](RheoboardV1/RheoBoard_v1.05.kicad_pro). The main schematic and PCB use that same stem. Other schematics beside them are standalone block references or stubs, not the project hierarchy.

The saved PCA/button update passes ERC and saved/refilled DRC under the retained rule settings. The [current review](Verification/REVIEW_REPORT.md) and [change evidence](Verification/PCA_Control_2026-10-10/README.md) describe coverage, retained circuits and exact source/output hashes. BOM, placement files, drawings and fabrication exports match the updated board.

This remains an untested hardware revision. Supplier placement review and physical electrical, thermal and mechanical qualification remain open. The maintainer accepted the retained sensor-reset and valve-voltage-margin risks for this prototype; that acceptance is not a measured hardware result or repeated-production approval.

- [Revision notes](REVISION_NOTES.md) explain circuit, footprint and assembly changes.
- [BOM](BOM/README.md) contains the complete workbook and SMT-only list.
- [Assembly and bringup](ASSEMBLY_AND_BRINGUP.md) covers the button cable, GPIO map, PCA register settings, power, multiplexer and hardware checks.
- [Manufacturing files](Manufacturing/README.md) contain the matching fabrication archive, SMT placements and drawings.
- [Filename-cleanup evidence](Verification/Rename_2026-10-10/validation.json) and [J18-removal evidence](Verification/Remove_J18_2026-10-09/README.md) are historical checkpoints.
- `Archive_v1_outputs/` contains historical v1 outputs, including the original [assembly workbook](Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx); they are not v1.05 production files.

The board retains fixed-direction, intermittent-use pumps and the last commanded state during communication loss. Multiple boards use one selected branch of an external I²C multiplexer. Each connected button bank additionally needs four separate master GPIO inputs. Firmware must implement the revised interface; the repository's DIY sketch is not verified compatible.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
