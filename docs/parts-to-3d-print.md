# Parts to 3D print

## Print these

Panel sizes and likely roles are from
[`BuildYourOwn/cad/encloser/README.md`](../BuildYourOwn/cad/encloser/README.md). Sensing-tube and
small-connector facts are from
[`BuildYourOwn/cad/connector/README.md`](../BuildYourOwn/cad/connector/README.md). Where that
README leaves copy count, orientation, or supports open, the cell below is TODO.

### Panel

Print in PLA on a Bambu Lab printer with the normal profile. Bambu Studio opens the STEP files
directly. There are no STL or slicer project files for these parts yet. Files are in
[`BuildYourOwn/cad/encloser/part/`](../BuildYourOwn/cad/encloser/part/).

| File | Approx. size (mm) | Likely role | Copies |
|---|---|---|---|
| [`part/part_01_holes_4mm.STEP`](../BuildYourOwn/cad/encloser/part/part_01_holes_4mm.STEP) | 170 × 170 × 4 | Base panel, with 4 mm holes | TODO |
| [`part/part_2.STEP`](../BuildYourOwn/cad/encloser/part/part_2.STEP) | 42.5 × 45 × 81 | Pump holder | TODO |
| [`part/part_03.STEP`](../BuildYourOwn/cad/encloser/part/part_03.STEP) | 45 × 12 × 15 | Small bracket, likely for the valve | TODO |
| [`part/part_04.STEP`](../BuildYourOwn/cad/encloser/part/part_04.STEP) | about 195 wide | TODO | TODO |
| [`part/part_05.STEP`](../BuildYourOwn/cad/encloser/part/part_05.STEP) | 195 × 195 × 29 | TODO | TODO |

TODO: confirm each part's role and the number of copies. The annotated photo does not label
`part_04.STEP` or `part_05.STEP`. TODO: print orientation, supports, and any non-default settings
per part.

[`Concept_v2.STEP`](../BuildYourOwn/cad/encloser/Concept_v2.STEP) is the full assembly, including
the purchased components. The editable CAD project that produced these STEP files is not in this
repository yet.

### Sensing tube and small connector

Import at 100% scale. STL dimensions are in millimetres. TODO: material, print settings, and
number of copies.

| File | Approx. size | What it is |
|---|---|---|
| [`sensing_tube.stl`](../BuildYourOwn/cad/connector/sensing_tube.stl) | 12 mm diameter × 113.2 mm tall | Full hanging sensing tube with an internal flow passage and a male luer-lock. Editable source: [`sensing_tube.scad`](../BuildYourOwn/cad/connector/sensing_tube.scad). The luer is intended to mate with a Value Plastics FTLLB220-6005 female luer-thread panel-mount fitting. |
| [`connector_small.stl`](../BuildYourOwn/cad/connector/connector_small.stl) | 6.35 × 7.33 × 15.54 mm | Compact threaded-to-barbed connector used in the sensing tube assembly. Mesh export; an editable source is not in that folder yet. |

Test-print both parts and verify the thread, luer, tubing, airflow, and leak-tight fit with the
real hardware before use. Use these two files for the current sensing-system assembly unless the
build instructions specify otherwise.

`sensing_tube.stl` and `connector_small.stl` are not the probe.

Printing both groups is [Step 01](../BuildYourOwn/README.md#step-01-assemble-the-platform) of the
build guide.

### Probe (print files not in the repo yet)

Exactly two models are still to be added, both TODO because the files are not in this repository: one GL45 two-port cap, and one tube that connects to that GL45 cap. The probe also uses the [Adafruit 4661](https://www.adafruit.com/product/4661) tube. [`sensing_tube.stl`](../BuildYourOwn/cad/connector/sensing_tube.stl) and [`connector_small.stl`](../BuildYourOwn/cad/connector/connector_small.stl) are not those parts.

## Meshes without an editable source

From the connector README: `sensing_tube.scad` is the preferred format for changing the sensing
tube. These files are mesh exports only. Treat them as supplementary until a parametric source is
added:

- `connector_small.stl` — current small connector (also in "Print these" above)
- `connector_big.stl` — alternate larger connector
- `sensing_tube_open.stl` — alternate open tube

License: CC BY-SA 4.0 — see [Certification](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Certification).
