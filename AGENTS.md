# AGENTS.md

This is a map, not a manual. Keep this file short (~100 lines); real depth lives in
`BuildYourOwn/`. If you're an agent (or future-me) starting a session in this repo, read this
file first.

## What this repo is

Open hardware supporting **RheoMap**, **RheoData**, and **SlipAtlas**. Product/spec detail lands in
`BuildYourOwn/product-specs.md` as it's written up; treat that file as the source of truth over
anything said here.

There are two hardware tracks:

1. **Build Your Own / DIY** (`BuildYourOwn/`) — off-the-shelf modules/dev boards on
   breadboard/perfboard, documented with wiring diagrams + BOM + assembly instructions instead
   of CAD. **This is the current focus and holds essentially the entire harness** (progress log,
   core beliefs, references, product specs) — assume work is about this track and lives in this
   folder unless told otherwise.
2. **PCB** (`RheoBoard_V8_Final/`) — custom Altium-designed board (schematic, layout, symbol/
   footprint libraries, BOM). Binary CAD files — describe intent of changes in prose
   (`PROGRESS.md`) since `git diff` won't show it. PCB-specific process docs can live alongside
   the design in `RheoBoard_V8_Final/` when needed, mirroring `BuildYourOwn/`'s pattern.

## Repository layout

- `BuildYourOwn/` — the harness + DIY build. `tutorial/` is where the build actually starts
  (numbered steps); `README.md` is the reference doc the tutorial links out to. Builder-facing:
  `README.md`, `tutorial/`, `laser-cut/` (cut files), `software/` (code + firmware), `hardware/`
  (`BOM.md`, `wiring/`, `images/` component photos, `references/` datasheets — everything
  electrical in one place), `images/` (project-wide photos: teaser, IDE screenshots),
  `VERIFICATION.md`. Agent-facing: `PROGRESS.md`, `core-beliefs.md`, `product-specs.md`.
- `RheoBoard_V8_Final/` — the Altium Designer PCB project: schematic, layout, symbol/footprint
  libraries, BOM, manufacturing outputs. Binary CAD files — Altium is the only thing that opens
  them (see "Hardware-specific notes" below).
- `scripts/` — small dependency-free maintenance scripts (currently just `check-docs.sh`).

## Working agreement (solo dev)

- One developer. Work happens directly on `main` — no feature branches, no PR review gate.
- **The agent never commits or pushes.** Make/edit files locally and stop there — the human
  reviews (`git status`/`git diff`) and runs `git commit`/`git push` themselves, always. Don't
  run `git commit`, `git push`, or `git add` on the user's behalf unless explicitly asked to in
  the moment.
- Since commits aren't happening every session, **`BuildYourOwn/PROGRESS.md` is the primary
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
  measurements, filming, and literally every item in `BuildYourOwn/VERIFICATION.md` — those are
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
3. Read `BuildYourOwn/PROGRESS.md` — the running log of what's been done and what's next; this
   is more current than git log now that commits are batched/manual.
4. Check `BuildYourOwn/product-specs.md` for the next priority if `PROGRESS.md`'s "Next" note
   doesn't already point somewhere obvious.

## Session wrap-up — do this before ending every session

1. Run `scripts/check-docs.sh` — catches dangling doc links and doc edits that forgot to update
   `BuildYourOwn/PROGRESS.md`. Fix what it flags.
2. Append a dated entry to `BuildYourOwn/PROGRESS.md`: what changed, why, what's next.
3. **Stop — do not commit or push.** Leave the changes in the working tree and tell the user
   what changed so they can review and commit themselves.

## `BuildYourOwn/` map

- `tutorial/` — the build guide itself: `README.md` overview + `steps/01`–`08` (numbered,
  Instructables-style). This is where a builder starts.
- `hardware/` — everything electrical in one place: `BOM.md` (parts list), `wiring/` (electrical +
  pneumatic diagrams), `images/` (component photos), `references/` (vendored datasheets).
- `laser-cut/` — laser-cut platform design files (placement map, cut lines, system diagram).
- `software/` — firmware source (`rheometer-firmware/`) and IDE setup notes.
- `images/` — project-wide photos not specific to a hardware part (teaser, IDE screenshots).
- `PROGRESS.md` — dated session log, the primary continuity mechanism (see working agreement).
- `core-beliefs.md` — operating principles; update it when a recurring mistake/preference emerges.
- `product-specs.md` — what we're building and why (RheoBoard, RheoMap, RheoData, SlipAtlas).
- `VERIFICATION.md` — pre-release checklist (human sign-off). See `BuildYourOwn/README.md` for how
  these fit together.

## Hardware-specific notes

- **DIY (`BuildYourOwn/`) is plain text/markdown** — normal git diffs work, edit it like code.
  Before calling a build revision "done," run it through `BuildYourOwn/VERIFICATION.md`.
- **PCB (`RheoBoard_V8_Final/`) is Altium binaries** (`.PcbDoc`, `.SchDoc`, `.PcbLib`, `.SchLib`,
  `.PrjPcb*`, `.OutJob`, `.Cam`, `.simcfg`) with no meaningful text diff — Altium itself is the
  only editor. If this track becomes active again, **always describe the intent of a hardware
  change in prose** somewhere text-based, since `git diff` won't show it.
  `RheoBoard_V8.BomDoc` is the one exception (text/XML, diffs fine).
- Any future firmware/software for RheoMap/RheoData/SlipAtlas that lands in this repo should get its own
  section here once it exists (build/run/test commands, entry points) rather than being
  discovered ad hoc — per the original harness articles this should include an `init.sh`-style
  script and a basic smoke-test step in "session bootstrap" once there's something runnable.
