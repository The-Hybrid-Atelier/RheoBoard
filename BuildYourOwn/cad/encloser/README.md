# 3D-printed panel

License: hardware CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

The pumps, valve, and electronics mount on a flat 3D-printed panel.
The folder name stays `BuildYourOwn/cad/encloser/`.

The placement reference is the annotated panel photo [`panel-annotated.pdf`](panel-annotated.pdf).

Readable labels in that photo, and where they point:

- "3-D-printed panel" points at the flat panel.
- "Air pump" appears twice, once on each pump along the top of the photo.
- "Pressure sensor" points at the board between those pumps.
- "ESP32 Thing Plus" points at the board on the left side of the photo.
- "Qwiic Button" points at the board on the right side of the photo.
- "Valve" points at the part in the center of the photo.
- "L298N drivers" points at the two boards along the bottom of the photo. The callout does not say which board is which.

Hole positions are not labeled. TODO: which L298N board is the pump driver and which is the valve driver.

`encloser_pic1.jpg`, `encloser_pic2.jpg`, and `encloser_pic3.jpg` are CAD views stored in this folder.

## Files

`Concept_v2.STEP` is the full assembly, including the purchased components. The printable parts
are in [`part/`](part/). Sizes below are approximate, measured from the STEP geometry.

| File | Approx. size (mm) | Likely role |
|---|---|---|
| [`part/part_01_holes_4mm.STEP`](part/part_01_holes_4mm.STEP) | 170 × 170 × 4 | Base panel, with 4 mm holes |
| [`part/part_2.STEP`](part/part_2.STEP) | 42.5 × 45 × 81 | Pump holder |
| [`part/part_03.STEP`](part/part_03.STEP) | 45 × 12 × 15 | Small bracket, likely for the valve |
| [`part/part_04.STEP`](part/part_04.STEP) | about 195 wide | TODO |
| [`part/part_05.STEP`](part/part_05.STEP) | 195 × 195 × 29 | TODO |

TODO: confirm each part's role and the number of copies to print. The annotated photo does not label `part_04.STEP` or `part_05.STEP`.

## Printing

- Material: PLA.
- Printer and settings: a Bambu Lab printer with the normal (default) PLA profile.
- Bambu Studio opens STEP files directly; there are no STL or slicer project files yet.

TODO: print orientation, supports, and any non-default settings per part.

The sensing tube and small connector are printed from [`../connector/`](../connector/), in Step 01 of the [assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#step-01-assemble-the-platform).

## Assembly

Fasteners are 4 mm zip ties. A count is not stated. Place the labeled parts as in [`panel-annotated.pdf`](panel-annotated.pdf). The files stay in this folder (`BuildYourOwn/cad/encloser/`).

The editable CAD project that produced these STEP files is not in this repository yet.
