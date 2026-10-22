# RheoBoard v0.05 Altium conversion

Open `RheoBoard_Altium_Reimport.kicad_pro` in KiCad 10. This historical conversion is separate from the later native v1 and v1.05 designs. The original Altium archive is retained in `RheoBoard_v0.05.rar`.

The import recovered MAIN, POWER and a drawing-only block-diagram sheet, plus a two-layer PCB and its imported footprint library. Eight external illustrations were unavailable. The flat schematic sheets require connectivity and reference reconciliation before schematic-to-board updates.

This conversion is not ready for fabrication. The retained design includes a backside copper overlap between pump 1 and a valve switched node, an unconnected controller LED5 output and missing valve-2 controller drive, plus ERC/DRC findings involving clearances, copper edges, annular rings, mask and imported library links. No fabrication package was regenerated for this conversion. Use the later revision folders for their corresponding designs; do not mix their outputs.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
