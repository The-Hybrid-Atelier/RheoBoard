# Laser-cut platform — design files

Design files for the laser-cut structural platform/frame that the BYO build sits on.

Modeled on [Calico's `3D print models/`](https://github.com/jsli96/calico/tree/main/3D%20print%20models)
folder — they document each mechanical part with material, recommended settings, and assembly
notes in the main README. We do the same here for laser-cut flat parts.

_Empty — no design files yet._

## Parts *(fill in as designed)*

| Part | Material | Thickness | Source file | Used in tutorial step |
|---|---|---|---|---|
| _(e.g. base plate)_ | _(e.g. plywood)_ | _(e.g. 6 mm)_ | _(e.g. `base-plate.svg`)_ | _(e.g. 02)_ |

## Recommended cut settings *(Calico-style — adapt per machine)*

Document starting-point settings here once tested — flag as "recalibrate for your machine,"
same as Calico does for print settings.

| Setting | Value | Notes |
|---|---|---|
| Material | TBD | e.g. birch plywood, acrylic |
| Thickness | TBD | Must match CAD |
| Kerf compensation | TBD | Measure on your laser |
| Power / speed | TBD | Starting point only |

Calico reference (3D print equivalent):

| Calico part | Material | Layer height | Infill | Notes |
|---|---|---|---|---|
| Main body | PLA / Carbon PLA | 0.2 mm | 20% | 30° print angle, supports |
| Track | TPU Shore-95A | 0.2 mm | 20% | Flat, modular pieces |

## File conventions

- **Vector source** (`.svg` or `.ai`/`.dxf`) is the source of truth for each cut part — keep it
  next to any exported/print-ready version rather than replacing it.
- **`.dxf`** — for feeding directly to a laser cutter that expects DXF.
- **`.pdf`** — print-at-actual-size reference, useful for checking fit without a laser cutter.
- Name files by part, e.g. `base-plate.svg`, `sensor-mount.svg`, not `Untitled1.svg`.
- Record material, thickness, and kerf compensation used for each part — either as a comment/
  layer in the file itself or in a short `cut-settings.md` here once that's known. Kerf varies
  by machine/material, so note the machine used too if it's not a generic setting.

## Once files exist

- [ ] Link each part to where it's referenced in `../tutorial/` (which step it's used in)
- [ ] Add material + thickness + kerf notes
- [ ] Add cut settings (power/speed) for the specific laser cutter used, if known — flag it as
      "starting point, recalibrate for your machine" rather than gospel
