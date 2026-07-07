# Bill of Materials — Build Your Own

Parts list for the DIY build. Add a row per part; link the specific product page/datasheet you
verified against, not just a generic search.

| Part | Qty | Source/link | Datasheet | Notes |
|---|---|---|---|---|

_Empty — no parts selected yet. See `docs/exec-plans/active/` for the plan that will populate this._

## Notes

- When choosing a part, record the reason and a datasheet/product link here and in
  `docs/references/index.md`.
- Prefer parts that are widely available off-the-shelf (multiple distributors, not
  region-locked) — the whole point of BYO is that someone else can source the same parts.
- Prefer parts with good hobbyist documentation/community support (common dev boards, breakout
  boards with established libraries) over parts that need custom driver work, unless there's a
  documented reason otherwise — see `docs/design-docs/core-beliefs.md`.
- This file is plain markdown and diffs natively in git — changes to it show up directly in
  `git log`, so there's no separate revision-history table to maintain (unlike the PCB track's
  binary BOM, which needs one; see `docs/hardware/pcb/`).
