# RheoBoard v1.05 BOM

The actuator-control update removes U4 TCA9534APWR and six supporting SMT parts. It adds the manually fitted J19 button header. The revised BOM contains **142 fitted parts: 135 SMT and seven manually fitted parts**. Use it with the saved layout and placement export in the [current review](../Verification/REVIEW_REPORT.md).

- [Complete parts workbook](BOM_RheoBoard_v1.05.xlsx): part identities, sources, quantities and manual accessories.
- [JLCPCB SMT-only BOM](BOM_RheoBoard_v1.05_JLCPCB.csv): 135 SMT parts in 42 groups, matching the [placement file](../Manufacturing/CPL/CPL_RheoBoard_v1.05_JLCPCB_SMT.csv).

Removed fitted parts are U4, C22, C47, R10, R11, R13 and R14; the obsolete TP1 feature is also removed. J19 uses a straight 1×5, 2.54 mm male header fitted by hand. The workbook gives a cut-to-five-position header source and a short five-lead GPIO cable example. Neither is included in the SMT order. Actual cost savings require a new quote including the extra header/cable; retained workbook prices are historical incomplete estimates.

U7 now controls both pumps and valves. The valves remain steady ON/OFF, while the four buttons connect separately to ESP32 GPIOs through J19. R16–R19 use host 3.3 V from J9. Follow the [button cable and firmware instructions](../ASSEMBLY_AND_BRINGUP.md#button-gpio-cable).

The previous improvements remain: Q11/D37/R85 lower-loss reverse-polarity stage, U12 eFuse, U13 Qwiic interface, corrected fuse/inductor footprints and four 22 µF/50 V bulk capacitors. J9 is the separately powered master port; J1/J12/J17 are local sensor ports. J3 remains the local debug port, and the old J18 service header remains removed.

Manually fit J2, J3, J19, U8, U9, U10 and U11. U9/U10 are valve headers; the external valves are listed on the Manual accessories tab. Holes, test points and open solder jumpers are PCB features, not purchased parts. The [original v1 workbook](../Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx) is historical.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0.
