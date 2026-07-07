# BOM tracking

Source of truth for the bill of materials is `RheoBoard_V8_Final/RheoBoard_V8.BomDoc` (an
Altium BOM document — text/XML underneath, so it diffs fine in git; see `.gitattributes`).

Notes:

- When swapping or adding a part, record the reason and a datasheet link in the relevant
  exec-plan and in `docs/references/index.md`.
- Prefer parts with mainstream distributor availability (avoid single-source parts) unless
  there's a documented reason otherwise — see `docs/design-docs/core-beliefs.md`.
- If/when it's useful to view the BOM outside Altium, export a CSV via the `.OutJob` and
  drop it somewhere reviewable — but treat `RheoBoard_V8.BomDoc` as the source of truth, not
  the export.
