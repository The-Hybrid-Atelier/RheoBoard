# Parts to 3D print

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

Printable tube and connector models are in [`BuildYourOwn/cad/connector/`](../BuildYourOwn/cad/connector/). Notes are in [`BuildYourOwn/cad/connector/README.md`](../BuildYourOwn/cad/connector/README.md).

From that README: `sensing_tube.scad` is the preferred format for changing the sensing tube. These files are mesh exports only. Treat them as supplementary until a parametric source is added:

- `connector_small.stl` — current small connector
- `connector_big.stl` — alternate larger connector
- `sensing_tube_open.stl` — alternate open tube

From that README: `connector_small.stl` is a mesh export. An editable source for it is not in `BuildYourOwn/cad/connector/` yet.

The panel and enclosure are 3D printed. Files are in [`BuildYourOwn/cad/encloser/`](../BuildYourOwn/cad/encloser/): `Concept_v2.STEP` is the full assembly, and `part/` holds the printable parts (base panel, pump holders, bracket, enclosure body, and lid). Print in PLA on a Bambu Lab printer with the normal profile; Bambu Studio opens the STEP files directly. Part roles, copy counts, and orientation are still TODO in [that README](../BuildYourOwn/cad/encloser/README.md). The editable CAD project is not in the repo yet.
