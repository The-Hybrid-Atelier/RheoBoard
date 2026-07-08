# Progress log

Running, dated log of what happened and what's next. This is the primary continuity
mechanism between sessions (see `AGENTS.md`) — write entries assuming the next reader
(agent or human) has zero memory of this session.

Newest entries at the top. One entry per session/sitting.

---

## 2026-07-08 (8)

Removed `exec-plans/` and `previous-revisions/` from the harness — both were pure empty
scaffolding (no active/completed plans, no tracked tech debt, no archived revisions ever
actually landed in them across 7 sessions) and the user judged the ceremony wasn't earning its
keep for a solo-dev docs repo at this size. Reversible later if a genuinely large/ambiguous piece
of work (e.g. the laser-cut design work, still TBD) turns out to need a written plan — just
recreate the folder and copy `AGENTS.md`'s old wording out of git history.

- Deleted `BuildYourOwn/exec-plans/` (active/, completed/, tech-debt-tracker.md, _template.md)
  and `BuildYourOwn/previous-revisions/`.
- Updated `AGENTS.md`: dropped both from the repo-layout and `BuildYourOwn/` map sections,
  removed session-bootstrap steps 4–5 (check exec-plans/active, tech-debt-tracker) in favor of a
  single "check product-specs.md if PROGRESS.md's Next note isn't obvious" step, and dropped
  wrap-up step 3 (update/move exec-plan).
- Updated `scripts/check-docs.sh`: removed the "Active exec-plans" check block and renumbered
  the check list in the header comment (now 3 checks instead of 4).
- Updated cross-references: top-level `README.md`, `BuildYourOwn/README.md` (harness table),
  `BuildYourOwn/VERIFICATION.md` (dropped the "move exec-plan to completed/" checklist item),
  `BuildYourOwn/tutorial/README.md` (dropped the Calico-"previous model" → `previous-revisions/`
  doc-model row, since there's no longer an equivalent).
- Left historical `PROGRESS.md` entries below untouched (they're a log of what happened at the
  time, not a living doc) even though several mention exec-plans/previous-revisions in the past
  tense.

**Next:** same as prior entry — capture `images/ide-settings.png` + `images/teaser.png` from a
real build; human-verify wiring/tutorial against a real build; `laser-cut/` design files TBD.

## 2026-07-08 (7)

Corrected the ESP32 board identity, promoted the Qwiic Button to a required part, wired in the
real ThingPlusBLEOSC repo, and rebuilt the wiring diagram at higher resolution:

- **ESP32 Thing Plus variant fix:** the actual part is the **micro-USB** board (WRL-15663, plain
  ESP32-WROOM-32D/E) — earlier session had mistakenly sourced the USB-C variant's photo (WRL-20168,
  ESP32-S3) and referenced "ESP32S3 Dev Module" as the Arduino board. Fixed photo, datasheet
  (vendored schematic + graphical datasheet), and every doc mention (`BOM.md`, `references.md`,
  `software/README.md`, `README.md`, tutorial steps 03/04/06). Added a note in
  `images/components/README.md` distinguishing the two similarly-named SparkFun boards so this
  mistake doesn't recur.
- **Qwiic Button promoted from optional to required** (user confirmed it's part of the system,
  daisy-chained after the MicroPressure sensor on one I2C bus). Added its photo, schematic PDF,
  and BOM row; updated wiring diagram, `README.md`, and tutorial steps accordingly.
- **ThingPlusBLEOSC** real repo found: https://github.com/cearto/ThingPlusBLEOSC (MIT license, by
  cearto). Replaced the "local library, path TBD" placeholder with real install instructions
  (`git clone` into Arduino `libraries/`) and its actual dependencies (OSC by Adrian Freed, ESP32
  BLE Arduino by Neil Kolban) across `references.md`, `software/README.md`, `README.md`, and
  tutorial step 04.
- **Wiring diagram regenerated again** (`wiring/2P1V-wiring-diagram.png`): higher resolution
  (220 dpi vs. 150), electrical-only (confirmed no pneumatic content ever leaked in — the tube
  diagram is a separate file), added the Qwiic Button to the I2C chain, labeled micro-USB
  explicitly, and fixed power-wire routing so it no longer visually cuts through other boxes.
- Vendored new datasheets: `ESP32_Thing_Plus_Schematic.pdf`, `ESP32_Thing_Plus_Graphical_Datasheet.pdf`,
  `Qwiic_Button_Schematic.pdf`, `Honeywell_MPR_Series_Datasheet.pdf`.
- **Wiring diagram redesigned a third time**, matching the layout/style of an older reference
  diagram the user had on hand (title + subtitle banner, a color-coded legend row, a 3-column
  block layout: DC supply/ESP32/Qwiic sensor/Qwiic button stacked on the left, the two L298N
  boards in the middle, color-coded output boxes — including an explicit greyed-out "2nd valve
  channel — not populated" box — on the right). Rewrote the generator (Matplotlib) to record each
  text line's exact y-coordinate per box and route every wire (+12V, GND, GPIO→EN, load wires) to
  land precisely on its labeled row instead of an eyeballed offset, which had caused visible
  misalignment/crossing in the prior pass. Verified the file on disk was already correctly updated
  from the previous regeneration (confirmed via direct pixel read, 3379×2506) — the "still shows
  old picture" report was a stale viewer cache, not a stale file; confirmed no duplicate copies of
  the diagram exist elsewhere in the repo.

**Next:** capture `images/ide-settings.png` + `images/teaser.png` from a real upload; human-verify
the corrected wiring against a real build; laser-cut `laser-cut/` design files still TBD.

Not committed.

## 2026-07-08 (6)

Regenerated the wiring diagram and added a real component photo gallery:

- **`wiring/2P1V-wiring-diagram.png`** regenerated programmatically (matplotlib, precise text —
  not AI image generation, to keep GPIO numbers/labels exact) to reflect the single **12 V**
  supply and **Adafruit 4700** pumps / **Adafruit 4663** valve (was previously dual 6–7 V supplies
  language baked into the old image). Second valve channel (ENB/OUT3-4) now shown explicitly as
  "not populated" rather than "wired but unused."
- Added **`images/components/`** with real vendor product photos: SparkFun ESP32 Thing Plus,
  SparkFun Qwiic MicroPressure, Adafruit 4700 pump, Adafruit 4663 valve (all sourced directly from
  Adafruit/SparkFun product pages or their GitHub hardware repos), plus a generic L298N module
  photo (cropped from a CC BY-SA 4.0 Wikimedia Commons circuit illustration — no single canonical
  vendor page exists for that generic part). Attribution/sourcing in
  `images/components/README.md`.
- Embedded (not just linked) diagrams and component photos directly in `BOM.md`, `README.md`
  (new "Component gallery" section), and `wiring/README.md`.

Not committed.

## 2026-07-08 (5)

Corrected BYO electronics BOM to match actual parts:

- **Pumps:** [Adafruit 4700](https://www.adafruit.com/product/4700) ZR320-02PM air pump/vacuum (not
  4699 peristaltic). Port plumbing sets flow direction; motor polarity does not.
- **Valve:** [Adafruit 4663](https://www.adafruit.com/product/4663) FA0520E — qty 1 active in 2P1V.
- **Power:** single external **12 V** adapter (both L298N motor rails); PWM limits effective drive.
- **Sensor / MCU:** SparkFun Qwiic MicroPressure + ESP32 Thing Plus (unchanged).
- Vendored datasheets: `references/datasheets/ZR320-02PM_4.5V.pdf`, `4663_C14660_DC_6V.pdf`.
- Updated `BOM.md`, `references.md`, `wiring/`, `README.md`, tutorial steps 03 and 06.

**Next:** update wiring diagram image if it still shows dual 6–7 V supplies or 4699 pumps; human-verify
build against revised BOM.

Not committed.

## 2026-07-08 (4)

Integrated **2P1VX** firmware and **2P1V** wiring/plumbing from user's Arduino sketchbook and diagrams:

- Copied firmware into `software/2P1VX/` (`2P1VX.ino`, `PneumaticSystem.*`, `RheoSystem.*`, API README).
- Added `wiring/2P1V-wiring-diagram.png`, `wiring/2P1V-tube-connection.png`, `wiring/pneumatic-plumbing.md`.
- Populated `BOM.md` (2P1V rig parts + datasheet links where available).
- Updated `README.md` (features, hardware, software config, connect/use, status checkboxes).
- Updated `software/README.md`, `references.md` (parts + ThingPlusBLEOSC local lib note).
- Filled tutorial steps 03, 04, 06, 07 with 2P1V-specific content (still needs human verification).

**Next:** capture `images/ide-settings.png` + `images/teaser.png`; human-verify tutorial steps and
`VERIFICATION.md`; decide whether to vendor `ThingPlusBLEOSC` into repo or document install-only.

Not committed.

## 2026-07-08 (3)

Naming and tone cleanup:

- Product names updated to **RheoMap**, **RheoData**, and **SlipAtlas** (replacing SlipTopo)
  across `README.md`, `AGENTS.md`, `BuildYourOwn/README.md`, `product-specs.md`, and tutorial
  step 08.
- Removed "not actively worked on" / "dormant" / "not active" language for the PCB track
  (`RheoBoard_V8_Final/`) — both hardware tracks stay in the repo; BYO remains the agent's
  default focus unless told otherwise. PCB described neutrally in `AGENTS.md`.

Not committed.

## 2026-07-08 (2)

User pointed to [Calico](https://github.com/jsli96/calico) as a closer documentation model than
OpenTheremin alone. Restructured BYO scaffolding to match:

- Rewrote `README.md` as Calico-style master builder doc: table of contents, features, hardware
  (platform + electronics), software configuration, connect/use, tips, plus link-out to
  `tutorial/steps/` for Instructables-depth detail.
- Added `images/` (teaser, IDE settings, project-wide photos — Calico keeps these at repo root).
- Added `previous-revisions/` (Calico's `calico 1.0 (previous model)/` pattern).
- Updated `laser-cut/README.md` with Calico-style parts table + recommended settings block.
- Updated `tutorial/README.md` doc model: Calico primary, OpenTheremin secondary.
- Updated `references.md`, `product-specs.md`, `software/README.md`, `AGENTS.md`.

Still all placeholders — no BOM parts, design files, firmware, or images yet. Not committed.

**Next:** choose MCU/platform and populate `BOM.md` + `README.md` Features section first.

## 2026-07-08

Accessed OpenTheremin V4 sources (GitHub, website download page, local assembly PDF) and
scaffolded RheoBoard BYO to **combine** their split documentation model in one repo:

- **GitHub side** (`Electronics/` + `Software/`): added `software/` folder (firmware placeholder);
  existing `BOM.md`, `wiring/`, `laser-cut/` map to their BOM/schematic/mechanical files (BYO
  uses modules + pictographic wiring instead of a single KiCad PCB).
- **Website/PDF side** (assembly + detailed flash guide): added 8 tutorial step folders
  (01-kit-contents through 08-ready-to-use) inspired by the 7-step OpenTheremin PDF + software
  upload page; step 04 is the combined flash guide.
- `tutorial/README.md`: documentation model table mapping OpenTheremin → RheoBoard paths.
- `references.md`: links to GitHub, website, and local PDF noted.

All step READMEs are scaffold/placeholders only — rheometer-specific content still TBD once
parts and firmware are chosen. Did not commit (per working agreement).

**Next:** define actual BOM parts and rheometer-specific calibration/measurement workflow, then
fill step 01 and 03 first (inventory + wiring depend on part choices).

## 2026-07-07 (8)

Full audit against all three source articles (not just the biggest gap from two sessions ago).
Mapped every concrete recommendation to something in the repo; found and closed three real gaps:

- **Feature-list pattern wasn't applied at the tutorial-step level.** `BuildYourOwn/README.md`
  had a track-level checklist, but `tutorial/README.md`'s step list was just placeholder text
  with no persistent status. Added a checkbox list that's explicit about "written" vs "verified"
  being different states (mirrors the article's "mark passing only after real testing" lesson).
- **Sprint-contract ordering was backwards.** `exec-plans/_template.md` had "Verification" after
  "Plan," so nothing forced agreeing on done-criteria before writing the plan. Renamed to
  "Success criteria" and moved it before "Plan," with an instruction to fill it in first.
- **"Every part is traceable" was prose-only, not mechanically enforced.** Added a check to
  `scripts/check-docs.sh` that scans `BOM.md` table rows for an empty Datasheet column and
  warns. Tested against both an empty table (current state, passes) and synthetic good/bad rows
  (correctly flags only the bad one) before landing it.
- Minor: noted in `AGENTS.md` that future firmware/software should get an `init.sh`-style script
  + smoke-test step, so that lesson isn't lost by the time it's relevant.

Everything else checked out already: AGENTS.md as map, progress log, exec-plans, human-only
verification boundary, golden principles, no-commit-by-agent workflow, repo-local-only content.

**Next:** same as before — start populating `BOM.md` and the first tutorial step.

## 2026-07-07 (7)

Workflow change: **the agent no longer commits or pushes.** The user commits to `main`
themselves, on their own schedule. Updated `AGENTS.md` ("Working agreement" section, session
bootstrap/wrap-up) and `core-beliefs.md` ("small, meaningful commits" bullet) to reflect this:

- Agent edits files locally and stops — no `git add`/`commit`/`push` on the user's behalf unless
  explicitly asked to in the moment.
- `BuildYourOwn/PROGRESS.md` is now the primary continuity mechanism between sessions (more so
  than before) since git log may lag behind the actual working tree state.
- Session bootstrap now treats an uncommitted working tree as expected, not a red flag.

**This very entry is an example of the new flow: written but not committed.** Next session (or
the user) should `git status`/`git diff` to see what's pending from this one.

## 2026-07-07 (6)

Consolidated the entire harness into `BuildYourOwn/`, removing the top-level `docs/` folder.
Prior sessions had drifted toward treating `docs/` and `BuildYourOwn/` as parallel structures,
which read as duplication even after the last cleanup pass — the real fix was to stop having two
top-level trees at all, since BYO is currently the only active track and everything under
`docs/` that wasn't PCB-specific was really about BYO anyway.

- Moved into `BuildYourOwn/` (flat, no subfolder): `PROGRESS.md`, `core-beliefs.md`,
  `product-specs.md`, `references.md`, `exec-plans/` (active/completed/tech-debt-tracker/template).
  Dropped `design-docs/index.md`'s wrapper — it only ever pointed at one file.
- Deleted `docs/hardware/pcb/` (BOM notes, revision history, verification checklist for the
  Altium track) rather than relocating it — that track is dormant, and per the "simplest
  solution, add complexity only when needed" principle, it's cheaper to recreate a small doc set
  later if PCB work resumes than to maintain it unused now. Noted this decision in `AGENTS.md`.
- Deleted `docs/` entirely once empty.
- Rewrote `AGENTS.md` top to bottom to reflect the new layout (repo now has just two top-level
  content areas: `BuildYourOwn/` and `RheoBoard_V8_Final/`, plus `scripts/`).
- Updated `scripts/check-docs.sh` to check `BuildYourOwn/` instead of `docs/` + `BuildYourOwn/`.
- Fixed all cross-references in `README.md`, `BuildYourOwn/README.md`, `VERIFICATION.md`,
  `BOM.md`, `core-beliefs.md`, `product-specs.md`, `exec-plans/_template.md`.

**Next:** same as before — start populating `BuildYourOwn/BOM.md` and the first tutorial step.
If PCB work ever resumes, revisit the "recreate docs there" decision above before assuming this
structure needs to expand back out.

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
