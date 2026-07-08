# Tutorial: Build Your Own Rheometer

An Instructables-style, step-by-step guide that takes someone from zero to a working DIY
rheometer build. This is the primary deliverable of the BYO track — everything else in
`BuildYourOwn/` (`BOM.md`, `laser-cut/`, `wiring/`, `software/`) is a resource this tutorial
links out to.

_Scaffold in place — step folders exist with placeholders; content TBD._

## Documentation model

**Primary reference:** [Calico](https://github.com/jsli96/calico) — one repo README (TOC,
features, hardware, IDE setup, connect/use, tips) + fab folders + firmware + images. Closest
match to what we want.

**Secondary reference:** [OpenTheremin V4](https://github.com/GaudiLabs/OpenThereminV4) — splits
GitHub (CAD/firmware) vs website/PDF (assembly + flash). We already combined those into one repo;
OpenTheremin's 7-step assembly PDF informed the step outline below.

| Source | What it contains | RheoBoard BYO equivalent |
|---|---|---|
| [Calico README](https://github.com/jsli96/calico/blob/main/README.MD) | Master builder doc: features, hardware, Arduino config, tips | [`../README.md`](../README.md) |
| [Calico `3D print models/`](https://github.com/jsli96/calico/tree/main/3D%20print%20models) | Mechanical fab files + settings | [`../laser-cut/`](../laser-cut/) |
| [Calico `PCB files/`](https://github.com/jsli96/calico/tree/main/PCB%20files) | Custom PCB | [`../wiring/`](../wiring/) (BYO modules) or [`../../RheoBoard_V8_Final/`](../../RheoBoard_V8_Final/) (PCB track) |
| [Calico `main_app.ino`](https://github.com/jsli96/calico/blob/main/main_app.ino) | Firmware at repo root | [`../software/`](../software/) |
| Calico root images (`teaser.png`, `esp32-3s-ide-settings.png`, …) | README visuals | [`../images/`](../images/) |
| OpenTheremin PDF assembly | 7-step photo guide | Steps 01–08 below |
| [OpenTheremin download page](https://www.gaudi.ch/OpenTheremin/index.php/download) | Detailed flash steps | Step [`04-install-firmware`](steps/04-install-firmware/) |

Local OpenTheremin PDF: `Instructions_OpenThereminV4.pdf` (user's copy in Downloads, not in repo).

## Overview *(fill in)*

- What you'll build, in a sentence or two, plus a hero photo/video once one exists.
- Estimated build time.
- Difficulty / prerequisite skills (e.g. "comfortable with a soldering iron," "no coding needed").

## Before you start

- **Materials:** see [`../BOM.md`](../BOM.md).
- **Design files:** see [`../laser-cut/`](../laser-cut/) for the platform, [`../wiring/`](../wiring/)
  for circuit diagrams.
- **Software:** see [`../software/`](../software/) for firmware source; flash procedure is step 04.
- **Tools:** *(fill in — e.g. laser cutter access, soldering iron, screwdriver set, computer for
  flashing firmware)*.

## Steps

Each step lives in its own folder under [`steps/`](steps/), numbered in build order. Copy
[`_step-template/`](_step-template/) to start a new one — see `steps/README.md` for the
naming convention.

This list is the source of truth for step status — keep it in sync with `steps/`. A step is
only `[x]` once it's written **and** a human has verified it against `../VERIFICATION.md`/by
building it (see `AGENTS.md` → "What the agent can and can't verify") — "written" and "verified"
are different things, don't collapse them.

- [ ] 01 — [Kit contents and tools](steps/01-kit-contents-and-tools/) — scaffold only
- [ ] 02 — [Assemble the platform](steps/02-assemble-platform/) — scaffold only
- [ ] 03 — [Wire the electronics](steps/03-wire-electronics/) — scaffold only
- [ ] 04 — [Install firmware](steps/04-install-firmware/) — scaffold only
- [ ] 05 — [Mount and set up](steps/05-mount-and-setup/) — scaffold only
- [ ] 06 — [Power and data connections](steps/06-power-and-connections/) — scaffold only
- [ ] 07 — [Calibrate](steps/07-calibrate/) — scaffold only
- [ ] 08 — [Ready to use](steps/08-ready-to-use/) — scaffold only

## Finished

_(fill in once there's a working build — final photos/video, what the builder should be able to
do with it, troubleshooting/FAQ, calibration notes if applicable.)_

## Tips

_Field notes for builders — also mirrored in [`../README.md`](../README.md) → Tips once stable.
Calico keeps tips in the main README; we maintain both so the entry doc stays self-contained._

_TBD._

## Media conventions

- **Images:** commit directly into the relevant step's `media/` folder (PNG/JPG, reasonably
  sized/compressed).
- **Video:** prefer hosting externally (e.g. an unlisted YouTube video) and embedding a link/
  thumbnail, rather than committing large video files to git — keeps the repo cloneable. If a
  clip is very short and small, committing it directly is fine; use judgment.
