# BOM tracking (Build Your Own)

Source of truth for the parts list is `BuildYourOwn/BOM.md` (plain markdown/table — diffs
natively in git, no export step needed).

Notes:

- When choosing a part, record the reason and a datasheet/product link in the relevant
  exec-plan and in `docs/references/index.md`.
- Prefer parts that are widely available off-the-shelf (multiple distributors, not
  region-locked) — the whole point of BYO is that someone else can source the same parts.
- Prefer parts with good hobbyist documentation/community support (common dev boards,
  breakout boards with established libraries) over parts that need custom driver work,
  unless there's a documented reason otherwise — see `docs/design-docs/core-beliefs.md`.
