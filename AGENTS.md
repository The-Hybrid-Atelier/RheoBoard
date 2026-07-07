# AGENTS.md

This is a map, not a manual. Keep this file short (~100 lines); real depth lives in `docs/`.
If you're an agent (or future-me) starting a session in this repo, read this file first.

## What this repo is

RheoBoard — open hardware supporting **RheoMap** and **SlipTopo**. Product/spec detail
lands in `docs/product-specs/` as it's written up; treat that folder as the source of truth
over anything said here.

## Repository layout

- `RheoBoard_V8_Final/` — the Altium Designer project: schematic, PCB layout, symbol/footprint
  libraries, BOM, and manufacturing outputs. These are binary CAD files (see "Hardware-specific
  notes" below) — Altium itself is the only thing that opens/edits them.
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
- `docs/hardware/` — BOM tracking, board revision history, verification checklist.
- `docs/references/` — external datasheets/standards the design depends on.

## Hardware-specific notes

- CAD files (`.PcbDoc`, `.SchDoc`, `.PcbLib`, `.SchLib`, `.PrjPcb*`, `.OutJob`, `.Cam`, `.simcfg`)
  are Altium binaries with no meaningful text diff. **Always describe the intent of a hardware
  change in prose** (in `docs/PROGRESS.md` and the relevant exec-plan) — that prose is the only
  legible record of what changed and why, since `git diff` won't show it.
- `RheoBoard_V8.BomDoc` is text/XML and diffs fine — treat BOM changes like code changes.
- Before calling a board revision "done," run it through
  `docs/hardware/verification-checklist.md`.
- Any future firmware/software for RheoMap/SlipTopo that lands in this repo should get its own
  section here once it exists (build/run/test commands, entry points) rather than being
  discovered ad hoc.
