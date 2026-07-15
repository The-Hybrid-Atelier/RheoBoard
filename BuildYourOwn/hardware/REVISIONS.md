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
| A | 2026-07-08 (not yet built) | Initial DIY design: 2 pumps + 1 valve, ESP32 Thing Plus, 2× L298N, Qwiic MicroPressure + Button, laser-cut acrylic panel. | [`wiring/wiring-diagram.png`](wiring/wiring-diagram.png) (+ [`wiring/generate_wiring_diagram.py`](wiring/generate_wiring_diagram.py)), [`wiring/tube-connection.png`](wiring/tube-connection.png), [`BOM.md`](BOM.md), [`../laser-cut/`](../laser-cut/) |
| B | 2026-07-15 (not yet built) | Inserted an Adafruit ATtiny1616 Breakout (seesaw, Qwiic) between the ESP32 and both L298N boards: connected control now runs ESP32 → Qwiic → seesaw pins `0`/`1`/`5` → L298N #1 `ENA`/`ENB` and L298N #2 `ENB`, instead of direct ESP32 GPIO 32/33/14. Seesaw pin `4` and L298N #2 Motor A are reserved/NC in the single-valve build. Firmware renamed `2P1VX.ino` → `2P1V_Adafruit.ino` (BLE device name `2P1V_Adafruit`); requires the Adafruit seesaw Library. Panel gains a draft seesaw placement (`10 SEESAW`, 4 zip-tie slots right of ESP32 / above PWR) in [`../laser-cut/`](../laser-cut/) — still unverified against the real board. | [`wiring/wiring-diagram.png`](wiring/wiring-diagram.png) (+ [`wiring/generate_wiring_diagram.py`](wiring/generate_wiring_diagram.py)), [`BOM.md`](BOM.md), [`../software/rheometer-firmware/`](../software/rheometer-firmware/), [`../laser-cut/`](../laser-cut/) |

**Current revision: Rev B — not yet physically built**, so nothing has a label yet. The first
physical unit built from these files should be labeled `RheoBoard DIY — Rev B` plus the build
date; record that date back into this table once it happens (this is a human step — see
`AGENTS.md` → "What the agent can and can't verify").

## When to bump the revision

Bump the letter (and add a row) for any change that would make an existing physical unit's design
files inaccurate — a rewired GPIO, a swapped part with different footprint/specs, a changed panel
layout. Cosmetic doc edits (typo fixes, rewording) don't need a bump. When in doubt, check whether
someone holding an already-built unit would need the new files to understand what they have — if
yes, bump it.
