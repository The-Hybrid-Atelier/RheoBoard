# Order five RheoBoard v2 bare PCBs

**For a board made from the original v2 Altium artwork**, upload [RheoBoard_v2-original-Altium-for-review.zip](RheoBoard_v2-original-Altium-for-review.zip). It contains the seven original plots and two original drill programs with standard fabrication filename extensions. Their contents match the supplied Altium exports byte for byte. The previous manufacturer's final production files are unavailable.

Choose **Review Before Payment** and request manual production-file approval. Ask JLCPCB to check the copper around the four pump slots on the bottom face, copper at the board edge and the eight 3.2 mm routed openings. The round-hole program contains both plated holes and two 9 mm unplated pump bores (tool T10). The slot program contains seven plated slots. Request approval before any artwork change. [JLCPCB ordering guide](https://jlcpcb.com/help/article/how-do-i-place-an-order)

Compare the manufacturer's production artwork with the [original Altium top and bottom views](../../Previews/RheoBoard_v2-Gerber.png).

The [adjusted JLCPCB fabrication ZIP](RheoBoard_v2-JLCPCB-bare-board.zip) has separate plated and non-plated drill files and adjustments to copper, holes, mask and legend.

## Quote settings for five bare boards

| Quote setting | Select |
|---|---|
| Board | FR-4, 2 layers, 1 design, single PCB, quantity 5 |
| Size | 136.042 × 88.340 mm |
| Finished thickness | 0.4 mm nominal |
| Outer copper | 1 oz on both faces |
| Solder mask / silkscreen | Green / white |
| Surface finish | ENIG |
| Via covering | Tented, following the supplied mask openings |
| Electrical test | Flying probe |
| Confirm production file | Yes, with “Do not confirm automatically” selected |
| PCB assembly / stencil | Off / off |

The specified 0.4 mm finished thickness follows the 0.411 mm board in the v2 Altium stack and STEP model. JLCPCB reviews thin boards. [JLCPCB thickness and fabrication limits](https://jlcpcb.com/capabilities/pcb-capabilities)

## Adjusted package preview

Compare JLCPCB’s preview with the [top and bottom views rendered from this ZIP](RheoBoard_v2-bare-board-preview.png).

Look for one board with **eight 3.2 mm routed openings**, **two 9 mm non-plated pump holes** and **seven plated slots**. Four of the slots are for the pumps and three are for the J6 power jack. The two solid rectangles along the top and bottom edges are printed on the top silkscreen.

JLCPCB’s quote detected both copper layers and a 136.04 × 88.34 mm outline. Its separate JLCDFM checker treated the two 9 mm pump bores as plated and flagged annular rings, through-hole-to-surface-pad spacing, via-to-pad spacing and silkscreen-to-hole clearance. Review those findings with the fabricator before releasing this ZIP for production.

When JLCPCB sends its production files, check these features again before approving fabrication. [How to confirm the production file](https://jlcpcb.com/help/article/how-to-confirm-the-production-file)

**Suggested order remark**

> Five individual 0.4 mm boards. Please confirm the seven plated slots in the PTH file, the two unplated 9 mm pump bores in the NPTH file, and the eight 3.2 mm routed openings. Please check the reported annular rings, through-hole and via clearances, and silkscreen near holes against your production limits before fabrication.

This upload preserves the v2 circuit and pump-terminal copper. It includes fabrication adjustments to pad shapes, four ground-via drills, perimeter copper, mask and legend. The adjacent Gerber and NC-Drill folders hold the original Altium exports in a different coordinate system. Use the upload ZIP as a complete set.

Five ground vias intersect exposed top-side solder lands. Keep these mask openings as drawn. Larger via holes may remain open after ordinary tenting. [JLCPCB via covering](https://jlcpcb.com/help/article/pcb-via-covering)

## LED orientation for hand assembly

Match the OSRAM package’s cathode mark to pad 1. Use the [component-side orientation guide](RheoBoard_v2-LED-orientation.png) with the pump footprints on the right. Several white polarity dots touch nearby lettering.

| Cathode faces | References |
|---|---|
| Right | LED1–LED6, lLED1, lLED2 |
| Left | lLED3 (3.3 V rail indicator) |
| Down | D10, D11, D16, D17 |

The three rail references start with a lowercase `l`. This guide covers the six red LS R976 and the seven green LG R971 LEDs in the v2 purchasing BOM. Both package drawings show the cathode mark on page 9. [Red LS R976](https://look.ams-osram.com/m/330c725e4ebb09ab/original/LS-R976.pdf#page=9) · [Green LG R971](https://look.ams-osram.com/m/2da6f2ceeebf8fac/original/LG-R971.pdf#page=9)
