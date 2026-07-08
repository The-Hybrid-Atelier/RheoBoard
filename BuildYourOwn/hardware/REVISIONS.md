# Hardware revisions

OSHWA certification requires that a physical unit be traceable to the exact design-file revision
it was built from (label the hardware with a version number or release date). This file is that
mapping — bump the revision letter here whenever a change lands that affects fit, wiring, or
part selection, and write the new letter (+ date) on the physical panel with a paint pen /
label before it's used.

Format: **Rev &lt;letter&gt;**, bumped alphabetically (A, B, C, ...) — no need for major/minor
numbers at this scale (single-board, no sub-assemblies to version independently).

| Rev | Date | What changed | Design files at this revision |
|---|---|---|---|
| A | 2026-07-08 (not yet built) | Initial DIY design: 2 pumps + 1 valve, ESP32 Thing Plus, 2× L298N, Qwiic MicroPressure + Button, laser-cut acrylic panel. | [`wiring/2P1V-wiring-diagram.png`](wiring/2P1V-wiring-diagram.png) (+ [`wiring/generate_wiring_diagram.py`](wiring/generate_wiring_diagram.py)), [`wiring/2P1V-tube-connection.png`](wiring/2P1V-tube-connection.png), [`BOM.md`](BOM.md), [`../laser-cut/`](../laser-cut/) |

**Current revision: Rev A — not yet physically built**, so nothing has a label yet. The first
physical unit built from these files should be labeled `RheoBoard DIY — Rev A` plus the build
date; record that date back into this table once it happens (this is a human step — see
`AGENTS.md` → "What the agent can and can't verify").

## When to bump the revision

Bump the letter (and add a row) for any change that would make an existing physical unit's design
files inaccurate — a rewired GPIO, a swapped part with different footprint/specs, a changed panel
layout. Cosmetic doc edits (typo fixes, rewording) don't need a bump. When in doubt, check whether
someone holding an already-built unit would need the new files to understand what they have — if
yes, bump it.
