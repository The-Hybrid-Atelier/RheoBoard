# Laser-cut platform — design files

Design files for the laser-cut structural platform/frame that the BYO build sits on.

_Empty — no design files yet._

## Conventions (fill in as files are added)

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
