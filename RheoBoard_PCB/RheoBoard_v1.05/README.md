# RheoBoard v1.05

The v1.05 CAD revision is saved and its manufacturing exports have been reconciled. The schematic cleanup resolves all 759 inherited warnings. KiCad reports zero schematic ERC errors or warnings and zero PCB DRC errors, warnings, unrouted connections or schematic/PCB mismatches. See the [review report](Verification/REVIEW_REPORT.md) for evidence and coverage limits.

This is an untested hardware revision. Supplier placement review, final order settings and physical electrical/thermal/mechanical qualification remain open; it is not approved for repeated production.

The latest full recheck found a pressure-sensor startup/reset risk at U5. Resolve that requirement before production approval; see the [current finding and checks](Verification/REVIEW_REPORT.md).

Open [the KiCad project](RheoboardV1/1.kicad_pro). Its main schematic and PCB are both named `1`; the other schematic files beside them are retained standalone block references or stubs, not the project hierarchy.

- [Revision notes](REVISION_NOTES.md) explain the circuit, footprint and assembly changes.
- [BOM](BOM/README.md) contains the complete workbook and SMT-only assembly list.
- [Assembly and bringup](ASSEMBLY_AND_BRINGUP.md) covers power, the external multiplexer, control mapping and open hardware checks.
- [Manufacturing files](Manufacturing/README.md) contain the matching fabrication archive, SMT placements and drawings.
- `Archive_v1_outputs/` contains the copied v1 manufacturing outputs and images for reference only. They are not v1.05 production files.

The original `RheoBoard_v1` project and shared original BOM remain preserved. This revision retains fixed-direction pumps for intermittent use and the last commanded state during communication loss. Multiple boards use an external I²C multiplexer, one board per channel.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
