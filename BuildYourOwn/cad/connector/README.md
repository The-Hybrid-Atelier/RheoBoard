# Sensing-system tube CAD

This folder contains printable tube and connector models for the rheometer's pneumatic sensing
system. Open either STL link on GitHub to inspect the model in its interactive 3D viewer.

License: hardware CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

## Sensing tube

**[Open the sensing tube in GitHub's interactive 3D viewer](sensing_tube.stl)**

[`sensing_tube.stl`](sensing_tube.stl) is the full hanging sensing tube with an internal flow
passage and a male luer-lock connection. The model is approximately **12 mm in diameter ×
113.2 mm tall**. Its editable OpenSCAD source is
[`sensing_tube.scad`](sensing_tube.scad).

The luer-lock geometry is intended to mate with a Value Plastics **FTLLB220-6005** female
luer-thread panel-mount fitting. Confirm the fit with the actual fitting before assembling the
pneumatic system.

## Small connector

**[Open the small connector in GitHub's interactive 3D viewer](connector_small.stl)**

[`connector_small.stl`](connector_small.stl) is the compact threaded-to-barbed connector used in
the sensing tube assembly. Its mesh envelope is approximately **6.35 × 7.33 × 15.54 mm**.
This file is a mesh export. An editable source for it is not in this folder yet.

## Viewing and printing

- GitHub displays an interactive rotate, pan, and zoom view when either STL link above is opened.
- GitHub Markdown cannot embed that interactive viewer directly inside this README.
- For local inspection or slicing, open the STL in your preferred slicer or mesh viewer.
- STL dimensions are in millimetres; import at **100% scale**.
- These models are prototypes. Test-print both parts and verify the thread, luer, tubing, airflow,
  and leak-tight fit with the real hardware before use.
- Do not treat a printed part as pressure-rated, food-safe, or medical-grade without independent
  material and process validation.

## Meshes without an editable source

`sensing_tube.scad` is the preferred format for changing the sensing tube. These files are mesh
exports only. Treat them as supplementary until a parametric source is added:

- [`connector_small.stl`](connector_small.stl) — current small connector
- [`connector_big.stl`](connector_big.stl) — alternate larger connector
- [`sensing_tube_open.stl`](sensing_tube_open.stl) — alternate open tube

Use [`sensing_tube.stl`](sensing_tube.stl) and [`connector_small.stl`](connector_small.stl) for the
current sensing-system assembly unless the build instructions specify otherwise.
