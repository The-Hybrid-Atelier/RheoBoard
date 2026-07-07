# AGENTS.md

This is a map, not a manual. Keep this file short (~100 lines); real depth lives in
`BuildYourOwn/`. If you're an agent (or future-me) starting a session in this repo, read this
file first.

## What this repo is

Open hardware supporting **RheoMap** and **SlipTopo**. Product/spec detail lands in
`BuildYourOwn/product-specs.md` as it's written up; treat that file as the source of truth over
anything said here.

There are two hardware tracks:

1. **Build Your Own / BYO** (`BuildYourOwn/`) — off-the-shelf modules/dev boards on
   breadboard/perfboard, documented with wiring diagrams + BOM + assembly instructions instead
   of CAD. **This is the current focus and holds essentially the entire harness** (progress log,
   exec-plans, core beliefs, references, product specs) — assume work is about this track and
   lives in this folder unless told otherwise.
2. **PCB** (`RheoBoard_V8_Final/`) — a custom Altium-designed board. Not actively worked on.
   It currently has no dedicated process docs (deleted when this track went dormant, per the
   "simplest solution, add complexity only when needed" principle) — if this track picks back
   up, recreate a small doc set for it then (BOM notes, revision history, verification
   checklist — mirror `BuildYourOwn/`'s pattern of keeping docs with the hardware they describe).

## Repository layout

- `BuildYourOwn/` — the harness. Everything lives here directly (plain text/markdown, diffs
  normally): `PROGRESS.md` (session log — read this first), `core-beliefs.md` (operating
  principles), `exec-plans/` (planning), `product-specs.md`, `references.md`, `BOM.md`,
  `VERIFICATION.md`, `laser-cut/`, `wiring/`, `tutorial/` (the Instructables-style build guide —
  the main deliverable).
- `RheoBoard_V8_Final/` — the Altium Designer PCB project: schematic, layout, symbol/footprint
  libraries, BOM, manufacturing outputs. Binary CAD files — Altium is the only thing that opens
  them (see "Hardware-specific notes" below).
- `scripts/` — small dependency-free maintenance scripts (currently just `check-docs.sh`).

## Working agreement (solo dev)

- One developer. Work happens directly on `main` — no feature branches, no PR review gate.
- Commit early and often, with descriptive messages. **Git log +
  `BuildYourOwn/PROGRESS.md` are the only continuity mechanism** between sessions — treat
  writing them well as part of the task, not cleanup afterward.
- Never rewrite published history on `main` (no force-push, no rebasing committed work).
  Recovery from a bad change should always be "read the log, revert/fix forward."

## What the agent can and can't verify

This is physical hardware. Unlike a web app, there's no `curl`/Playwright equivalent for an
agent to confirm a wire is actually connected or a part actually fits — that gap is exactly
where agents in general are most prone to declaring victory too early, so treat it explicitly:

- **Agent-executable:** writing/editing docs, researching and drafting BOM candidates (with
  real product/datasheet links), structuring and drafting tutorial content, describing wiring
  step-by-step, reviewing checklists, git operations.
- **Human-only:** buying/handling parts, cutting, soldering, wiring, powering on, taking
  measurements, filming, and literally every item in `BuildYourOwn/VERIFICATION.md` — those are
  physical checks by definition.
- **Never check off a human-only item yourself, and never write as if a physical step
  succeeded unless a human has reported back that it did.** "The instructions look correct"
  is not verification. If a checklist can't be completed because it needs a human, say so
  explicitly and leave it unchecked/open rather than rounding up.

## Session bootstrap — do this first, every session

1. `git status` — confirm a clean working tree before touching anything.
2. `git log --oneline -20` — see what actually happened recently (don't trust memory/summaries alone).
3. Read `BuildYourOwn/PROGRESS.md` — the running log of what's been done and what's next.
4. Check `BuildYourOwn/exec-plans/active/` — if a plan is in flight, resume it before starting
   something new.
5. If nothing is active, check `BuildYourOwn/exec-plans/tech-debt-tracker.md` and
   `BuildYourOwn/product-specs.md` for the next priority.

## Session wrap-up — do this before ending every session

1. Run `scripts/check-docs.sh` — catches dangling doc links, more than one active exec-plan,
   and doc edits that forgot to update `BuildYourOwn/PROGRESS.md`. Fix what it flags.
2. Append a dated entry to `BuildYourOwn/PROGRESS.md`: what changed, why, what's next.
3. Update the relevant exec-plan, or move it to `BuildYourOwn/exec-plans/completed/` if finished.
4. Commit everything to `main` with a descriptive message.

## `BuildYourOwn/` map

- `PROGRESS.md` — dated session log, the primary continuity mechanism (see working agreement).
- `core-beliefs.md` — operating principles; update it when a recurring mistake/preference emerges.
- `exec-plans/` — plans for any non-trivial chunk of work.
  - `active/` — in-flight plans (should usually contain 0-1 items).
  - `completed/` — finished plans, kept for history.
  - `tech-debt-tracker.md` — known gaps/deferred items that aren't worth fixing right now.
  - `_template.md` — copy this to start a new plan.
- `product-specs.md` — what we're building and why (RheoBoard, RheoMap, SlipTopo).
- `references.md` — external datasheets/standards the design depends on.
- `BOM.md`, `laser-cut/`, `wiring/`, `VERIFICATION.md`, `tutorial/` — see `BuildYourOwn/README.md`.

## Hardware-specific notes

- **BYO (`BuildYourOwn/`) is plain text/markdown** — normal git diffs work, edit it like code.
  Before calling a build revision "done," run it through `BuildYourOwn/VERIFICATION.md`.
- **PCB (`RheoBoard_V8_Final/`) is Altium binaries** (`.PcbDoc`, `.SchDoc`, `.PcbLib`, `.SchLib`,
  `.PrjPcb*`, `.OutJob`, `.Cam`, `.simcfg`) with no meaningful text diff — Altium itself is the
  only editor. If this track becomes active again, **always describe the intent of a hardware
  change in prose** somewhere text-based, since `git diff` won't show it.
  `RheoBoard_V8.BomDoc` is the one exception (text/XML, diffs fine).
- Any future firmware/software for RheoMap/SlipTopo that lands in this repo should get its own
  section here once it exists (build/run/test commands, entry points) rather than being
  discovered ad hoc.
