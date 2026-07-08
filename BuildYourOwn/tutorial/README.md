# Tutorial: Build Your Own Rheometer

An Instructables-style, step-by-step guide that takes someone from zero to a working DIY
rheometer build. This is the primary deliverable of the BYO track — everything else in
`BuildYourOwn/` (`BOM.md`, `laser-cut/`, `wiring/`, `software/`) is a resource this tutorial
links out to.

_Content written for all 8 steps; none human-verified yet, and no photos/videos exist for any
step — see the per-step status below._

## Overview

You'll build the **2P1V rig**: a benchtop pneumatic "pull-push" measurement head — 2 air pumps +
1 valve zip-tied to a laser-cut acrylic panel, driven by an ESP32 over BLE, sensed by a Qwiic
MicroPressure sensor. The end result is a device that runs a **REP** (retract → extrude pulse)
on command — from BLE (RheoData), the onboard/Qwiic button, or USB serial — and streams a
pressure trace for each one.

_Hero photo/video: add once a build exists._

- **Estimated build time:** not yet measured end-to-end by a human build. Summing the per-step
  time estimates below gives **~4–7 hours hands-on** for a first-timer, dominated by step 03's
  soldering — plus laser-cut turnaround time on top if that's outsourced rather than done
  in-house (step 02). Treat this as a placeholder until someone times a real build — see
  [`../PROGRESS.md`](../PROGRESS.md) to log the correction.
- **Difficulty / prerequisite skills:** comfortable with a soldering iron (pull-down resistors,
  screw terminals), basic Arduino IDE usage (installing boards/libraries, flashing firmware), and
  reading a wiring diagram. No custom PCB work, no CAD skills required (the panel is cut from a
  provided file) — but laser cutter access (or a cut-to-order service) is required for step 02.

## Before you start

- **Materials:** see [`../BOM.md`](../BOM.md).
- **Design files:** see [`../laser-cut/`](../laser-cut/) for the platform, [`../wiring/`](../wiring/)
  for circuit diagrams.
- **Software:** see [`../software/`](../software/) for firmware source; flash procedure is step 04.
- **Tools:**
  - Laser cutter access, or a cut-to-order service that accepts `.svg`/`.dxf` (step 02 — note the
    vector file itself is still TBD, see `laser-cut/README.md`)
  - Small zip-tie/flush cutters (step 02)
  - Soldering iron + solder, wire strippers, small flathead/Phillips screwdriver for L298N screw
    terminals, multimeter (step 03, step 06)
  - Computer with a free USB port, data-capable micro-USB cable (steps 04, 06)
  - A phone/tablet or computer running **RheoData** for BLE control (steps 06–08)

## Steps

Each step lives in its own folder under [`steps/`](steps/), numbered in build order. Copy
[`_step-template/`](_step-template/) to start a new one — see `steps/README.md` for the
naming convention.

This list is the source of truth for step status — keep it in sync with `steps/`. A step is
only `[x]` once it's written **and** a human has verified it against `../VERIFICATION.md`/by
building it (see `AGENTS.md` → "What the agent can and can't verify") — "written" and "verified"
are different things, don't collapse them. **No step has photos or video yet regardless of text
status** — every step's `media/` folder is currently empty.

- [ ] 01 — [Kit contents and tools](steps/01-kit-contents-and-tools/) — written, not verified; no media
- [ ] 02 — [Assemble the platform](steps/02-assemble-platform/) — written, not verified; blocked on laser-cut vector file; no media
- [ ] 03 — [Wire the electronics](steps/03-wire-electronics/) — written, not verified; no media
- [ ] 04 — [Install firmware](steps/04-install-firmware/) — written, not verified; no media
- [ ] 05 — [Mount and set up](steps/05-mount-and-setup/) — written, not verified; blocked on RheoMap's sample/fixture geometry spec; no media
- [ ] 06 — [Power and data connections](steps/06-power-and-connections/) — written, not verified; no media
- [ ] 07 — [Calibrate](steps/07-calibrate/) — written, not verified; no media
- [ ] 08 — [Ready to use](steps/08-ready-to-use/) — written, not verified; no media

## Finished

_(fill in once there's a working build — final photos/video, what the builder should be able to
do with it, troubleshooting/FAQ, calibration notes if applicable.)_

## Tips

_Field notes for builders — also mirrored in [`../README.md`](../README.md) → Tips once stable.
We maintain both copies so each entry doc stays self-contained. Full context for each is in the
step it's tagged with._

- **(01)** Don't start wiring until every `BOM.md` row is accounted for — discovering a missing
  part mid-build is far more costly than catching it during inventory.
- **(02)** Leave the VALVE1 zip-tie slot empty — it's a reserved position for a future 2-valve
  variant, not part of this 2P1V build.
- **(03)** L298N ENA/ENB jumpers must be **removed** — the ESP32 drives those pins with PWM, and a
  jumper would fight it.
- **(03)** Pumps are ~4.5 V parts riding on a 12 V motor rail; effective drive is set by firmware
  PWM duty, not adapter voltage. Don't run either pump at 100% duty continuously — Adafruit rates
  the 4700 for ~50%.
- **(04)** Charge-only micro-USB cables won't expose a serial port — use a known data-capable one.
- **(04)/(06)** If the MPRLS or Qwiic Button seems missing or misbehaving, check Qwiic daisy-chain
  order and reseat the cable before suspecting the part itself.
- **(06)** Never route pump/valve current through the ESP32's 5 V pin — it's a logic supply, not a
  motor rail. Motors get their own 12 V adapter.
- **(07)** If extrude feels too aggressive, lower `rheo/rep/push/power` or lengthen
  `push/ramp/time` before shortening `push/time` — a longer ramp is gentler than a shorter pulse.
- **(08)** The onboard boot button (GPIO 0), the Qwiic Button, BLE (`rheo/rep`), and USB serial
  (`REP`) all trigger the same REP routine — pick whichever's convenient at the bench.

## Media conventions

- **Images:** commit directly into the relevant step's `media/` folder (PNG/JPG, reasonably
  sized/compressed).
- **Video:** prefer hosting externally (e.g. an unlisted YouTube video) and embedding a link/
  thumbnail, rather than committing large video files to git — keeps the repo cloneable. If a
  clip is very short and small, committing it directly is fine; use judgment.
