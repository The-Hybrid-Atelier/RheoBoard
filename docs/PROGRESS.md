# Progress log

Running, dated log of what happened and what's next. This is the primary continuity
mechanism between sessions (see `AGENTS.md`) — write entries assuming the next reader
(agent or human) has zero memory of this session.

Newest entries at the top. One entry per session/sitting.

---

## 2026-07-07 (5)

Re-audited the harness against the three source articles (Anthropic long-running-agent posts,
OpenAI harness engineering post) specifically for gaps, not just structure. Found and fixed:

- **Biggest gap: no equivalent of "verify like a real user."** All three articles rely on the
  agent being able to run the software itself (Playwright, curl) to catch premature "done"
  claims. This agent can't solder, cut, or wire anything — so that failure mode (declaring
  victory too early) was completely unguarded against for physical work. Added a new AGENTS.md
  section, "What the agent can and can't verify," drawing a hard line: agent-executable
  (docs/BOM/tutorial writing) vs human-only (anything physical), with an explicit rule to never
  check off a human-only item without a human having reported back.
- Added a **sign-off block** (verified by / date / notes) to both `BuildYourOwn/VERIFICATION.md`
  and `docs/hardware/pcb/verification-checklist.md`, plus a banner stating every item there is
  human-only.
- Added a **top-level deliverables checklist** to `BuildYourOwn/README.md` (BOM / laser-cut /
  wiring / tutorial steps / physically-built-and-verified) — the adapted version of the
  articles' "feature list" concept, so "is this track actually done" has a concrete answer
  instead of a vibe.
- Added `docs/design-docs/core-beliefs.md` principles: work one deliverable at a time (mirrors
  the "one feature/sprint at a time" lesson), and checklists are a floor, not something to
  quietly water down to make progress look better than it is.
- Added `scripts/check-docs.sh` — a dependency-free script (not CI, just run-by-hand) that
  catches dangling relative markdown links, more than one active exec-plan, and docs/BOM
  changes that forgot to update `docs/PROGRESS.md`. Wired into the AGENTS.md session wrap-up
  checklist. This is the OpenAI post's "enforce mechanically, don't rely on memory" lesson,
  scaled down to something cheap enough for a solo docs-heavy repo.
- Fixed a stale link in `docs/exec-plans/_template.md` left over from the previous session's
  `docs/hardware/build-your-own/` removal (would have been caught by the new script).

**Next:** same as before — start populating `BuildYourOwn/BOM.md` and the first tutorial step.
Run `scripts/check-docs.sh` as part of that work, not just retroactively.

## 2026-07-07 (4)

Cleanup: `docs/hardware/build-your-own/` was redundant with `BuildYourOwn/` itself and got
removed, per the harness principle of stripping load-bearing-less complexity rather than
letting it accumulate.

- `docs/hardware/build-your-own/bom.md` was pure indirection (a pointer + notes about
  `BuildYourOwn/BOM.md`) — merged its notes directly into `BuildYourOwn/BOM.md`.
- `docs/hardware/build-your-own/revision-history.md` duplicated `docs/PROGRESS.md`/git log with
  no added value, since (unlike the PCB track's binary files) `BuildYourOwn/` is plain markdown
  and already diffs natively — deleted, not replaced.
- `docs/hardware/build-your-own/verification-checklist.md` had real unique content but was
  misplaced under `docs/` — moved to `BuildYourOwn/VERIFICATION.md`, living with the hardware
  it checks.
- `docs/hardware/pcb/` is unaffected and still justified: Altium binaries have no meaningful
  diff, so BOM notes + revision history need to live somewhere text-based to be legible at all.
- Updated `docs/hardware/index.md` to explain this asymmetry explicitly, so it doesn't get
  "fixed" back into symmetric-but-redundant structure later.

**Next:** same as before — start populating `BuildYourOwn/BOM.md` and the first tutorial step.

## 2026-07-07 (3)

Fleshed out `BuildYourOwn/` into an Instructables-style tutorial structure, per direction that
the deliverable is a full step-by-step guide (BOM, laser-cut platform files, pictographic
circuit diagrams, assembly video) rather than a single flat assembly doc.

- Added `BuildYourOwn/laser-cut/` — scaffold + conventions for platform design files (vector
  source of truth, DXF for cutting, material/thickness/kerf notes to add once known).
- Updated `BuildYourOwn/wiring/README.md` to recommend pictographic/breadboard-style diagrams
  (e.g. Fritzing) over pure schematics, per explicit request.
- Replaced the flat `BuildYourOwn/ASSEMBLY.md` with `BuildYourOwn/tutorial/`: a
  `README.md` table-of-contents/intro, a `steps/` folder (one subfolder per numbered step, each
  with its own `README.md` + `media/`), and a `_step-template/` to copy when adding a step.
  Decided videos should generally be linked externally (e.g. unlisted YouTube) rather than
  committed to git, to avoid repo bloat — documented in `tutorial/README.md`.
- Updated `BuildYourOwn/README.md`, `AGENTS.md`, and
  `docs/hardware/build-your-own/verification-checklist.md` to match.
- Still all empty/TBD scaffolding — no BOM entries, design files, diagrams, or step content
  written yet.

**Next:** pick the first real step to write (probably "gather materials" or "cut the platform")
once parts/design decisions start landing — likely worth an exec-plan for the first pass at the
whole tutorial rather than trickling steps in ad hoc.

## 2026-07-07 (2)

Learned there are two hardware tracks, not one, and restructured the repo accordingly:

- **Build Your Own (BYO)** — off-the-shelf modules/dev boards, breadboard/perfboard, wiring
  diagrams + BOM + assembly guide instead of CAD. **This is the current focus going forward.**
- **PCB** — the existing Altium project (`RheoBoard_V8_Final/`). Not actively worked on now.

Changes:
- Added `BuildYourOwn/` at repo root (`README.md`, `BOM.md`, `ASSEMBLY.md`, `wiring/`) — all
  empty/TBD scaffolding, nothing built yet.
- Split `docs/hardware/` into `docs/hardware/pcb/` (moved the existing BOM-tracking,
  revision-history, verification-checklist docs here unchanged) and
  `docs/hardware/build-your-own/` (new equivalents, adapted for a no-CAD DIY build — e.g. no
  DRC/ERC, continuity/power-up checks instead). Added `docs/hardware/index.md` as the map
  between the two.
- Updated `AGENTS.md`, `README.md`, `docs/product-specs/index.md` to describe both tracks and
  flag BYO as current focus.
- Did **not** touch `RheoBoard_V8_Final/` itself — left it exactly where it is to avoid any
  risk of breaking internal Altium project references.

**Next:** start populating `BuildYourOwn/BOM.md` once parts are chosen — probably worth opening
an exec-plan for the first BYO revision rather than editing ad hoc.

## 2026-07-07

Set up harness engineering scaffolding for solo, direct-to-`main` development, adapted from
Anthropic's long-running agent harness posts and OpenAI's "Harness engineering" post.

- Added `AGENTS.md` (repo map), `README.md`, `.gitignore`/`.gitattributes` tuned for Altium.
- Added `docs/` structure: `design-docs/`, `exec-plans/` (active/completed/tracker/template),
  `product-specs/`, `references/`, `hardware/` (BOM tracking, revision history, verification
  checklist).
- No hardware/product content authored yet — this session was scaffolding only. Repo still
  only contains `RheoBoard_V8_Final/` (Altium project, imported as-is, no revision history
  captured prior to this point).

**Next:** fill in `docs/product-specs/index.md` with the actual RheoBoard/RheoMap/SlipTopo
context once available, and backfill `docs/hardware/revision-history.md` for the existing
V8 board if worth reconstructing.
