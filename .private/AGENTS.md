# AGENTS.md

This is the maintainer/agent map. Builder documentation lives in `BuildYourOwn/`; continuity,
verification, and planning files live beside this file in `.private/`.

## What this repo is

Open hardware supporting **RheoMap**, **RheoData**, and **SlipAtlas**. Product/spec detail lands in
`.private/product-specs.md` as it's written up; treat that file as the source of truth over
anything said here.

There are two hardware tracks:

1. **Build Your Own / DIY** (`BuildYourOwn/`) — off-the-shelf modules/dev boards on
   breadboard/perfboard, documented with wiring diagrams + BOM + assembly instructions instead
   of CAD. **This is the current focus.** Assume work is about this track unless told otherwise.
2. **PCB** (`RheoBoard-PCB_V9/`) — custom Altium-designed board (schematic, layout, symbol/
   footprint libraries, BOM). Binary CAD files — describe intent of changes in prose
   (`.private/PROGRESS.md`) since `git diff` won't show it. PCB-specific process docs can live alongside
   the design in `RheoBoard-PCB_V9/` when needed, mirroring `BuildYourOwn/`'s pattern.

- The repo root `README.md` is the project overview (both tracks: features, hardware, software
  config, connect-and-use, repo layout) — it links into `BuildYourOwn/` for detail rather than
  duplicating it.

## Repository layout

- `BuildYourOwn/` — the harness + DIY build. `README.md` **is** the concise five-step build guide
 itself (single file, no separate `tutorial/` folder) — that's where a
  builder actually starts; the root `README.md` is the reference/overview doc that links into it.
  Also here: `laser-cut/` (cut files), `software/` (code + firmware), `hardware/` (`README.md` BOM,
  `electronic-wiring/`, `tube-wiring/`, `references/` datasheets), `images/` (project and
  component photos).
- `RheoBoard-PCB_V9/` — the Altium Designer PCB project: schematic, layout, symbol/footprint
  libraries, BOM, manufacturing outputs. Binary CAD files — Altium is the only thing that opens
  them (see "Hardware-specific notes" below).
- `.private/` — maintainer/agent guidance, progress, verification, specs, and doc checks.

## Working agreement (solo dev)

- One developer. Work happens directly on `main` — no feature branches, no PR review gate.
- **The agent never commits or pushes.** Make/edit files locally and stop there — the human
  reviews (`git status`/`git diff`) and runs `git commit`/`git push` themselves, always. Don't
  run `git commit`, `git push`, or `git add` on the user's behalf unless explicitly asked to in
  the moment.
- Since commits aren't happening every session, **`.private/PROGRESS.md` is the primary
  continuity mechanism** between sessions (git log matters too, but may lag behind the working
  tree) — treat writing it well as part of the task, not cleanup afterward.
- Never rewrite published history on `main` (no force-push, no rebasing committed work) — this
  is the human's call regardless, but the agent should never suggest or attempt it.

## What the agent can and can't verify

This is physical hardware. Unlike a web app, there's no `curl`/Playwright equivalent for an
agent to confirm a wire is actually connected or a part actually fits — that gap is exactly
where agents in general are most prone to declaring victory too early, so treat it explicitly:

- **Agent-executable:** writing/editing docs, researching and drafting BOM candidates (with
  real product/datasheet links), structuring and drafting tutorial content, describing wiring
  step-by-step, reviewing checklists, read-only git operations (`status`/`log`/`diff`).
- **Human-only:** buying/handling parts, cutting, soldering, wiring, powering on, taking
  measurements, filming, and literally every item in `.private/VERIFICATION.md` — those are
  physical checks by definition.
- **Never check off a human-only item yourself, and never write as if a physical step
  succeeded unless a human has reported back that it did.** "The instructions look correct"
  is not verification. If a checklist can't be completed because it needs a human, say so
  explicitly and leave it unchecked/open rather than rounding up.

## Session bootstrap — do this first, every session

1. `git status` — see what's uncommitted (the user may not have committed the last session's
   work yet — that's expected now, not a red flag).
2. `git log --oneline -20` — see what's actually landed on `main` (don't trust memory/summaries
   alone), but remember the working tree may be ahead of this.
3. Read `.private/PROGRESS.md` — the running log of what's been done and what's next; this
   is more current than git log now that commits are batched/manual.
4. Check `.private/product-specs.md` for the next priority if `PROGRESS.md`'s "Next" note
   doesn't already point somewhere obvious.

## Session wrap-up — do this before ending every session

1. Run `.private/check-docs.sh` — catches dangling doc links and doc edits that forgot to update
   `.private/PROGRESS.md`. Fix what it flags.
2. Append a dated entry to `.private/PROGRESS.md`: what changed, why, what's next.
3. **Stop — do not commit or push.** Leave the changes in the working tree and tell the user
   what changed so they can review and commit themselves.

## `BuildYourOwn/` map

- `README.md` — **the build guide itself**: one file, five numbered steps inline
  (Instructables-style), plus overview/before-you-start/tips at the top. This is where a builder
  starts; there is no separate `tutorial/` folder — it was consolidated into this single file.
- `hardware/` — `README.md` (parts list), `electronic-wiring/` (electronic schematic + generator),
  `tube-wiring/` (pneumatic tube diagram + instructions), and `references/` (vendored datasheets).
- `laser-cut/` — laser-cut platform design files (vector cut files, cut-line reference, and
  placement map).
- `software/` — one README/API reference, with Arduino-compatible MIT-licensed sketch sources
  in `2P1V_Adafruit/` (entry point `2P1V_Adafruit.ino`; folder/file names must match).
- `images/` — project photos, component reference photos, IDE screenshots, and per-step build
  photos named by step (e.g. `step02-panel-placement.jpg`).
- `.private/PROGRESS.md` — dated session log, the primary continuity mechanism.
- `.private/core-beliefs.md` — operating principles.
- `.private/product-specs.md` — internal product/spec notes.
- `.private/VERIFICATION.md` — author-only build and certification checklist.

## Licensing

Three licenses, one per content category (see root `README.md` → License), are declared in one
physical repo-root `LICENSE`: hardware design files (wiring, laser-cut, BOM, PCB) are
**CERN-OHL-W-2.0**; firmware is **MIT**; documentation is **CC BY-SA 4.0**, linked to the canonical
text (https://creativecommons.org/licenses/by-sa/4.0/). The root file contains the complete,
unmodified CERN-OHL-W-2.0 and MIT texts plus clear scope declarations. New firmware files carry an
`SPDX-License-Identifier: MIT` header (see existing files for the pattern); new hardware-design
index files (hardware/electronic-wiring/tube-wiring/laser-cut READMEs) should carry a one-line pointer
to the root `LICENSE`.
Machine-readable open-hardware metadata lives in `okh-RheoBoard.yml` (Open Know-How manifest, repo
root) — keep its `date-updated`, `version`, `made`, license, and design-file paths in sync with
`README.md` whenever those change.
Not yet OSHWA-certified — see `.private/PROGRESS.md` and `.private/VERIFICATION.md` for the
remaining private review and submission tasks.

## Hardware-specific notes

- **DIY (`BuildYourOwn/`) is plain text/markdown** — normal git diffs work, edit it like code.
  Before calling a build revision "done," run it through `.private/VERIFICATION.md`.
- **PCB (`RheoBoard-PCB_V9/`) is Altium binaries** (`.PcbDoc`, `.SchDoc`, `.PcbLib`, `.SchLib`,
  `.PrjPcb*`, `.OutJob`, `.Cam`, `.simcfg`) with no meaningful text diff — Altium itself is the
  only editor. If this track becomes active again, **always describe the intent of a hardware
  change in prose** somewhere text-based, since `git diff` won't show it.
  `RheoBoard_V8.BomDoc` is the one exception (text/XML, diffs fine).
- Any future firmware/software for RheoMap/RheoData/SlipAtlas that lands in this repo should get its own
  section here once it exists (build/run/test commands, entry points) rather than being
  discovered ad hoc — per the original harness articles this should include an `init.sh`-style
  script and a basic smoke-test step in "session bootstrap" once there's something runnable.
