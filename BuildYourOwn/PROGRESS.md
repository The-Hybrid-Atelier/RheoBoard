# Progress log

Running, dated log of what happened and what's next. This is the primary continuity
mechanism between sessions (see `AGENTS.md`) — write entries assuming the next reader
(agent or human) has zero memory of this session.

Newest entries at the top. One entry per session/sitting.

---

## 2026-07-15 (44)

Shortened the builder guide from eight steps to five meaningful stages:

1. Assemble the platform.
2. Connect electronics and tubing.
3. Install firmware.
4. Run the power-on test.
5. Calibrate and use.

Moved the obvious inventory/tool check into `Before you start`, merged generic mounting/setup into
power-on and use, and removed repeated operating explanations. Updated every active step reference
in the root README, laser-cut/electronic-wiring/software guides, `AGENTS.md`, and
`okh-RheoBoard.yml`. Historical progress entries retain the former eight-step names.

**Next:** no documentation-only work; physical verification remains open.

---

## 2026-07-15 (43)

Focused every active `BuildYourOwn/**/README.md` on its own folder and removed duplicated
instructions:

- `BuildYourOwn/README.md` is now only the build workflow. Detailed cutting, wiring,
  firmware setup/API, and repeated tip summaries were replaced with direct folder links.
- `hardware/README.md` is only the BOM plus procurement/substitution notes.
- `hardware/electronic-wiring/README.md` owns electrical terminals, power rules, pin mapping,
  startup-risk notes, and diagram regeneration.
- `hardware/tube-wiring/README.md` owns pneumatic ports, valve paths, and REP flow behavior.
- `software/README.md` owns toolchain setup, dependencies, upload, firmware pin constants, API
  parameters/defaults, timing, and serial commands. Removed the machine-specific sketchbook path
  and screenshot TODO.
- `hardware/references/README.md` now indexes only hardware references/datasheets;
  `images/README.md` now contains only image identification and attribution.
- Clarified that the pump's manufacturer-recommended ~50% duty cycle means intermittent run time,
  not a 50% instantaneous PWM ceiling.

License pointers and short safety checks remain where they are needed for standalone use.

**Next:** physically verify the build and panel; no documentation-only check can complete those
human sign-offs.

---

## 2026-07-15 (42)

Rewrote `laser-cut/README.md` as a concise, builder-facing laser-cut instruction instead of a
design-history/status document. Reduced it from 112 lines to a direct workflow: specifications,
file choice, 1:1 paper/cardboard test fit, kerf test, machine-specific cutting, post-cut checks,
component placement, and vector regeneration. Preserved the prominent warning that the draft has
not been physically cut or verified against real parts; no human-only checks were marked complete.

**Next:** after the first physical cut, record the machine, power/speed, measured kerf, and fit
corrections.

---

## 2026-07-15 (41)

Removed `laser-cut/panel-system-diagram.png` at the user's request because it documented a
different 2-pump/2-valve variant and could mislead builders of the current single-valve design.
Removed its file-table row and the full 2P2V explanatory section from `laser-cut/README.md`, and
updated `AGENTS.md`'s laser-cut folder map. The current `panel-placement-map.png`, electronic
wiring, and tube-wiring instructions remain authoritative.

**Next:** none from this cleanup.

---

## 2026-07-15 (40)

Consolidated all project license notices into the single repository-root `LICENSE`, as requested.

- Removed `BuildYourOwn/software/LICENSE`.
- Added an explicit scope notice to root `LICENSE`: CERN-OHL-W-2.0 for hardware, MIT for firmware,
  and CC BY-SA 4.0 for documentation.
- Preserved the complete, unmodified CERN-OHL-W-2.0 text and appended the complete MIT text.
- Updated the root/software READMEs, verification checklist, `AGENTS.md`, and all firmware
  copyright-header pointers. Firmware retains `SPDX-License-Identifier: MIT` headers.

**Next:** none from this consolidation.

---

## 2026-07-15 (39)

Simplified the hardware documentation at the user's request:

- Removed `hardware/REVISIONS.md`.
- Renamed `hardware/BOM.md` to `hardware/README.md`, making the BOM the folder's automatic landing
  page on GitHub.
- Updated all live BOM and revision-file links across the build guide, root README, wiring and
  laser-cut guides, image attribution, verification checklist, `AGENTS.md`, Open Know-How
  manifest, and `scripts/check-docs.sh`.
- Kept the current `Rev B` version identifier in `okh-RheoBoard.yml` and existing design
  annotations, but `PROGRESS.md` is now the only narrative change history. The first physical unit
  still needs a version/date label for OSHWA readiness.

**Next:** none from this simplification; physical test-fit, build labeling, and OSHWA submission
remain human-only.

---

## 2026-07-15 (38)

Consolidated the duplicate image locations into one `BuildYourOwn/images/` folder, as requested.
Moved all seven component reference photos out of `hardware/images/` and removed that empty folder;
`teaser.jpg` remains in the same top-level images folder.

- Moved the component-photo source/attribution README to `BuildYourOwn/images/README.md`, shortened
  its introduction, and added the teaser photo to its inventory. This attribution content is kept
  because the component images come from vendors and Wikimedia.
- Updated the BOM thumbnails and attribution links, root README component gallery and repository
  tree, electronic-wiring guide, verification checklist, and `AGENTS.md` to use the consolidated
  location.
- Historical progress entries retain their original image paths.

**Next:** none from this consolidation.

---

## 2026-07-15 (37)

Removed the redundant `BuildYourOwn/software/rheometer-firmware/` documentation layer at the
user's request. Merged its API README into `software/README.md`, leaving one concise source for
toolchain setup, dependencies, upload steps, seesaw/L298N pin map, OSC parameters, REP timing,
serial commands, and Qwiic Button gestures.

Arduino requires the sketch folder and primary `.ino` filename to match. After that constraint was
confirmed from Arduino's official sketch specification, the user chose the compatible layout:

- `software/README.md` — setup + API
- `software/LICENSE` — MIT license
- `software/2P1V_Adafruit/` — sketch and four `.h`/`.cpp` files, with matching
  `2P1V_Adafruit.ino` entry point

Updated all living software references in the root/build READMEs, hardware docs,
electronic/tube wiring guides, laser-cut guide, verification checklist, `AGENTS.md`, and
`okh-RheoBoard.yml`.
Source license-header pointers now use `../LICENSE`. Historical progress entries retain their old
paths because they describe the layout at the time.

**Next:** none from this consolidation.

---

## 2026-07-15 (36)

Removed `BuildYourOwn/images/README.md` at the user's request. It was a verbose inventory of the
existing `teaser.jpg` plus three nonexistent/TBD image placeholders (`platform.png`, `wiring.png`,
`ide-settings.png`); no living file linked to this README, so deleting it removes scaffolding
without losing build instructions, image attribution, or a required cross-reference. Future
builder photos can still be added directly under `BuildYourOwn/images/` and linked where used.

**Next:** none from this cleanup.

---

## 2026-07-15 (35)

Split the combined `hardware/wiring/` folder into two sibling folders at the user's request:

- `hardware/electronic-wiring/` now contains `wiring-diagram.png`,
  `generate_wiring_diagram.py`, and a focused electronic-wiring `README.md`.
- `hardware/tube-wiring/` now contains `tube-connection.png` and a focused `README.md` that
  incorporates the former `pneumatic-plumbing.md` instructions, component port map, valve logic,
  REP cycle, and diagram-source note.
- Updated all live links and path declarations across the root/build READMEs, BOM, laser-cut
  guide, firmware README, verification checklist, hardware revision mapping, Open Know-How
  manifest, image index, and `AGENTS.md`. Historical progress entries retain their original paths.

This is an organizational/documentation change only; the diagrams, wiring, and physical design did
not change, so Rev B remains current.

**Next:** none from this folder split.

---

## 2026-07-15 (34)

Audited every living `README.md` and the builder instructions under `BuildYourOwn/` against the
current Rev B firmware, BOM, wiring, and laser-cut placement. The core Rev B facts were already
consistent: sketch/BLE name `2P1V_Adafruit`, seesaw `0x49`, connected pins `0`/`1`/`5`, reserved
pin `4`, three Qwiic cables, `10 SEESAW` panel placement, and ~22 zip ties.

- Fixed `software/README.md`'s one substantive signal-path error: the ESP32 sends I2C commands to
  the seesaw over Qwiic, but seesaw-to-L298N `ENA`/`ENB` controls are discrete wires. Also renamed
  the Qwiic Button purpose from “onboard” to “external.”
- Fixed `hardware/BOM.md` saying MPRLS was the “first” Qwiic device; physical device order does
  not matter on the shared I2C bus.
- Tightened `BuildYourOwn/README.md`: corrected overview typos, made the conditional soldering
  requirement explicit, removed the misleading “cable/order” troubleshooting phrase, and changed
  Step 06 so the ESP32/seesaw/MPRLS logic is verified over USB before the 12 V motor rail is
  energized. The 12 V step now explicitly watches for unexpected actuator motion because the
  seesaw power-up pin state remains physically unverified.
- Updated the firmware's missing-Qwiic-Button serial message: the code can continue without it,
  but it is required for the documented external-button controls (no longer called optional).
- Updated `VERIFICATION.md` to describe the panel accurately: columns A–E are traced geometry;
  Rev B seesaw column F is an explicit draft addition.
- Re-swept all `BuildYourOwn/**/README.md` files: no live stale `2P1VX`, direct ESP32 GPIO
  14/15/32/33 instructions, ~20 zip-tie count, “no seesaw placement” note, or Qwiic-carried
  ENA/ENB claim remains. Historical `PROGRESS.md` entries were intentionally left unchanged.
- Follow-up audit fixed the stale software “Connect and use” target, removed the deleted “Media
  conventions” cross-reference, clarified seesaw I2C commands versus discrete EN wires, and
  updated living PCB links from removed `RheoBoard_V8_Final/` to `RheoBoard-PCB_V9/`.
  `scripts/check-docs.sh` now passes all checks. IDE diagnostics report no errors. No firmware
  compiler (`arduino-cli`) is installed, but the only firmware edit is a Serial message string.

**Next:** human test-fit/cut/bring-up remains required. During first 12 V power-up, specifically
watch for L298N output glitches before treating the seesaw idle-state question as resolved.

---

## 2026-07-15 (32)

Added the Rev B **ATtiny1616 seesaw placement** to the laser-cut panel and swept the related
docs so the panel, build guide, BOM, revisions, and OKH manifest agree.

- **Geometry** (Adafruit Eagle outline for PID 5690: **12.7 × 30.48 mm**): new column F in
  `laser-cut/generate_panel_vector.py` — four zip-tie slots at (240/264, 128/148) mm, board
  centered at (252, 138) mm, long axis horizontal, right of ESP32 / above PWR. Regenerated
  `panel.svg` / `panel.dxf`. Column F is an explicit addition (not traced from the original
  `panel-cut-lines.png`); still draft / not human test-fit.
- **Rasters:** extended the generator with `--rasters` to (1) draw the new slots onto
  `panel-cut-lines.png` and (2) overlay footprint + slots + dashed ties + `10 SEESAW` label +
  legend line onto `panel-placement-map.png` (calibrated from existing ESP32/PWR/chamber
  features). Re-running `--rasters` stacks overlays — restore those PNGs from git first.
- **Docs:** `laser-cut/README.md` (row #10, status callout, regen command), Step 02 in
  `BuildYourOwn/README.md`, root `README.md` platform blurb, `hardware/REVISIONS.md` Rev B,
  `hardware/BOM.md` zip-tie qty ~20→~22, `okh-RheoBoard.yml` (removed "no seesaw slot yet").
- Did **not** touch `panel-system-diagram.png` (still the dormant 2P2V overlay reference).
- `scripts/check-docs.sh`: only pre-existing `README.md -> RheoBoard_V8_Final/` false-positive.

**Next:** human test-fit of the draft panel (including the new seesaw slots) against real parts,
then cut/label a Rev B unit. Seesaw boot-time pin state remains an open hardware question
(BOM/wiring notes). OSHWA form still unsubmitted.

---

## 2026-07-15 (33)

Updated `BuildYourOwn/README.md`'s Overview wording exactly as supplied by the user: replaced the
benchtop "pull-push" description with "pneumatic retraction-extrusion," identified the panel as
laser-cut acrylic, split the REP sensing routine into its own paragraph, removed the build-time
estimate, and changed the prerequisite wording to say basic soldering is not needed. Also removed
the closing "Media conventions" section at the user's request.

**Next:** none from this copy edit.

---

## 2026-07-15 (32)

Follow-up to entry 31: the user pointed at their own `wiring_diagram.py` (in the external
`2P1V_Adafruit` sketchbook folder that entry 31 sourced the firmware from) and asked for
`hardware/wiring/wiring-diagram.png` to be regenerated from **that** script's layout/style
instead — "just change title and couple notes, keep everything else." Entry 31's from-scratch
schematic-sheet redesign (sheet border, revision title block, and IN1/IN3 direction-pin detail)
is now superseded by this simpler style.

- Replaced `hardware/wiring/generate_wiring_diagram.py` wholesale with a port of the user's
  source script: rounded `FancyBboxPatch` component boxes (ESP32 → Qwiic bus → Button/seesaw/
  MicroPressure → seesaw pin-fanout → 2× L298N → 4 actuator boxes), a legend bar, and a
  pin-map reference table — no sheet border/grid.
- Initially changed only the title (`"2P1V_Adafruit — ESP32 Thing Plus + ..."` →
  `"RheoBoard DIY — Electrical Schematic"`) and the two spots that named the external
  `2P1VX`/`2P1V_GPIO` sketch variants by name (subtitle + footer note), reworded to generic
  "direct ESP32 GPIO" / "earlier direct-GPIO builds" phrasing per this repo's existing
  generic-naming convention (entry 14). Kept the Pillow palette-quantize post-step (not in the
  source script) since it's this repo's established convention for keeping embedded PNGs small.
- After reviewing that render, the user asked to restore the missing power information and fix
  the top text overflowing its box. Widened the same source layout to add a left-hand **12 V DC
  supply** block and +12 V connections to both L298N motor rails. The first revision used a long
  black common-GND route; at the user's request, replaced that with standard three-bar ground
  symbols at the supply, ESP32, and both L298Ns (repeated symbols denote one common net).
  Enlarged the top information box so the full design summary and five-item legend (I2C,
  control, load, +12 V, GND) are contained inside it.
- Expanded both L298N blocks to account for every terminal (`ENA`, `ENB`, `IN1`–`IN4`,
  `OUT1/2`, `OUT3/4`, `12V`, `5V`, `GND`) and explicitly state that both ENA/ENB jumper caps
  are OFF. This exposed and fixed an earlier documentation inconsistency: because VALVE2 uses
  Motor B (`ENB`), it must connect to `OUT3/OUT4`, not `OUT1/OUT2`. L298N #2 Motor A is wholly
  unused (`ENA`, `IN1`, `IN2`, `OUT1/OUT2` = NC); seesaw pin `4` is reserved/NC rather than
  physically wired. Synchronized the build guide, wiring README, BOM, firmware README/comments,
  hardware revision row, and laser-cut variant note with this terminal-level map.
- Clarified the ATtiny1616 breakout's power pins: Qwiic supplies its 3.3 V and common GND, so
  the separate `Vin` header pin is NC. Added an explicit ground symbol at the breakout and
  synchronized that note across the diagram source, wiring README, build guide, BOM, and
  firmware README.
- Clarified the three different L298N jumper caps after reviewing the user's module photo:
  keep each module's `5V-EN` regulator jumper ON, remove its ENA/ENB jumper caps, and use its
  local +5 V output for the direction-input highs. Added this directly inside both L298N blocks
  and synchronized the wiring README, build guide, BOM, and firmware README; also warned not to
  parallel the two onboard-regulator outputs or apply external 5 V while `5V-EN` is installed.
- Regenerated `wiring-diagram.png` from the new script and visually reviewed it. Did not touch
  unrelated hardware geometry or physical-verification status.
- Re-ran `scripts/check-docs.sh`: same single pre-existing `README.md -> RheoBoard_V8_Final/`
  flag as entry 31, nothing new.

**Next:** none outstanding from this follow-up.

## 2026-07-15 (31)

Followed a user-supplied firmware update (`~/Documents/Arduino/RheoData/thingplus/2P1V_Adafruit`,
outside this repo) and, per explicit confirmation, made it the **replacement** for the current
build (not an additional variant) — bumping the hardware revision to **Rev B**. The change: an
**Adafruit ATtiny1616 Breakout with seesaw** (STEMMA QT/Qwiic, PID 5690, I2C `0x49`) is inserted
between the ESP32 and the two L298N drivers. `ENA`/`ENB` control (`PUMP1_EN`/`PUMP2_EN`/
`VALVE1_EN`/`VALVE2_EN`) now runs ESP32 → Qwiic (I2C) → seesaw pins `0`/`1`/`4`/`5` → discrete
wires → L298N, instead of straight to native ESP32 GPIO 32/33/15/14. Seesaw exposes real 8-bit PWM
over I2C, so proportional pump control is unchanged — only the low-level HAL primitives moved.

- **Firmware** (`software/rheometer-firmware/`): renamed `2P1VX.ino` → `2P1V_Adafruit.ino`
  (`DEVICE_NAME`/Serial banner updated to match); rewrote `PneumaticSystem.h`/`.cpp` to use
  `Adafruit_seesaw` (`ss.analogWrite`/`ss.digitalWrite`/`ss.pinMode` instead of `ledcWrite`/
  `digitalWrite`) — dropped the now-unused LEDC PWM helpers. `RheoSystem.h`/`.cpp` are
  byte-for-byte unchanged (REP phase machine/params never touched native pins). While touching all
  5 firmware files, also fixed a pre-existing stale license-header path
  (`../../../LICENSE-SOFTWARE.txt`, a file removed in entry 30's license consolidation but never
  updated in the firmware comments) to `LICENSE` (same directory).
- Rewrote `software/rheometer-firmware/README.md` (added a "Hardware / wiring" section covering
  the seesaw pin map and the Adafruit seesaw Library dependency) and `software/README.md`
  (toolchain library table, sketchbook path, device name) — same "keep generic phrasing, keep the
  literal device-name string only where technically required" convention as entry 14's earlier
  2P1VX rename.
- **`hardware/BOM.md`**: added the seesaw breakout row (link, address, notes); vendored a real
  product photo (`hardware/images/adafruit-attiny1616-seesaw.jpg`, fetched from
  `cdn-shop.adafruit.com/970x728/5690-00.jpg`, cited in `images/README.md`); bumped Qwiic cable
  qty 2→3 (three daisy-chained peripherals now, not two); removed the "10 kΩ pull-down resistor
  ×4" row (those protected ESP32 pins that are no longer wired to anything) and added a note
  flagging that the seesaw board's own boot-time pin state is **not confirmed** — the source docs
  for this build don't mention an equivalent safeguard, and that's a real hardware question this
  agent can't verify (see `AGENTS.md`).
- **`hardware/wiring/generate_wiring_diagram.py`**: restructured the schematic — ESP32 box now
  only needs Qwiic + GND (no more GPIO32/33/14/15* pins); added an "ATtiny1616 (seesaw)" box on
  the Qwiic bus row alongside Button/MicroPressure; its 4 output pins route down and outside the
  L298N box footprints (added a small `route_ctrl()` helper) instead of cutting across them; made
  the `box()` title/subtitle text positioning use fixed offsets from the top edge instead of
  height-fraction offsets, so it no longer breaks on short boxes (needed for the resized actuator
  row). Regenerated `wiring-diagram.png`; visually spot-checked via cropped-region reads before
  accepting it (seesaw pin fan-out, actuator stack, title block placement). Updated
  `wiring/README.md`'s prose summary and `wiring/pneumatic-plumbing.md`'s valve-logic table
  (`GPIO 14` → `seesaw pin 5`) to match.
- **`BuildYourOwn/README.md`** (Steps 03/04/06/08, Tips) and root **`README.md`** (Features,
  Software configuration, Connect and use, Tips, component gallery): swept every `2P1VX` /
  `GPIO 32/33/14/15` reference to the seesaw-based equivalent; added the Adafruit seesaw Library
  install step; added the seesaw breakout to the component gallery.
- `hardware/references/README.md`: added the ATtiny1616 breakout + Adafruit seesaw Library rows.
- `hardware/REVISIONS.md`: added the **Rev B** row describing this change; bumped
  `okh-RheoBoard.yml`'s `version`/`date-updated`/`description` to match; updated
  `VERIFICATION.md`'s "Rev A" readiness-checklist mention to Rev B.
- `laser-cut/README.md`: flagged (didn't fabricate) that the seesaw board has **no placement slot
  yet** in `panel-placement-map.png`/`panel.svg` — those files are still the pre-existing
  unverified draft, and adding a real slot needs the same human test-fit pass already blocking the
  rest of the vector file, not an agent-drawn guess. Also swapped its two `GPIO 14`/`GPIO 15`
  callouts (2P2V variant discussion) for the seesaw pin numbers.
- Ran `scripts/check-docs.sh` — the only remaining flag (`README.md -> RheoBoard_V8_Final/`) is a
  pre-existing directory-link false-positive from before this session (confirmed via
  `git show HEAD:README.md`), not something introduced here.
- **Did not touch:** `laser-cut/panel.svg`/`.dxf`/`panel-placement-map.png` geometry (see above),
  `panel-system-diagram.png` (still documents the dormant 2-valve variant with its own separate
  callout), or the sibling `2P1V_Sparkfun`/`2P1V_GIPO`/`2P1V_QMD` builds mentioned in the source
  sketchbook's comments — those aren't part of this repo and weren't asked for.

**Next:** the seesaw board's own power-up pin state (see BOM/wiring notes above) and the
laser-cut placement slot are both open, human-only follow-ups. Otherwise this brings the DIY track
back to fully self-consistent docs at Rev B — same remaining gaps as before (panel not test-fit/
cut, OSHWA self-certification not submitted).

---

## 2026-07-08 (30)

User repeatedly asked why there were 4 (root-level) / 8 (repo-wide) files with "LICENSE" in the
name, then asked to consolidate to match how small open-hardware repos actually do it, citing two
reference repos ([GaudiLabs/OpenThereminV4](https://github.com/GaudiLabs/OpenThereminV4),
[BadenLab/LED-Zappelin](https://github.com/BadenLab/LED-Zappelin)). Fetched both repos' actual
`LICENSE`/README/`okh-*.yml` files: OpenThereminV4 uses a single `LICENSE` (GPL-3.0, one license
for everything); LED-Zappelin declares 3 different per-category licenses (hardware CERN-OHL v1.2,
software GPL-3.0, docs CC-BY-SA — their own manifest mislabels software/docs swapped, a flaw not a
pattern to copy) but still only vendors **one** physical `LICENSE` file — the others are declared
in prose/manifest only, not duplicated as separate text files, and neither repo colocates copies
in every subfolder. This directly contradicted this repo's prior (session 25-ish) approach of
colocating full duplicate license-text copies in every folder they covered.

Consolidated from 8 files down to 2:

- **Removed** (all were exact-duplicate or redundant text): `LICENSE-HARDWARE.txt` (root — root
  `LICENSE` already had this text verbatim), `LICENSE-SOFTWARE.txt` (root — duplicate of
  `BuildYourOwn/software/rheometer-firmware/LICENSE`), `LICENSE-DOCUMENTATION.txt` (root — CC
  BY-SA is a link-and-attribute license, not normally vendored as a local file; folding into
  README prose instead, matching how CC licenses are typically handled),
  `RheoBoard_V8_Final/LICENSE`, `BuildYourOwn/hardware/LICENSE`, `BuildYourOwn/laser-cut/LICENSE`
  (all three were identical copies of the root file).
- **Kept:** root `LICENSE` (CERN-OHL-W-2.0, full text — hardware, and what GitHub's detector
  picks up) and `BuildYourOwn/software/rheometer-firmware/LICENSE` (MIT, full text) — kept
  separate specifically because the firmware's actual license differs from the root license, so
  this one isn't a duplicate, it's necessary.
- **Updated references** so nothing dangles: `README.md` (intro paragraph, repo-layout tree,
  License section table — CC BY-SA row now links straight to
  https://creativecommons.org/licenses/by-sa/4.0/ instead of a local file), `AGENTS.md` (Licensing
  section rewritten for the 2-file structure), `BuildYourOwn/hardware/wiring/README.md`,
  `BuildYourOwn/laser-cut/README.md`, `BuildYourOwn/hardware/BOM.md` (all three pointed at
  `LICENSE-HARDWARE.txt`, now point at the root `LICENSE`), and `BuildYourOwn/VERIFICATION.md`
  (OSHW Definition walkthrough item 2, and the OSHWA-readiness checklist's license-file item —
  same underlying fact, updated wording/paths only, left checked since the requirement itself
  didn't change). `okh-RheoBoard.yml` needed no change (it declares license names abstractly, no
  file paths). Ran `scripts/check-docs.sh` after — no dangling links.
- Per the user's separate standing note this session: the actual license choice/text still isn't
  approved yet — this was purely a structural cleanup (fewer files, same three declared
  licenses), not a decision about what to license under. Nothing here should be read as "final."

**Next:** none pending on this specifically — routine remaining OSHWA gaps are unchanged (see
entry 29 and `VERIFICATION.md`'s self-certification checklist).

---

## 2026-07-08 (29)

User asked for a fillable worksheet mirroring the [OSHWA certification form](https://application.oshwa.org/apply)
(all 4 sections), pre-filled from repo context wherever possible.

- Added `BuildYourOwn/OSHWA-APPLICATION-DRAFT.md`: field-by-field draft answers pulled from
  `okh-RheoBoard.yml` (name/affiliation/email/description/keywords/version), `README.md` →
  License, and `VERIFICATION.md`'s existing OSHW Definition walkthrough (backs all the Section 3
  yes/no licensing questions). Uses a ✅/⚠️/⬜ legend to distinguish confident pre-fills from
  judgment-call suggestions (e.g. Individual vs. Company, primary project type) from true unknowns
  that only the user has (address, phone, personal legal attestations/checkboxes).
- Flagged the one substantive open question up top: `VERIFICATION.md`'s own readiness checklist
  shows the physical build isn't done (panel not test-fit/cut, no unit labeled with a revision
  yet) — noted this isn't necessarily blocking for OSHWA (which certifies documentation openness,
  not a working prototype) but is a deliberate call only the user can make, not something to round
  up on.
- Since the filled-in file will contain personal info (address, phone) once the user completes it,
  added `BuildYourOwn/OSHWA-APPLICATION-DRAFT.md` to `.gitignore` so it never gets committed to the
  public repo.
- User then asked for a `.docx` version (to upload to Google Docs). Installed `pandoc` via
  Homebrew (wasn't present) and converted the markdown to
  `BuildYourOwn/OSHWA-APPLICATION-DRAFT.docx` (`pandoc -f gfm --standalone`). Added the `.docx` to
  `.gitignore` alongside the `.md` for the same PII reason.
- User then asked to relocate both files into `BuildYourOwn/hardware/references/` rather than
  leaving them loose at the `BuildYourOwn/` root, to keep the tree organized. Moved both
  (`OSHWA-APPLICATION-DRAFT.md`/`.docx`) there and updated their `.gitignore` paths to match. Note:
  this folder's own `README.md` scopes it to "external datasheets, standards, and third-party
  library notes" — the OSHWA draft doesn't quite fit that description, but it's gitignored (so
  invisible in git regardless) and this was an explicit placement request, so left the README's
  table/scope untouched rather than second-guessing it.
- User then: (1) asked to drop the `.md` and keep only the `.docx` going forward, (2) asked to
  strip the ✅/⚠️/⬜ legend/icons so it reads like an actual filled-out form rather than an
  annotated worksheet, highlighting only the fields still genuinely blank, and (3) provided real
  personal/contact info (legal name **Hoang Vuong**; address at UTA's Department of Computer
  Science and Engineering, 500 UTA Blvd, Arlington, TX 76010; phone). Regenerated the `.docx`
  directly from a temp markdown source via `pandoc -f markdown+mark` (the `mark` extension isn't
  available under `gfm`, hence switching input format) so genuinely-unfilled fields (public contact
  email, the "Creator Contribution requirement" personal read-and-confirm, and the two agreement
  checkboxes) get real yellow Word highlighting instead of an emoji marker. Deleted the `.md`
  source per the user's request to keep only the `.docx`, and updated `.gitignore` accordingly
  (dropped the now-nonexistent `.md` entry, kept the `.docx` entry, added a `~$*` pattern for Word's
  transient lock files after noticing one — `~$HWA-APPLICATION-DRAFT.docx` — appear alongside it,
  meaning the file was open in Word at the time; user should close/reopen Word to see the
  regenerated version rather than risk Word re-saving stale content over it).
- Flagged but did not act on (not asked): the repo's own metadata (`okh-RheoBoard.yml`,
  `README.md`) still says "Charlie Vuong," not "Hoang Vuong" — left a note in the `.docx` itself
  about the mismatch rather than silently changing repo-wide maintainer identity unprompted.
- User supplied the last missing field (public contact email: `cearto@uta.edu` — matches the
  GitHub handle already cited for the `ThingPlusBLEOSC` dependency, so likely the same person).
  Regenerated the `.docx` again and did a full verification pass: parsed the OOXML directly to
  confirm exactly 3 fields remain yellow-highlighted (Creator Contribution requirement read-and-
  confirm, and the two agreement checkboxes — all personal attestations, correctly left blank) and
  that no ✅/⚠️/⬜ characters remain anywhere in the document; also did a `pandoc ... -t plain`
  full-text readback to proofread every field value end-to-end. The Word lock file
  (`~$HWA-APPLICATION-DRAFT.docx`) is gone now, confirming Word was closed and the regenerated file
  is what the user will see on next open — no stale-overwrite risk this time.

**Next:** none outstanding on this thread — the `.docx` is ready for the user to check the 3
highlighted items themselves and submit at https://application.oshwa.org/apply.
- Did **not** submit anything or touch the actual OSHWA form — this is a local worksheet only, per
  "Human-only" in `AGENTS.md` (the online submission + agreement checkboxes are the user's alone).

**Next:** no fixed next step — waiting on the user to fill in personal fields and decide the
before/after-physical-build timing question, then submit the form themselves.

## 2026-07-08 (28)

User pasted the full text of the [OSHWA Open Source Hardware Definition](https://www.oshwa.org/definition/)
(the 12-point criteria a license/project must meet to *be* OSHW — distinct from OSHWA
*certification*, the separate registration process already tracked in `VERIFICATION.md`) and asked
to double-check compliance. Went through each of the 12 points against the actual repo state rather
than assuming: confirmed hardware design files are native/editable everywhere (matplotlib/Python
sources for wiring + laser-cut, Altium native `.SchDoc`/`.PcbDoc`/`.SchLib`/`.PcbLib` for the PCB —
checked the PCB library subfolders too, all native, no Gerber-only substitutes), each `LICENSE-*.txt`
states its scope explicitly and excludes third-party photos/datasheets (cross-checked against
`hardware/images/README.md` / `hardware/references/README.md`), firmware is MIT (OSI-approved,
satisfies the "necessary software" clause directly), and none of the three licenses
(CERN-OHL-W-2.0 / MIT / CC BY-SA 4.0) carry a non-commercial or no-derivatives restriction that
would violate the definition (web-searched OSHWA's own licensing guidance to confirm CERN-OHL-W-2.0
is their explicitly recommended hardware license and to double check the NC/ND incompatibility
rule). Result: **all 12 criteria are met** by the current licensing setup.

- Added a permanent "Open Source Hardware Definition compliance" section to `VERIFICATION.md`,
  point-by-point, so this check doesn't need to be redone from scratch if licenses or design-file
  formats ever change — kept separate from the existing "OSHWA self-certification readiness"
  checklist since the two are genuinely different things (definition compliance vs. registration).
- Added one line to root `README.md` → License, covering an obligation from the Definition's
  introduction (not one of the 12 numbered criteria, but a stated expectation): anyone building/
  selling units based on this design should make clear those units aren't sanctioned by the
  original designer and shouldn't use the project's names to imply endorsement.
- Ran `scripts/check-docs.sh` — passes.

**Next:** unchanged — laser-cut vector file test-fit, building/labeling a first unit, and OSHWA
self-certification submission remain the human-only steps.

## 2026-07-08 (27)

User asked again to align with the same reference repo from (26) (kept anonymous per their
instruction), saying "try again" — read this as wanting closer structural fidelity, not that (26)
was lost (it was still present on disk, uncommitted). Fetched the reference repo's actual raw
`README.md`, `okh-<name>.yml`, `kitspace.yml`, and its nested hardware-folder `LICENSE` file
directly (rather than relying on a scraped page) to find concrete patterns (26) hadn't yet matched:

- **Root `LICENSE` rewritten as a clean, unmodified CERN-OHL-W-2.0 text** (just a copyright line +
  the standard license body, no custom scope paragraph) instead of the old three-way explainer —
  the explainer version wasn't machine-detectable by GitHub's license identifier. The scope
  explanation now lives only in `LICENSE-HARDWARE.txt` (unchanged) and the README's License
  section table, so nothing was lost, just deduplicated to where it's discoverable.
- **Colocated full LICENSE-text copies next to the files they cover** — the reference repo puts a
  complete `PCB/LICENSE` (CERN OHL) right in its hardware folder, not just a root pointer; matched
  that with `BuildYourOwn/hardware/LICENSE`, `BuildYourOwn/laser-cut/LICENSE`,
  `RheoBoard_V8_Final/LICENSE` (all CERN-OHL-W-2.0, identical to root `LICENSE`) and
  `BuildYourOwn/software/rheometer-firmware/LICENSE` (MIT). Documented in `AGENTS.md` that all
  copies must be updated together if a license text ever changes.
- **Renamed `okh.yml` → `okh-RheoBoard.yml`**, matching the reference manifest's
  `okh-<ProjectName>.yml` naming convention (the OKH spec allows either; picked the named form for
  fidelity). Updated every reference in `README.md`, `AGENTS.md`, `VERIFICATION.md` (left
  `PROGRESS.md`'s (26) entry using the old name — historical, accurate at the time). Also added
  `manifest-language`, `documentation-language`, and a `contact` block to the manifest, matching
  fields present in the reference's manifest that (26)'s version had omitted.
- **Dropped the `2P1V-` prefix from the two wiring-diagram image filenames** (`wiring-diagram.png`,
  `tube-connection.png` — previously `2P1V-wiring-diagram.png` / `2P1V-tube-connection.png`),
  closing out a de-branding gap flagged as a "separate, larger decision" back in (18)/(19). Updated
  every live reference (`README.md`, `BuildYourOwn/README.md`, both wiring READMEs, `BOM.md`,
  laser-cut `README.md`, `REVISIONS.md`, `okh-RheoBoard.yml`, and the `OUT =` filename inside
  `generate_wiring_diagram.py`) and regenerated the PNG under its new name to confirm the script
  still runs end-to-end. Left historical `PROGRESS.md` entries referencing the old filenames as-is.
- **README top matter tightened to the reference's style:** the three per-category licenses are
  now named as direct clickable links right under the maintainer line (mirroring "This project is
  licensed under X. The hardware is licensed under Y."), and added a short bullet-list of
  highlights linking straight to the build guide, BOM, verification checklist, and firmware API —
  the reference repo leads with an equivalent linked bullet list before its full write-up.
- **Did not add `kitspace.yml`** — confirmed by inspecting `RheoBoard_V8_Final/`: it's all Altium
  binaries (`.PcbDoc`, `.SchDoc`, `.PcbLib`, etc.), no KiCad project or Gerber exports, and
  Kitspace's `kitspace.yml` schema specifically expects `gerbers:`/`bom:` paths per sub-board. Not
  fabricable through that pipeline as-is; revisit only if the PCB track is ever exported to
  KiCad/Gerbers.
- Ran `scripts/check-docs.sh` (passes) and independently verified every path referenced from
  `okh-RheoBoard.yml` resolves on disk, and the manifest still parses as valid YAML.

**Next:** unchanged from (26) — the laser-cut vector file still needs a physical test-fit pass;
that plus building/labeling a first unit are the remaining human-only steps before OSHWA
self-certification submission. If the maintainer affiliation/email in `okh-RheoBoard.yml` +
`README.md` is wrong, correct it (appears in both).

## 2026-07-08 (26)

User pointed at a well-regarded published open-hardware repo as a structural model (kept
anonymous here per their instruction — it's a reference for us, not something to name in the
project) and asked to bring RheoBoard up to that bar. Studied what that kind of repo does that we
didn't yet: a machine-readable metadata manifest, and a README that leads with open-hardware
front-matter (tagline, maintainer, prominent up-front license statement, repo-structure tree).

- **Added `okh.yml`** (repo root) — an Open Know-How Manifest 1.0 file (the standard
  machine-readable metadata format for open-hardware discoverability/indexing, aligned with the
  OSHWA definition). Researched the OKH spec fresh rather than guessing fields. Populated title,
  description, intended-use, keywords, project/documentation links, teaser image, version
  (`Rev A`), `development-stage: prototype`, `made: false` / `made-independently: false` (honest —
  nothing's been physically built), a health-and-safety notice (mains 12 V, soldering,
  pressurized air), the three SPDX licenses (hardware CERN-OHL-W-2.0 / software MIT / docs
  CC-BY-SA-4.0), licensor, and relative paths to the BOM, design-file sources (both generator
  scripts + `panel.svg`/`.dxf`), schematics, build guide, firmware, and verification checklist.
  Validated it parses as YAML and confirmed all 13 referenced local paths resolve on disk.
- **Root `README.md` restructured to lead like an open-hardware landing page:** added a
  one-line tagline under the title, a maintainer line, and a prominent license statement in the
  first screenful (previously licensing was only at the bottom) with a pointer to `okh.yml`.
  Converted the "Repository layout" bullet list into an ASCII directory tree (clearer at a
  glance, matches the convention of established open-hardware repos), and added `okh.yml` +
  the LICENSE files to it.
- **Cross-references:** noted `okh.yml` in `AGENTS.md`'s Licensing section (with a reminder to
  keep its `date-updated`/`version`/`made`/license/paths in sync with README + REVISIONS), and
  added it as a checked item in `VERIFICATION.md`'s OSHWA-readiness checklist.
- Git remote confirmed as `github.com/The-Hybrid-Atelier/RheoBoard` (used for the absolute URLs
  in the manifest); maintainer identity pulled from `git config` (Charlie Vuong), affiliation
  "The Hybrid Atelier" inferred from the org name — **flag for the user to correct if the
  affiliation/email in `okh.yml` and README should be something else.**
- Ran `scripts/check-docs.sh` — all pass. Did **not** add a `kitspace.yml` (the model repo has
  one): Kitspace indexing targets fabricable PCB projects with KiCad/Gerber outputs, whereas our
  current focus is the breadboard/module DIY track and the PCB track is Altium binaries Kitspace
  can't ingest — so it'd be low-value/likely-invalid right now. Revisit if the PCB track is ever
  exported to KiCad/Gerbers.

**Next:** unchanged from (25) — the laser-cut vector file still needs a physical test-fit pass;
that plus building/labeling a first unit are the remaining human-only steps before OSHWA
self-certification submission. If the maintainer affiliation/email in `okh.yml` + README is
wrong, correct it (one-line fix in both).

---

## 2026-07-08 (25)

Continuation of (24): user clarified "don't say we have the license" — meaning don't imply OSHWA
*certification/approval* has happened (applying our own license to our own copyrighted work
needs no external approval; OSHWA certification is a separate, later, self-submitted step that
genuinely hasn't happened). Re-audited all files touched in (24) for exactly that distinction —
none overclaimed certification, all consistently say "not yet OSHWA-certified." Then closed out
the two biggest actual gaps flagged in (24) as blockers:

**1. Wiring diagram now has real editable source.** The original generator script was lost in a
past `/tmp/` session (see (24)). Rebuilt it from scratch as
`hardware/wiring/generate_wiring_diagram.py` (matplotlib, ~230 lines) — reproduces the same
electrical facts as the existing `2P1V-wiring-diagram.png` (title block, legend, all
component boxes/pins/wires/notes) from the documented wiring (wiring/README.md prose +
`PneumaticSystem.h` GPIO defines), not copied from the lost original. Iterated on layout bugs
(legend/box overlap, a GND-bus wire that accidentally routed through the Notes section, Qwiic
I2C wire drawn through component text) until clean. Regenerated the PNG from this script
(functionally identical diagram, minor cosmetic differences from the old render since the exact
prior script is unrecoverable) and folded PNG palette-quantization into the script itself so
`python3 generate_wiring_diagram.py` is a complete one-command regen (89KB output, same order of
magnitude as the previous file). Documented the regen command in `hardware/wiring/README.md`.

**2. Hardware revision scheme added.** New `hardware/REVISIONS.md` — Rev-letter table mapping a
physical unit to the exact design-file revision it was built from (OSHWA requirement). Current:
**Rev A, not yet physically built**. Cross-linked from `hardware/BOM.md`, `laser-cut/README.md`,
and root `README.md`.

**3. Laser-cut vector file — draft produced, explicitly NOT verified.** This was the harder call.
No authoritative body-dimension data exists for the pump/valve (only port dimensions are
published; datasheets don't give an overall footprint), so a from-scratch CAD layout built on
assumed dimensions would have been a bigger risk than value. Instead: pixel-analyzed the existing
`panel-cut-lines.png` raster reference (confirmed its 1024×706px canvas maps 1:1 onto the panel's
290×200mm bounds — aspect ratio matches to within 0.1%) using `scipy.ndimage` connected-component
labeling, extracted real mm coordinates for all 4 corner holes, the chamber bulkhead hole, and
~50 zip-tie slots directly from the reference geometry (not invented), then cross-checked the
big circle's position against `panel-placement-map.png`'s independently-extracted CHAMBER marker
position — they agree, which validated the scale/registration assumption. Wrote
`laser-cut/generate_panel_vector.py`, which outputs `panel.svg` + `panel.dxf` (hand-written
minimal ASCII DXF, no `ezdxf` dependency) from this traced table, plus a `--retrace` mode that
re-runs the pixel extraction fresh for future review. **What's honestly uncertain:** which exact
part each zip-tie slot belongs to — grouped into 5 rough columns matching
`panel-placement-map.png`'s left-to-right layout, but not independently re-verified slot-by-slot,
so the script/README are explicit that this is a draft needing a real test-fit pass, not a
finished CAD file. Updated `laser-cut/README.md`'s status callout, file table, parts table, and
"once files exist" checklist accordingly; updated root `README.md`, `BuildYourOwn/README.md`
(Step 02), `hardware/BOM.md`, and `AGENTS.md` to say "draft vector file, not yet test-fit"
instead of "vector file TBD" everywhere that phrase appeared.

**4. OSHWA submission-readiness checklist added** to `VERIFICATION.md` (new section) — the
concrete, current-state checklist for what's done vs. still needed before submitting the actual
OSHWA self-certification form (a human-only step: agreeing to their Certification Mark License
Agreement isn't something an agent should do on the user's behalf).

Ran `scripts/check-docs.sh` — all checks pass (no dangling links after the ~8 files' worth of
cross-reference updates).

**Next:** the laser-cut vector file's slot-to-part assignment and overall accuracy needs a real
test-fit pass once physical parts are in hand — that's the last concrete blocker before this
project could reasonably go up for OSHWA self-certification. Everything else on the checklist in
`VERIFICATION.md` → "OSHWA self-certification readiness" is either done or explicitly human-only
(submitting the form itself, physically cutting/labeling the first unit).

---

## 2026-07-08 (24)

User asked whether the project meets OSHWA (Open Source Hardware Association) certification
standards, and which license to use. Researched OSHWA's actual certification requirements and the
hardware-license landscape (CERN OHL variants, TAPR, Solderpad) live, then applied licensing.

**Assessment (before this session): not OSHWA-certifiable.** Root `README.md`'s License section
said "TBD", no `LICENSE` file existed anywhere, firmware had no license header, and docs had no
open-license notice. Design-file-format gap also confirmed: the wiring diagram is a rendered PNG
only — the matplotlib script that generated it lived in `/tmp/` in a past session and was never
committed, so there's no editable source in the repo; the laser-cut panel has no vector source
either (already flagged). Both are OSHWA blockers ("design files in the preferred format for
making changes") independent of licensing.

**What changed this session** — clarified with the user that hardware/software/docs need
*separate* licenses (standard OSHWA guidance, since GPL/CC aren't hardware licenses and OSHWA
itself isn't a license). User chose, via explicit options:

- **Hardware** (wiring diagrams, laser-cut design, BOM, PCB design under `RheoBoard_V8_Final/`) —
  **CERN-OHL-W-2.0** (weakly reciprocal: modifications to the design must stay open, but a larger
  project merely incorporating it doesn't have to). Full official text vendored into
  `LICENSE-HARDWARE.txt` (fetched from `ohwr.org/cern_ohl_w_v2.txt`, not reproduced from memory).
- **Firmware** — **MIT** (matches the `ThingPlusBLEOSC` dependency's own license). Text in
  `LICENSE-SOFTWARE.txt`.
- **Documentation** — **CC BY-SA 4.0**. Notice + link in `LICENSE-DOCUMENTATION.txt` (per CC's own
  guidance, linked rather than the full legal code reproduced).
- Added a root `LICENSE` file explaining the three-way split, and rewrote root `README.md`'s
  License section into a table with the same breakdown plus an explicit "not yet OSHWA-certified"
  caveat pointing back here.
- Added `SPDX-License-Identifier: MIT` headers to all 5 firmware files
  (`2P1VX.ino`, `PneumaticSystem.{h,cpp}`, `RheoSystem.{h,cpp}`).
- Added one-line "License: CERN-OHL-W-2.0 — see ..." pointers to the three hardware-design index
  files: `hardware/wiring/README.md`, `laser-cut/README.md`, `hardware/BOM.md`.
- Added a license line to `BuildYourOwn/README.md`'s intro, and a new "## Licensing" section in
  `AGENTS.md` so future agents apply the right header/pointer convention to new files.
- Copyright holder on all three: "Charlie Vuong" (pulled from `git config user.name`, confirmed
  live rather than guessed) — copyright year 2026.
- Did **not** add a `RheoBoard_V8_Final/` README notice (none exists yet) — it's already in scope
  per `LICENSE-HARDWARE.txt`'s stated coverage, but a per-folder pointer can be added once that
  track has an active README.

**Remaining OSHWA gaps (not addressed this session — user explicitly scoped this pass to
"just add the LICENSE files," not a full remediation plan):**

- Laser-cut vector source file (`.svg`/`.dxf`) still doesn't exist — raster-only design files
  don't satisfy "preferred format for making changes."
- Wiring diagram's generator script was never committed (lost from a past `/tmp/` session) — the
  PNG alone is a rendered artifact, not editable source. Should regenerate the script and commit
  it under `hardware/wiring/` alongside the PNG next time the diagram is touched.
- No hardware version/revision-numbering scheme documented (OSHWA wants a physical unit
  traceable to a design-file revision).
- Self-certification with OSHWA (free, online form + Certification Mark Agreement, annual
  renewal) hasn't been submitted — only makes sense once the above are resolved.
- Ran `scripts/check-docs.sh` — passes (it doesn't check license files, this was a manual review).

**Next:** if/when the laser-cut vector file gets produced and the wiring-diagram generator script
is recreated, commit both as real design-file sources — that's the biggest remaining blocker to
being certifiable, bigger than anything licensing-related.

---

## 2026-07-08 (23)

Removed the "none human-verified yet, no photos/video exist yet" style status notes from
`BuildYourOwn/README.md` (the builder-facing guide), per explicit user feedback: these were
agent-facing progress-tracking notes that had leaked into the builder-facing doc, not something a
person following the guide needs to read. User's framing: *"This is a note for you only... a note
for you to keep track and help me."*

- Removed from the top-of-file intro (`_All 8 steps written; none human-verified yet, no
  photos/video exist yet._`) and from the "## Steps" section intro sentence ("None of these are
  human-verified yet, and no photos/video exist for any step.").
- **Left the per-step blockers in place** (Step 02 "— blocked on laser-cut vector file", Step 05
  "— blocked on RheoMap's fixture spec") — those are actual content facts a builder needs (the
  step genuinely can't be completed yet), not process/QA metadata about doc completeness, so they
  stayed. Same reasoning for the inline TBD notes inside Steps 02 and 05's instructions.
  Did **not** touch the root `README.md`'s "## Status" checklist (`- [ ] Step-by-step guide
  human-verified against a real build`, etc.) — that's a distinct, explicitly-labeled
  project-status section (conventional in a README), not narration mixed into the guide itself;
  wasn't what was quoted, so left alone pending explicit ask.
- **This tracking status isn't lost** — it's exactly what this file (`PROGRESS.md`) and
  `VERIFICATION.md` are for. Current state, for continuity: all 8 steps in
  `BuildYourOwn/README.md` are written but **not yet human-verified against a real build**; no
  step has photos or video. Step 02 is additionally blocked on the laser-cut vector file not
  existing yet; Step 05 is additionally blocked on RheoMap's sample/fixture geometry spec not
  existing yet. Update this note (or clear it) once a real build verification pass happens.
- Ran `scripts/check-docs.sh` — all checks pass.

**Next:** none from this pass. If the root `README.md` Status checklist should get the same
treatment, that's a separate explicit ask.

---

## 2026-07-08 (22)

Trimmed `BuildYourOwn/README.md` (the step-by-step guide) per explicit user feedback that it read
as "too much yapping" — pointed back at the OpenTheremin V4 PDF example shared earlier in the
session (entry from 2026-07-08 07:33 session log: "notably brief at just four pages with numbered
steps," minimal prose, mostly short imperative instructions) as the target style.

- **Removed `Time`/`Difficulty` lines from every step** — cut, not relocated; there's no
  replacement metadata since it wasn't asked for.
- **Rewrote all 8 steps as short numbered instructions**, cutting most explanatory prose,
  multi-sentence justifications, and repeated context (e.g. "What you'll need for this step" prose
  blocks trimmed to a single inline sentence or dropped where it just repeated "Before you start").
  Kept every load-bearing technical fact (GPIO pins, I2C addresses, part numbers, OSC parameter
  names/defaults, table content) — nothing technical was cut, only the surrounding narration.
  Condensed multi-line "Tips / common mistakes" and "Check before moving on" blocks per step down
  to a single **Tip:** line each (or folded into the instructions directly) rather than separate
  sections.
- Compacted the top-level "Steps" status list from one verbose line per step
  ("— written, not verified; no media") repeated 8 times to one disclaimer sentence above the list
  plus only the two step-specific blockers (Step 02: laser-cut vector file; Step 05: RheoMap
  fixture spec) called out inline.
- Trimmed "Overview" from two long bullet points to two short paragraphs; trimmed the bottom
  "Tips (all steps, at a glance)" list to one line per tip instead of two—three; trimmed "Media
  conventions" to two sentences.
- **Did not touch step heading text** (`## Step 01: Kit contents and tools`, etc.) — anchors
  linked from `laser-cut/README.md` and within the file itself depend on the exact heading slug;
  verified after the edit that `grep -n "^## Step"` still matches every anchor used elsewhere.
- Net effect: `BuildYourOwn/README.md` went from ~540 lines to ~266 (git diff: +133/-407).
- Ran `scripts/check-docs.sh` — all checks pass.

**Next:** none from this pass. If the root `README.md` (project overview) also reads as too
verbose, the same treatment could apply there, but that wasn't requested this round.

---

## 2026-07-08 (21)

Two more structural moves per explicit user request: (1) consolidate the 9-file `tutorial/`
folder (overview + 8 per-step READMEs + template + steps index) into **one** `README.md`, located
directly in `BuildYourOwn/`; (2) move the old `BuildYourOwn/README.md` (project overview/reference
doc) up to the **repo root**, merged with the existing root `README.md`.

- **New `BuildYourOwn/README.md`** — the step-by-step build guide itself, single file. All 8 steps
  from `tutorial/steps/NN-*/README.md` are now `## Step NN: ...` sections in one document (in
  build order), preceded by the old `tutorial/README.md`'s Overview/Before-you-start content and
  followed by its Tips/Media-conventions content. Internal links between steps changed from folder
  links (`../03-wire-electronics/`) to same-file anchors (`#step-03-wire-the-electronics`) —
  verified every anchor matches GitHub's actual heading-slug algorithm (lowercase, strip
  punctuation, spaces→hyphens) by cross-checking against the real `## Step NN: ...` headings.
  Cross-folder paths got *shallower* since this file now sits at `BuildYourOwn/` root instead of
  3 levels deep in `tutorial/steps/NN/`: `../../../hardware/BOM.md` → `hardware/BOM.md`, etc.
- **Deleted `tutorial/` entirely** (`rm -rf`) — `README.md`, `_step-template/` (incl. its
  `media/.gitkeep`), and all 8 `steps/NN-*/README.md` — once satisfied every line of content had
  a home in the new consolidated file. Per-step `media/` folders go away too: future build photos
  now land directly in `images/`, named by step (e.g. `step02-panel-placement.jpg`), noted in the
  new file's "Media conventions" section.
- **Root `README.md` rewrite** — merged the old root README (project intro, two-track summary,
  minimal repo layout, status, license) with the old `BuildYourOwn/README.md`'s reference content
  (Features, Hardware incl. wiring diagrams + component gallery, Software configuration, Connect
  and use, Tips, Repo layout, Status checklist). All paths that used to be relative to
  `BuildYourOwn/` got a `BuildYourOwn/` prefix added since this content now lives at repo root.
  Added a "Start here" callout pointing at `BuildYourOwn/README.md` as the actual build guide,
  mirroring the callout the old `BuildYourOwn/README.md` had (that callout's job moved with the
  content).
- **Updated every remaining cross-reference to the removed `tutorial/`**: `AGENTS.md` (repository
  layout + `BuildYourOwn/` map sections rewritten), `BuildYourOwn/VERIFICATION.md`,
  `BuildYourOwn/product-specs.md`, `BuildYourOwn/images/README.md`, `BuildYourOwn/laser-cut/README.md`
  (two step-02 references + the "once files exist" checklist), `BuildYourOwn/software/README.md`
  (builder walkthrough link). Also fixed two unrelated pre-existing stale references caught along
  the way: `VERIFICATION.md`'s sign-off note pointed at a `tech-debt-tracker.md` that was deleted
  back in entry 15/16's `exec-plans/` cleanup (now points at `PROGRESS.md` instead).
- Ran `scripts/check-docs.sh` (all three checks pass) plus a manual `rg -n "tutorial/"` sweep
  (only the two intentional "there is no separate tutorial/ folder anymore" explanatory mentions
  in `AGENTS.md` remain) — the script's own dangling-link checker doesn't validate `#anchor`
  fragments, so those were checked by hand as noted above.
- **Left every earlier `PROGRESS.md` entry untouched** — they correctly describe the repo as it
  was at the time (including the now-removed `tutorial/` folder); only this entry reflects the
  current single-file-guide structure.

**Next:** none from this pass — verified clean. Same substantive open items as before: real
laser-cut vector file, `images/ide-settings.png` + `images/platform.png` + `images/wiring.png`
captures, human verification of the build guide, RheoMap fixture spec for Step 05.

---

## 2026-07-08 (20)

Restructured `BuildYourOwn/`'s top level again, superseding entry 19's `assets/` folder — per
explicit user request to make the tutorial the obvious starting point and group all electrical
content (wiring, BOM, component photos, datasheets) under one `hardware/` folder, as a sibling to
`laser-cut/` and `software/`. Asked 3 clarifying multiple-choice questions first (what happens to
nested tutorial steps, what exactly "hardware" should contain, where it sits) since guessing wrong
would mean a third reshuffle — user chose: keep `tutorial/steps/` nested as-is (just make
`README.md` launch it more clearly), `hardware/` = wiring + BOM + component photos + datasheets
(the full electronics bundle), and `hardware/` as a top-level sibling folder.

- **New layout:**
  - `hardware/BOM.md` — was `BOM.md` at root.
  - `hardware/wiring/` — was `assets/wiring/` (unchanged contents: diagrams + `pneumatic-plumbing.md`).
  - `hardware/images/` — was `assets/images/components/` (the 6 component photos + their README),
    flattened one level since `hardware/` already narrows the scope to electrical parts.
  - `hardware/references/` — was `assets/references/` (unchanged contents: `README.md` +
    `datasheets/`).
  - `images/` (top-level) — was `assets/images/` minus the `components/` subfolder (just
    `teaser.jpg` + `README.md`, the project-wide/non-electrical photos). Dropped the now-pointless
    `assets/` wrapper since nothing else remained under it — a single-child wrapper folder wasn't
    adding value once wiring/references/component-photos moved out.
  - `tutorial/` — untouched structurally (steps stay nested under `tutorial/steps/NN-*/`).
- Added a "Start here" callout at the top of `README.md` (right after the hero image, before the
  table of contents) explicitly stating the README is a reference doc and pointing to
  `tutorial/README.md` as the actual build entry point — addresses the "should include all steps"
  half of the request without physically flattening the tutorial folder.
- Updated every cross-reference across the repo (`README.md`, `VERIFICATION.md`, `core-beliefs.md`,
  `AGENTS.md`, `laser-cut/README.md`, `software/README.md`, `tutorial/README.md`,
  `tutorial/_step-template/README.md`, all 8 tutorial step READMEs, and the moved files'
  own internal links) — recomputed relative-path depth in both directions: some links got
  shallower (e.g. `hardware/wiring/README.md`'s `../../BOM.md` → `../BOM.md` since BOM.md is now a
  direct sibling in `hardware/`), others needed a new `hardware/` segment inserted
  (e.g. tutorial steps' `../../../assets/wiring/...` → `../../../hardware/wiring/...`).
- Fixed `scripts/check-docs.sh`'s hardcoded `bom_file="BuildYourOwn/BOM.md"` to
  `BuildYourOwn/hardware/BOM.md` — the datasheet-coverage check was silently passing vacuously
  (file not found → zero rows checked) until this was caught and fixed; re-ran after the fix and
  confirmed it's now actually checking the real file.
- Rewrote `AGENTS.md`'s "Repository layout" and "`BuildYourOwn/` map" sections to describe the new
  `hardware/`/`images/`/`tutorial/`-starts-here structure instead of the one-session-old `assets/`
  description from entry 19.
- **Left entry 19 (and all earlier entries) untouched** — they correctly describe the repo as it
  was at the time; only this entry and going forward reflect `hardware/`.
- Verified with a fresh `rg -l "assets/"` sweep (not just the IDE's search tool, which had a stale
  cache from the pre-move git state) that no live doc still points at the old `assets/` paths —
  remaining hits are entry 19's historical prose and one unrelated `sparkfun.com/assets/...` URL.
  Also spot-checked every `.md` file's relative links resolve on disk (`dirname(file) + link`
  exists) across the whole `BuildYourOwn/` tree; the only "broken" hit is the step template's
  placeholder `media/photo-01.jpg`, which is intentionally aspirational.
- Ran `scripts/check-docs.sh` after all fixes — all three checks pass.

**Next:** none from this reshuffle — verified clean. Same open items as before (real laser-cut
vector file, `images/ide-settings.png` + `images/platform.png` + `images/wiring.png` captures,
human verification of tutorial steps, RheoMap fixture spec for step 05).

---

## 2026-07-08 (19)

Consolidated `images/`, `wiring/`, and `references.md`/`references/` into a single
`assets/` folder, per explicit user request ("These are should be combine to same folder").
All three were top-level, media/reference-heavy folders (photos, diagrams, vendored PDFs) —
grouping them makes `BuildYourOwn/`'s top level read as prose docs + one media folder instead
of three separate media-ish folders scattered alongside the markdown.

- New layout: `assets/images/` (was `images/`, includes `components/` subfolder unchanged),
  `assets/wiring/` (was `wiring/`, includes `pneumatic-plumbing.md` unchanged), and
  `assets/references/` (was the `references.md` file *and* the separate `references/datasheets/`
  folder — merged so the file becomes `assets/references/README.md` sitting next to its own
  `datasheets/` subfolder, matching the `images/README.md` / `wiring/README.md` pattern of
  "folder + its own README index").
- **Named it `assets/`, not `media/`** — `media/` was already taken: every tutorial step has its
  own per-step `tutorial/steps/NN-*/media/` folder for step-specific photos/video (see
  `tutorial/_step-template/README.md`). Reusing "media" for the new top-level folder would have
  created a confusing naming collision between two different-scoped things.
- Used plain `mv`/`mkdir` (not `git mv`) — consistent with how the `software/2P1VX/` →
  `software/rheometer-firmware/` rename was done earlier (entry 14): the agent doesn't stage
  files for the user.
- Updated every real cross-reference across the repo: `README.md`, `BOM.md`,
  `VERIFICATION.md`, `AGENTS.md`, `core-beliefs.md`, `laser-cut/README.md`, `software/README.md`,
  `tutorial/README.md`, `tutorial/_step-template/README.md`, and all 8 tutorial step READMEs —
  recomputing relative-path depth where the move added a folder level (e.g. tutorial steps'
  `../../../wiring/...` → `../../../assets/wiring/...`). Also fixed the *internal* links inside
  the moved files themselves (`assets/wiring/README.md`'s `../software/` → `../../software/` now
  that it's one level deeper; `assets/references/README.md`'s self-references to its own
  `datasheets/` subfolder simplified from `references/datasheets/` to just `datasheets/`).
- **Did not touch `PROGRESS.md`'s historical entries** — old entries describing past work
  correctly reference paths as they were *at the time* (e.g. "`wiring/2P1V-wiring-diagram.png`
  regenerated..."); rewriting them to the new path would misrepresent history. Only this entry
  and going forward use `assets/`.
- Left the per-step `media/` convention (`tutorial/steps/README.md`, `_step-template/README.md`)
  completely alone — unrelated concept, different scope.
- Ran `scripts/check-docs.sh` — all checks pass. Also did a manual repo-wide grep sweep for any
  remaining bare (non-`assets/`-prefixed) mentions of `images/`, `wiring/`, or `references/` —
  the only hits left are the unrelated per-step `media/` lines and two intentional parenthetical
  breakdowns in `AGENTS.md` that spell out `assets/`'s contents.

**Next:** none — this was a complete, verified sweep. If new project-wide media gets added later
(e.g. `platform.png`, `wiring.png`, `ide-settings.png` from the still-open capture tasks), it goes
in `assets/images/` going forward, not a new top-level folder.

---

## 2026-07-08 (18)

Replaced every "BYO" with "DIY" across the project, per explicit user request ("Do not use
'BYO'"). Purely a terminology swap — no structural changes.

- Updated prose in `AGENTS.md` (3 spots), `BuildYourOwn/README.md`, `tutorial/README.md`,
  `BOM.md`, `images/README.md`, `tutorial/steps/07-calibrate/README.md`, and every historical
  mention across `PROGRESS.md` itself (whole-word replace, so it didn't touch `BuildYourOwn` —
  that folder name doesn't contain "BYO" as a substring).
- The regenerated wiring schematic from entry 17 had "BYO" baked into its rendered title text
  (both the page title and the title block said "RheoBoard BYO") — re-ran the generator script
  with "DIY" substituted and re-exported `wiring/2P1V-wiring-diagram.png` (same size, 49 KB).
  Checked the other three generated/vendored diagrams (`2P1V-tube-connection.png`,
  `panel-system-diagram.png`, `panel-placement-map.png`) for baked-in "BYO" text — none found, no
  other images needed touching.
- Did **not** rename the `BuildYourOwn/` folder or any filenames — the user's instruction was
  about the "BYO" abbreviation in prose/labels, not the spelled-out folder name.
- Ran `scripts/check-docs.sh` — all checks pass. Confirmed with a case-insensitive repo-wide
  grep that no "byo" string remains anywhere.

**Next:** none — this was a complete, verified sweep.

---

## 2026-07-08 (17)

Regenerated `wiring/2P1V-wiring-diagram.png` in a real schematic-capture style, per user
request ("make it more organiz[ed], look like real schematic") — previous version was a
labeled block diagram (colored boxes with prose-y text inside); this one now reads like
something out of KiCad/Eagle.

- Rebuilt the generator script (Matplotlib) from scratch with: a bordered sheet frame + column/
  row reference grid (numbers/letters along the edges, like a fab drawing), components drawn as
  IC-style rectangles with individually labeled pin stubs (`ENA`, `GPIO32`, `OUT1/2`, etc.)
  instead of paragraph text, strictly orthogonal (Manhattan) wire routing, filled junction dots
  at every real electrical tee (e.g. the +12V and GND buses splitting to both L298N boards), and
  a KiCad-style title block (title / scale / rev / sheet) in the bottom-right corner.
- Net-color legend kept from the old version (+12V orange, GND black, GPIO→EN blue, motor/valve
  load dark red, Qwiic/I2C purple, unused/reserved dashed gray) since it reads well and matches
  the pneumatic diagram's palette.
- The two internal L298N jumpers (IN1/IN3→+5V, IN2/IN4→GND — hardwired direction-setting wires
  the builder makes on the module's own terminal block) are drawn as short local pin stubs at the
  bottom of each driver box rather than routed anywhere, since they're not really "cross-board"
  wires. Positioned at pin fractions >0.5 specifically so they fall outside the GND bus's
  horizontal span and don't visually cross it.
- Content is unchanged from the previous version (nets, GPIO map, unused/reserved items) — this
  was a pure re-draw for legibility/style, cross-checked against
  `software/rheometer-firmware/PneumaticSystem.h` again while at it.
- Updated `wiring/README.md`'s "Conventions" section: it previously said to prefer pictographic
  diagrams over abstract schematics everywhere, which the new electrical diagram deliberately
  contradicts. Now states electrical wiring is schematic-style (precision) while pneumatic
  plumbing stays pictographic (physical tube routing, not electrical nets).
- Also dropped "2P1V" from the on-image title text (now "RheoBoard DIY — Electrical Schematic")
  to match the broader de-branding done this session — left the **filename**
  (`2P1V-wiring-diagram.png`) alone since renaming files is a separate, larger decision (see
  entry 14 and the tech-debt note it left).
- Rendered at 1900 px wide, quantized PNG (`FASTOCTREE`, 256 colors) → **49 KB** (previous
  version was 92 KB, so this is smaller *and* denser with information).
- Ran `scripts/check-docs.sh` — all checks pass.

**Next:** no further action needed unless the user wants the pneumatic tube diagram
(`2P1V-tube-connection.png`) restyled to match, or wants the filename itself de-branded (would
need to update every reference across `README.md`, `BOM.md`, `wiring/README.md`,
`tutorial/steps/03-wire-electronics/`).

---

## 2026-07-08 (16)

Resized/compressed every other image in the repo (user asked for "all other pictures as well"
after the `teaser.jpg` pass) — total image payload dropped from ~2.1 MB to ~460 KB (excluding
`teaser.jpg`, already handled previously).

- **`wiring/2P1V-wiring-diagram.png`:** 3322×2618 / 644 KB → 1800×1418 / **92 KB**. Resized with
  `sips`, then re-encoded as an indexed-palette PNG (`PIL.Image.quantize`, FASTOCTREE, 256 colors)
  instead of plain resize alone — this is what a diagram made mostly of flat colors/text
  compresses down so dramatically with zero visible quality loss (spot-checked at full size).
- **`wiring/2P1V-tube-connection.png`:** was actually JPEG-encoded (mismatched extension, same
  issue as the old `teaser.png`) at 1024×683 / 88 KB. Re-encoded as a true quantized PNG →
  **42 KB**, removing the JPEG artifacts a line-art diagram shouldn't have. Extension left as
  `.png` since this is genuinely PNG now (only `teaser` needed a rename, since that one's a
  photo).
- **`laser-cut/*.png`** (`panel-cut-lines`, `panel-system-diagram`, `panel-placement-map`): same
  quantization treatment → 24 KB→4 KB, 98 KB→22 KB, 65 KB→36 KB respectively. These are only
  linked (not embedded inline) in `laser-cut/README.md`, so this only helps click-through load
  time, not page rendering.
- **`images/components/*.jpg`** (6 product photos, real photography — quantization doesn't apply
  to photos): resized to a 500 px long edge + re-compressed (JPEG quality 78) →
  ~150–300 KB each down to **10–36 KB each**.
- **Display-width fixes** (the underlying reason this mattered beyond just download size): the
  component gallery in `README.md` and the photo column in `BOM.md` were plain Markdown `![]()`
  images inside table cells — at native resolution (600–970 px per photo, wildly inconsistent
  with the 248 px L298N photo) that would have rendered as an oversized, uneven-looking table.
  Switched every one to an HTML `<img width="…">` tag (180 px in the `README.md` gallery, 100 px
  in `BOM.md`'s narrower table) for small, uniform thumbnails. Did the same for the two wiring
  diagrams in `README.md`/`wiring/README.md` (`width="800"`/`"600"`, wrapped in `<a href>` so the
  full-resolution image is still one click away for anyone who needs to actually read the diagram
  closely).
- Ran `scripts/check-docs.sh` — all checks pass.

**Next:** no other images remain unoptimized. If any new photos/diagrams get added later (the
still-outstanding `platform.png`, `wiring.png`, `ide-settings.png`, or step media), apply the same
treatment: quantized PNG for line-art/diagrams, resized+compressed JPEG for photos, explicit
`<img width>` for anything embedded inside a table or used as a page hero.

---

## 2026-07-08 (15)

Resized/compressed `images/teaser.jpg` (formerly `teaser.png`) — user said the picture was
rendering too big.

- The file was 1024×768, 244 KB, and — despite the `.png` extension — actually JPEG-encoded
  content (it just worked because browsers/GitHub sniff real content, not the extension).
  Resized to 640×480 and re-compressed (quality ~70) → ~100 KB, and renamed to `teaser.jpg` to
  match its real encoding.
- Beyond shrinking the file, also switched both embeds (`README.md`, `tutorial/README.md`) from
  plain Markdown `![]()` to an HTML `<img width="480">` tag — Markdown image syntax has no size
  control, so this was needed to actually constrain the *rendered* width on the page, not just
  the underlying file's pixel dimensions.
- Updated every reference (`images/README.md`'s file table, step 08's Media section) from
  `teaser.png` → `teaser.jpg`. Left the four older, pre-existing `PROGRESS.md` entries that
  mention a still-pending `images/teaser.png` capture request untouched (historical record from
  before this file existed) — only corrected the one same-day entry (13) that documented adding
  the original file.
- Ran `scripts/check-docs.sh` — link/BOM checks pass; the "PROGRESS.md freshness" WARN it printed
  is about the user's own most recent git commit not touching `PROGRESS.md` in the same commit —
  unrelated to this session (the agent never commits), noted here only so it's not mistaken for
  something this change broke.

**Next:** if 480 px still reads too large/small once viewed on GitHub, it's a one-line `width=`
tweak in both READMEs.

---

## 2026-07-08 (14)

Stopped branding the build as "2P1V"/"2P1VX" in user-facing docs — user wants it referred to as
"this design" or "this simple rheometer" instead, without touching the actual firmware/code.

- **Renamed `software/2P1VX/` → `software/rheometer-firmware/`** (plain `mv`, not `git mv` — no
  `git add`/staging performed). **Left every file inside untouched**, including the sketch's own
  filename (`2P1VX.ino`) and its content (`DEVICE_NAME "2P1VX"` and all code/comments) — user was
  explicit: don't change firmware or code, only the folder and its `README.md`.
- Rewrote `software/rheometer-firmware/README.md`'s title/intro to drop the "2P1VX Firmware API"
  branding, while explicitly noting the sketch file and BLE device name are still `2P1VX` in the
  unchanged code — a builder scanning for the device in RheoData still needs that exact string.
- Swept every other doc (`README.md`, `BOM.md`, `references.md`, `wiring/README.md`,
  `wiring/pneumatic-plumbing.md`, `laser-cut/README.md`, `images/README.md`,
  `images/components/README.md`, `software/README.md`, `tutorial/README.md`, all 8 tutorial step
  READMEs) replacing descriptive/label uses of "2P1V rig" / "2P1VX firmware" with generic phrasing
  ("this design," "this simple rheometer," "the firmware"), and updated every path reference from
  `software/2P1VX/` to `software/rheometer-firmware/`.
- **Deliberately kept** the literal string `2P1VX` in the small number of places where it's a
  required technical fact, not decoration — the BLE device name a builder must look for in
  RheoData, and literal quoted serial output (`2P1VX initialized`) — since the firmware itself is
  unchanged and those strings are real. **Deliberately left alone:** the wiring diagram filenames
  themselves (`2P1V-wiring-diagram.png`, `2P1V-tube-connection.png` — renaming image files wasn't
  asked for, only the firmware folder), and all older `PROGRESS.md` entries below this one (not
  rewriting history).
- Ran `scripts/check-docs.sh` after the rename — no dangling links, confirming every reference to
  the old `software/2P1VX/` path got updated.

**Next:** if the wiring diagram filenames (`2P1V-*.png`) or the firmware's actual BLE device name
should also drop "2P1V" at some point, that's a separate, larger decision (image regeneration /
alt-text, or an actual firmware change affecting real device pairing) — flagged, not done here.

---

## 2026-07-08 (13)

Added the first real build photo: `images/teaser.jpg`, a user-provided photo of the assembled
bench build (user described it as representative of the final product's look).

- Copied the photo to `images/teaser.jpg` and embedded it in `README.md` (top hero spot,
  replacing the "add when it exists" placeholder) and `tutorial/README.md` (Overview section,
  replacing the "add once a build exists" placeholder). (Originally saved as `teaser.png`; see the
  later same-day entry that resized it and corrected the extension to `.jpg`, its actual encoding.)
- Updated `images/README.md`'s file table to mark `teaser.jpg` as added (still `TBD`:
  `platform.png`, `wiring.png`, `ide-settings.png`).
- Updated step 08's Media section to point at `images/teaser.jpg` for the assembled shot, and
  narrowed the remaining ask to a photo/video of the rig actually mid-REP (LED lit), not just
  assembled.
- **Did not** cross-check the photo against the wiring diagram or laser-cut mounting scheme in
  detail (e.g. exact zip-tie vs. tape mounting) — this was purely an image-placement task, not a
  build-verification pass. Flagged in chat for the user to confirm separately if wanted.

**Next:** `platform.png`, `wiring.png`, and `ide-settings.png` are still unphotographed; a
mid-REP action shot/video is still wanted per step 08. Tutorial steps remain human-unverified.

---

## 2026-07-08 (12)

Wrote the tutorial in full detail — all 8 steps now have real instructions, and the last
`TBD`/scaffold sections in `tutorial/README.md` are filled in.

- **`tutorial/README.md`:** wrote "Overview" (what you build, difficulty, an estimated **~4–7 h
  hands-on** build time derived from summing per-step estimates — flagged as unmeasured/placeholder
  until a real build times it), a concrete "Tools" list under "Before you start," and a "Tips"
  section consolidating the most important tip from each step (tagged by step number). Updated the
  per-step status table: all 8 steps are now "written, not verified" (step 05 additionally flagged
  as blocked on a real spec).
- **Step 05 (mount and setup) — written from scratch:** panel placement/stability, tracing every
  pneumatic line for kinks/pinches after mounting, cable slack, and environmental notes (drafts/heat
  near the chamber can bias the REP's baseline sampling window). Explicitly flagged that exact
  sample/fixture geometry (standoff, alignment) depends on RheoMap's product spec, which is still
  `TBD` in `product-specs.md` — did not fabricate fixture specifics that don't exist yet.
- **Step 08 (ready to use) — written from scratch:** using the full `2P1VX` firmware API (read from
  `2P1VX.ino` / `PneumaticSystem.h` / `software/rheometer-firmware/README.md`), documented all four REP trigger
  paths (BLE `rheo/rep`, onboard boot-button GPIO 0, Qwiic single-click, USB serial `REP`), the
  Qwiic Button's other two gestures (double-click = latched suck, hold = momentary blow), the bench
  serial command set, and how to read a trace via RheoData or raw serial `#S`/`#PH` lines. Points at
  `VERIFICATION.md` sign-off as the final gate.
- **Reviewed steps 01–04, 06–07** against everything that changed in recent sessions (ESP32
  micro-USB variant, Qwiic Button now required, laser-cut panel, diagram regeneration): filled in
  three remaining `TBD` time estimates (step 01 ~15–20 min, step 02 ~30–45 min hands-on excluding
  laser turnaround, step 07 ~20–30 min initial pass) and fixed a stale "follow section A —
  Electrical Wiring" reference in step 03 — the current diagram/`wiring/README.md` use "Electrical
  wiring" / "Pneumatic plumbing" headings, not lettered sections. Everything else checked out
  consistent (no leftover "Qwiic Button optional" language, ESP32 Boards Manager URL matches
  `references.md`).
- Top-level `README.md` Status checklist split the old combined "written **and** human-verified"
  tutorial line into two — "written" is now checked (all 8 steps), "human-verified" stays open.
- Ran `scripts/check-docs.sh` — all checks pass (no dangling links, BOM datasheet coverage OK,
  `PROGRESS.md` freshness OK).

**Next:** tutorial content is now complete on paper; the remaining gap across the whole tutorial is
media (zero photos/videos exist for any step) and human verification against a real build
(`VERIFICATION.md`). Step 05 will need a follow-up pass once RheoMap's sample/fixture geometry spec
is written. No further tutorial *writing* is blocking — next session's focus should shift to either
producing the laser-cut vector file (`.svg`/`.dxf`, still TBD in `laser-cut/README.md`) or an actual
build to generate media and verification data.

---

## 2026-07-08 (11)

Removed all mentions of **Calico** and **OpenTheremin V4** from project-facing docs. Both were
used across earlier sessions purely as internal documentation-model references (style/structure
inspiration for the agent) — user clarified they were never meant to be cited in the actual
project docs, and asked for them out.

- Stripped references from 13 files: `README.md`'s "Documentation model" blockquote and three
  "Calico equivalent"/Instructables-layer asides; `tutorial/README.md`'s entire "Documentation
  model" section (the Calico/OpenTheremin table + PDF note) and a "Calico keeps tips..." aside;
  `product-specs.md`'s DIY bullet; `laser-cut/README.md`'s "Modeled on Calico" intro;
  `images/README.md`'s "Modeled on Calico" intro; `software/README.md`'s "(Calico pattern)"
  aside; `AGENTS.md`'s "(Calico-style master doc)" aside; and OpenTheremin "equivalent" asides in
  tutorial steps 01, 02, 05, 07, 08.
- In every case, kept the actual substance the reference had informed (e.g. "document material/
  thickness/kerf," "images live at the folder root for GitHub rendering," "runtime BLE params
  instead of one-shot calibration") — only removed the citation/comparison, not the underlying
  content or structure.
- Removed the Calico and OpenTheremin V4 (4 sub-rows: repo, download page, product page, local
  PDF) rows from `references.md` entirely, since those aren't real project dependencies — they
  were agent-facing planning notes that had leaked into a file meant for datasheets/libraries the
  design actually depends on.
- While in `tutorial/steps/README.md`, also fixed a stale example folder tree (showed made-up
  names `01-gather-materials/`, `02-cut-the-platform/` instead of the real
  `01-kit-contents-and-tools/`, `02-assemble-platform/`) and updated its own stale "content still
  TBD" note to match the real per-step status already recorded in `tutorial/README.md`.
- Verified with `scripts/check-docs.sh` (all passing) and a repo-wide grep — zero remaining
  mentions outside `PROGRESS.md`'s own history (left untouched, since it's a log of what
  happened at the time, not a living doc).
- User also asked what `scripts/` is/whether it's needed — explained `check-docs.sh`'s three
  checks (dangling links, PROGRESS.md freshness, BOM datasheet coverage) and recommended keeping
  it since — unlike the empty `exec-plans/`/`previous-revisions/` removed in entry (8) — it's
  small, functional, and has actually caught real issues across this session. Left as-is pending
  user's call.

**Next:** same as prior entries — vector cut file for the laser-cut panel, steps 05/08 still need
real instructions, then a real build to generate photos/video and human-verify the written steps.

## 2026-07-08 (10)

User asked to double-check the repo against their Instructables definition (step-by-step guide +
BOM + laser-cut design files + pictographic circuit diagram + assembly videos). Audited
everything (see chat for the full breakdown given to the user) and fixed one stale doc found
along the way:

- **`tutorial/README.md`'s per-step status tracker was stale** — it marked all 8 steps
  "scaffold only" even though steps 01/02/03/04/06/07 have had real written instructions for
  several sessions now. Updated it to accurately show: 6 steps written-but-unverified, 2 steps
  (05-mount-and-setup, 08-ready-to-use) still genuinely TBD, and called out that **zero steps
  have photos or video yet** regardless of text status (every `media/` folder is empty).
- No other content changes this entry — this was an audit pass, not new build content. Full
  gap list (laser-cut vector file, step photos, assembly/calibration videos, remaining BOM
  vendor links) is unchanged from what's already flagged in `README.md` → Status and
  `laser-cut/README.md`.

**Next:** same as prior entries — vector cut file for the panel, then a real build to generate
photos/video and human-verify the written steps.

## 2026-07-08 (9)

Added the first real content to `laser-cut/` (previously empty scaffold) from design reference
images the user provided, and caught/fixed stale `exec-plan` references left over from session
(8)'s removal of `exec-plans/`.

- **`laser-cut/` populated:** vendored three raster reference images
  (`panel-cut-lines.png` — cut-geometry preview, no labels; `panel-placement-map.png` — labeled
  component + zip-tie map; `panel-system-diagram.png` — full pneumatic+electronics system
  diagram) and rewrote `laser-cut/README.md` around them: panel spec (290×200×3 mm acrylic,
  zip-tied, no screws), the component placement/tie-count table, and cut settings (material/
  thickness known, kerf/power/speed still TBD).
- **Important scope catch:** the provided system diagram documents a **2P2V** (2-valve) variant
  (VALVE1 + VALVE2 both driven, GPIO14 + GPIO15), which conflicts with the repo's actual **2P1V**
  (1-valve) build. Flagged this to the user explicitly rather than silently merging; user chose
  to **keep the build 2P1V** and use the panel only for its mechanical layout. Documented VALVE1's
  zip-tie slot as unpopulated/reserved (mirrors GPIO15 already being reserved in the wiring
  diagram) with an explicit callout in `laser-cut/README.md` pointing back at
  `wiring/pneumatic-plumbing.md` as the authoritative pneumatic/electrical reference. No changes
  made to `BOM.md`'s part list, the wiring diagram, or firmware GPIO config as a result — those
  only change if a 2-valve variant is actually built later.
- **Explicitly flagged as NOT laser-ready:** these are raster (`.png`) references only; no
  vector (`.svg`/`.dxf`) cut file exists yet, and nothing has been physically cut. Called this out
  in `laser-cut/README.md`, `BuildYourOwn/README.md`, and tutorial step 02 rather than implying
  the platform is buildable today.
- Added BOM rows for the panel itself, zip ties, the Ø10 chamber bulkhead fitting, and corner
  feet (all generic/no-vendor-link-yet, same pattern as the existing resistor/DC-adapter rows).
- Filled in `tutorial/steps/02-assemble-platform/README.md` with real instructions (cut → feet →
  bulkhead → zip-tie each component per the placement map → stop before wiring) instead of `TBD`
  placeholders.
- **Stale `exec-plan` reference cleanup** (missed during session (8)'s removal): fixed mentions in
  `core-beliefs.md` (5 spots — replaced "exec-plan"/"exec-plan's decision log" with `PROGRESS.md`
  as the record-keeping location), `BOM.md`, `VERIFICATION.md`, and tutorial steps
  01-kit-contents-and-tools and 03-wire-electronics. Re-grepped the whole repo afterward to
  confirm no more live (non-`PROGRESS.md`-history) references remain.

**Next:** produce the actual vector cut file (`.svg`/`.dxf`) matching `panel-cut-lines.png`
before the panel can be physically cut; capture `images/ide-settings.png` + `images/teaser.png`
from a real build; human-verify wiring/tutorial/platform against a real build.

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

Corrected DIY electronics BOM to match actual parts:

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
  (`RheoBoard_V8_Final/`) — both hardware tracks stay in the repo; DIY remains the agent's
  default focus unless told otherwise. PCB described neutrally in `AGENTS.md`.

Not committed.

## 2026-07-08 (2)

User pointed to [Calico](https://github.com/jsli96/calico) as a closer documentation model than
OpenTheremin alone. Restructured DIY scaffolding to match:

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
scaffolded RheoBoard DIY to **combine** their split documentation model in one repo:

- **GitHub side** (`Electronics/` + `Software/`): added `software/` folder (firmware placeholder);
  existing `BOM.md`, `wiring/`, `laser-cut/` map to their BOM/schematic/mechanical files (DIY
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
top-level trees at all, since DIY is currently the only active track and everything under
`docs/` that wasn't PCB-specific was really about DIY anyway.

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

- **Build Your Own (DIY)** — off-the-shelf modules/dev boards, breadboard/perfboard, wiring
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
  flag DIY as current focus.
- Did **not** touch `RheoBoard_V8_Final/` itself — left it exactly where it is to avoid any
  risk of breaking internal Altium project references.

**Next:** start populating `BuildYourOwn/BOM.md` once parts are chosen — probably worth opening
an exec-plan for the first DIY revision rather than editing ad hoc.

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
