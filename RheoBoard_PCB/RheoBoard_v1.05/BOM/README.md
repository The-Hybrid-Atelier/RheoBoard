# RheoBoard v1.05 BOM

The selective Altium update replaces D1 with Q11, D37 and R85. The revised BOM contains **148 fitted parts: 142 SMT and six manually fitted parts**. Use it with the matching saved layout and placement export documented in the [current review](../Verification/REVIEW_REPORT.md).

- [Complete parts workbook](BOM_RheoBoard_v1.05.xlsx): manufacturer numbers, sources, quantities, revision notes and manual accessories.
- [JLCPCB SMT-only BOM](BOM_RheoBoard_v1.05_JLCPCB.csv): 142 SMT parts in 45 procurement groups. Use with the [matching placement file](../Manufacturing/CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv).

Q11 is Diodes DMP6023LE-13 / C154901, SOT-223. D37 is Diodes BZT52C10-7-F / C155227, SOD-123. R85 reuses the 10 kΩ UNI-ROYAL 0603WAF1002T5E / C25804 selection; that group now contains 20 resistors. D1 B540C-13-F is removed. Verify Q11's gate/drain/source pin order and D37's cathode orientation in the new supplier preview.

Earlier improvements remain: U12 input eFuse, U13 Qwiic master interface, corrected fuse/inductor footprints and Panasonic EEEFK1H220P 22 µF / 50 V bulk capacitors. J9 connects the separately powered master; J1/J12/J17 are locally powered sensor ports. J3 provides local debug access; J18 remains removed. Valves retain on/off control and pumps retain PWM.

The six manually fitted parts are J2, J3, U8, U9, U10 and U11. U9/U10 are valve headers; the external valves are listed separately on the Manual accessories tab. Mounting holes, test points and open solder jumpers are PCB features, not purchased components. The original v1 workbook, [BOM_RheoboardV1_JLCSMT.xlsx](../Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx), is historical and is not the v1.05 order file.

Prices are historical estimates retained only for unchanged selected parts. New parts have blank, unquoted prices. The displayed subtotal is incomplete and is not an assembly quotation. Review exact part numbers and the new supplier placement preview before approving an order.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0.
