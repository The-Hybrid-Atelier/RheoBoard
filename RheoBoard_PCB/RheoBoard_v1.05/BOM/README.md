# RheoBoard v1.05 BOM

Updated for the dedicated J9 Qwiic master interface on 2026-10-09. The BOM contains 147 fitted parts, including 140 SMT parts. Final saved-board and placement reconciliation is recorded in the [current review report](../Verification/REVIEW_REPORT.md).

- [Complete parts workbook](BOM_RheoBoard_v1.05.xlsx): 147 fitted parts, manufacturer numbers, sources, revision notes and manual accessories.
- [JLCPCB SMT-only BOM](BOM_RheoBoard_v1.05_JLCPCB.csv): 140 SMT parts. Use with the [matching v1.05 placement file](../Manufacturing/CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv).

The Qwiic update adds U13 TCA9517ADGKR (C201698), C68/C69 100 nF (C14663), R82/R83 4.7 kΩ (C23162) and R84 10 kΩ (C25804). J9 connects the separately powered master; J1/J12/J17 remain locally powered sensor ports. J18 is a local service connector. The four bulk capacitors remain Panasonic EEEFK1H220P, 22 µF / 50 V. Earlier purchased-part selections are retained.

The original v1 workbook, [BOM_RheoboardV1_JLCSMT.xlsx](../Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx), is archived with the other v1 outputs. It is not the v1.05 order file.

The seven manually fitted parts are J2, J3, J18, U8, U9, U10 and U11. U9/U10 are valve headers; the external valves are listed separately on the workbook's Manual accessories tab. Mounting holes, test points and open solder jumpers are PCB features, not purchased components.

Prices are historical estimates retained only for unchanged selected parts. Blank prices are unquoted. The displayed subtotal is incomplete and is not an assembly quotation. Review exact part numbers and the new supplier placement preview before approving an order.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0.
