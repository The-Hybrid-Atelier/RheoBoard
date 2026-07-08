# Laser-cut platform — design files

Design files for the laser-cut acrylic panel that mounts every component of this design (pumps, valve,
L298N drivers, ESP32, Qwiic sensor + button) with zip ties — no screws, no enclosure, just a flat
mounting panel.

Each mechanical part is documented here with material, recommended cut settings, and assembly
notes, same spirit as a 3D-printed-parts README but for laser-cut flat stock.

## Panel at a glance

- **Material:** acrylic
- **Dimensions:** 290 × 200 mm
- **Thickness:** 3 mm
- **Mounting:** everything zip-tied through cut slots (no screws, no standoffs)
- **Feet:** rubber/plastic feet at the 4 corner holes
- **Chamber mount:** Ø10 panel-mount bulkhead fitting

## ⚠️ Status: design reference only — vector cut file still needed

The three images below are a **raster design reference** (component placement, cut-geometry
preview, and full system context). They are **not laser-ready** — a laser cutter needs vector
geometry (`.svg`/`.dxf`/`.ai`), and no such vector source has been vendored into this repo yet.
Treat `panel-cut-lines.png` as the spec for what the vector file's geometry should match; produce
the actual vector file before cutting anything, and add it here once it exists (see "File
conventions" below). Nothing in this folder has been physically cut or verified — see `AGENTS.md`
→ "What the agent can and can't verify."

## Files

| File | What it is |
|---|---|
| [`panel-cut-lines.png`](panel-cut-lines.png) | Cut-geometry preview: corner mounting holes + every zip-tie slot, no labels. This is the geometry the real vector file needs to reproduce. |
| [`panel-placement-map.png`](panel-placement-map.png) | Labeled reference: where each component sits, which slots it's zip-tied through, and how many ties per part. Primary reference for [tutorial step 02](../tutorial/steps/02-assemble-platform/). |
| [`panel-system-diagram.png`](panel-system-diagram.png) | Full system diagram (pneumatic tubing + electronics wiring) overlaid on the same panel/mounting scheme, drawn for a **2-valve (2P2V)** variant — see the callout below before using it as wiring truth. |

## Component placement + zip-tie map

Looking at the panel from the front (top view). Colored box = component footprint; red slot =
zip-tie hole (cut); red dashes = zip-tie strap over the part. Full detail in
[`panel-placement-map.png`](panel-placement-map.png).

| # | Part | How it's tied | Status in our build |
|---|---|---|---|
| 1–2 | PUMP1, PUMP2 (Adafruit 4700) | Lie flat; 2 zip-ties across the body (4 slots) each | Populated |
| 3 | VALVE1 (Adafruit 4663) | 2 ties over the body (4 slots) | **Unpopulated / reserved.** Mirrors GPIO 15 being reserved and not populated in [`../hardware/wiring/2P1V-wiring-diagram.png`](../hardware/wiring/2P1V-wiring-diagram.png). Leave this slot empty unless building a 2-valve variant (see callout below). |
| 4 | VALVE2 (Adafruit 4663) | 2 ties over the body (4 slots) | Populated — the only valve driven (GPIO 14) |
| 5 | MPRLS (Qwiic MicroPressure) | 2 ties (4 slots) — next to ESP32 | Populated |
| 6 | Button (Qwiic Button) | 2 ties (4 slots) | Populated |
| 7 | ESP32 Thing Plus | 2 ties over the short sides (4 slots) | Populated |
| 8–9 | L298N #1 (pumps), L298N #2 (valve) | 2 ties each, clear of the heatsink | Populated |
| T | T-connector (shared pneumatic line) | 1 tie at each node on the shared line | Populated |
| PWR | Power terminal block | 1 tie on the terminal block | Populated |

## ⚠️ Panel diagram shows a 2-valve (2P2V) system — this design uses a single valve

[`panel-system-diagram.png`](panel-system-diagram.png) documents a **2P2V** variant: VALVE1 and
VALVE2 each dedicated to one pump and driven independently (GPIO 14 → VALVE1, GPIO 15 → VALVE2).
**That is not our current build.** This design uses a single valve (VALVE2) whose metal/plastic
poles switch one shared line between the two pumps — GPIO 15 stays reserved/unpopulated. The
authoritative pneumatic and electrical reference for what we're actually building is:

- [`../hardware/wiring/2P1V-wiring-diagram.png`](../hardware/wiring/2P1V-wiring-diagram.png) — electrical wiring
- [`../hardware/wiring/pneumatic-plumbing.md`](../hardware/wiring/pneumatic-plumbing.md) — tubing + valve logic
- [`../hardware/wiring/2P1V-tube-connection.png`](../hardware/wiring/2P1V-tube-connection.png) — tube diagram

The panel's *mechanical* layout (dimensions, component positions, hole map) is shared between
both variants — only VALVE1's position, its tubing, and GPIO 15 are unused here. If a 2-valve
variant gets built later, `panel-system-diagram.png` applies directly; update `../hardware/BOM.md`, the
wiring diagram, `pneumatic-plumbing.md`, and `../software/rheometer-firmware/PneumaticSystem.h` together at that
point (per `AGENTS.md`), and record the decision in `PROGRESS.md`.

## Parts

| Part | Material | Thickness | Source file | Used in tutorial step |
|---|---|---|---|---|
| Component panel | Acrylic | 3 mm | [`panel-cut-lines.png`](panel-cut-lines.png) (reference only — vector `.svg`/`.dxf` TBD) | [02](../tutorial/steps/02-assemble-platform/) |

## Recommended cut settings

| Setting | Value | Notes |
|---|---|---|
| Material | Acrylic | |
| Thickness | 3 mm | Must match CAD |
| Panel dimensions | 290 × 200 mm | |
| Kerf compensation | TBD | Measure on your laser once the vector file is cut |
| Power / speed | TBD | Starting point only — set once a physical cut is tested |

## File conventions

- **Vector source** (`.svg` or `.ai`/`.dxf`) is the source of truth for the cut panel — keep it
  next to any exported/print-ready version rather than replacing it. **Not yet added** — only the
  raster reference images above exist so far.
- **`.dxf`** — for feeding directly to a laser cutter that expects DXF.
- **`.pdf`** — print-at-actual-size reference, useful for checking fit without a laser cutter.
- Name files by part, e.g. `panel.svg`, not `Untitled1.svg`.
- Record material, thickness, and kerf compensation used — either as a comment/layer in the file
  itself or in a short `cut-settings.md` here once known. Kerf varies by machine/material, so note
  the machine used too if it's not a generic setting.

## Once files exist

- [ ] Vector source (`.svg`/`.dxf`) produced, matching `panel-cut-lines.png` geometry
- [x] Each part linked to where it's referenced in `../tutorial/` (step 02)
- [ ] Kerf compensation measured and recorded (material + thickness already known: 3 mm acrylic)
- [ ] Cut settings (power/speed) recorded for the specific laser cutter used, flagged as
      "starting point, recalibrate for your machine" rather than gospel
- [ ] Panel physically cut and test-fit against every component in the placement map (human-only
      — see `AGENTS.md`)
