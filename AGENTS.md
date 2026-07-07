# AGENTS.md

This is a map, not a manual. Keep this file short (~100 lines); real depth lives in `docs/`.
If you're an agent (or future-me) starting a session in this repo, read this file first.

## What this repo is

Open hardware supporting **RheoMap** and **SlipTopo**. Product/spec detail lands in
`docs/product-specs/` as it's written up; treat that folder as the source of truth over
anything said here.

There are two hardware tracks:

1. **PCB** (`RheoBoard_V8_Final/`) — a custom Altium-designed board. Existing, not actively
   worked on right now.
2. **Build Your Own / BYO** (`BuildYourOwn/`) — off-the-shelf modules/dev boards on
   breadboard/perfboard, documented with wiring diagrams + BOM + assembly instructions instead
   of CAD. **This is the current focus** — assume work is about this track unless told otherwise.

## Repository layout

- `RheoBoard_V8_Final/` — the Altium Designer PCB project: schematic, layout, symbol/footprint
  libraries, BOM, and manufacturing outputs. These are binary CAD files (see "Hardware-specific
  notes" below) — Altium itself is the only thing that opens/edits them.
- `BuildYourOwn/` — the BYO build: `BOM.md` (parts list), `laser-cut/` (platform design files),
  `wiring/` (pictographic circuit diagrams), `VERIFICATION.md` (checklist), `tutorial/` (the
  Instructables-style, step-by-step build guide — the main deliverable of this track). Plain
  text/markdown — diffs and edits normally, so all of this track's process docs live here
  directly rather than under `docs/` (see `docs/hardware/index.md` for why).
- `docs/` — the system of record for everything else. Start at `docs/design-docs/index.md`.

## Working agreement (solo dev)

- One developer. Work happens directly on `main` — no feature branches, no PR review gate.
- Commit early and often, with descriptive messages. Since binary CAD diffs are opaque
  (see below), **git log + `docs/PROGRESS.md` are the only continuity mechanism** between
  sessions — treat writing them well as part of the task, not cleanup afterward.
- Never rewrite published history on `main` (no force-push, no rebasing committed work).
  Recovery from a bad change should always be "read the log, revert/fix forward."

## Session bootstrap — do this first, every session

1. `git status` — confirm a clean working tree before touching anything.
2. `git log --oneline -20` — see what actually happened recently (don't trust memory/summaries alone).
3. Read `docs/PROGRESS.md` — the running log of what's been done and what's next.
4. Check `docs/exec-plans/active/` — if a plan is in flight, resume it before starting something new.
5. If nothing is active, check `docs/exec-plans/tech-debt-tracker.md` and `docs/product-specs/`
   for the next priority.

## Session wrap-up — do this before ending every session

1. Append a dated entry to `docs/PROGRESS.md`: what changed, why, what's next.
2. Update the relevant exec-plan, or move it to `docs/exec-plans/completed/` if it's finished.
3. Commit everything (docs + design/code files) to `main` with a descriptive message.

## Docs map

- `docs/design-docs/` — operating principles and architecture notes. Start at `index.md`.
- `docs/exec-plans/` — plans for any non-trivial chunk of work.
  - `active/` — in-flight plans (should usually contain 0-1 items).
  - `completed/` — finished plans, kept for history.
  - `tech-debt-tracker.md` — known gaps/deferred items that aren't worth fixing right now.
  - `_template.md` — copy this to start a new plan.
- `docs/product-specs/` — what we're building and why (RheoBoard, RheoMap, SlipTopo).
- `docs/hardware/` — start at `index.md`. Only holds a `pcb/` subfolder (BOM tracking notes,
  revision history, verification checklist) — PCB needs it because its files are binary. BYO's
  equivalent docs live directly in `BuildYourOwn/` since that's already plain text.
- `docs/references/` — external datasheets/standards the design depends on.

## Hardware-specific notes

- **BYO (`BuildYourOwn/`) is plain text/markdown** — normal git diffs work, edit it like code.
  Before calling a build revision "done," run it through `BuildYourOwn/VERIFICATION.md`.
- **PCB (`RheoBoard_V8_Final/`) is Altium binaries** (`.PcbDoc`, `.SchDoc`, `.PcbLib`, `.SchLib`,
  `.PrjPcb*`, `.OutJob`, `.Cam`, `.simcfg`) with no meaningful text diff — Altium itself is the
  only editor. **Always describe the intent of a hardware change in prose** (in
  `docs/PROGRESS.md` and the relevant exec-plan) since `git diff` won't show it.
  `RheoBoard_V8.BomDoc` is the one exception (text/XML, diffs fine). Verification checklist:
  `docs/hardware/pcb/verification-checklist.md`.
- Any future firmware/software for RheoMap/SlipTopo that lands in this repo should get its own
  section here once it exists (build/run/test commands, entry points) rather than being
  discovered ad hoc.
