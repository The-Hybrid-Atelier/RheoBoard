# Build verification checklist

Run through before calling a build revision "done." Check items off here, or copy the relevant
items into `PROGRESS.md` for the change and check them off there.

> **Every item below requires a human to physically do or observe something.** An agent must
> never check these off itself — see `AGENTS.md` → "What the agent can and can't verify." Use
> the sign-off block at the bottom to record who actually did the checking.

## Build

- [ ] Wiring matches the diagram in [`hardware/wiring/`](hardware/wiring/) (no ad hoc deviations left undocumented)
- [ ] Continuity/short check on all new connections before first power-up
- [ ] Powers up without excessive current draw or heat on any component
- [ ] Every signal/sensor reads plausible values (not stuck, not noise)

## Parts & docs

- [ ] Every part in [`hardware/BOM.md`](hardware/BOM.md) has a product/datasheet link (see [`hardware/references/`](hardware/references/))
- [ ] No unresolved/placeholder parts
- [ ] [`README.md`](README.md) (step-by-step guide) matches what was actually built (steps, part
      orientation, media) — someone should be able to follow it cold and get the same result

## Documentation

- [ ] `PROGRESS.md` updated with what changed and why (this is the revision history for this
      track — see note in `hardware/BOM.md`, no separate log needed since these files diff natively)

## OSHWA self-certification readiness

Checklist for [OSHWA certification](https://certification.oshwa.org/) (free, self-certified,
annual renewal) — **not yet submitted**. Everything below needs a human to review and act on;
an agent can prepare files but can't submit the form or make the underlying physical/legal calls.
See root `README.md` → License for the license breakdown, and `hardware/REVISIONS.md` for
version tracking.

- [x] Hardware, software, and documentation each have an open license applied (`LICENSE-*.txt`)
- [x] Firmware carries `SPDX-License-Identifier` headers
- [x] Hardware-design index files (BOM, wiring, laser-cut READMEs) link to `LICENSE-HARDWARE.txt`
- [x] Hardware revision scheme documented (`hardware/REVISIONS.md`)
- [x] Editable design-file sources exist for the wiring diagram
      (`hardware/wiring/generate_wiring_diagram.py`) and the laser-cut panel
      (`laser-cut/generate_panel_vector.py` → `panel.svg`/`panel.dxf`)
- [ ] Laser-cut vector file test-fit against real physical parts and corrected if needed
      (human-only — the current file is a traced draft, see `laser-cut/README.md`)
- [ ] Panel physically cut at least once, confirming the vector file is accurate
- [ ] First physical unit labeled with its revision (`hardware/REVISIONS.md` — currently Rev A,
      unbuilt) and that revision/date recorded back into `REVISIONS.md`
- [ ] Third-party components double-checked as either fully open or clearly marked non-open with
      accessible datasheets (currently: all COTS parts, all datasheets vendored in
      `hardware/references/` — re-confirm nothing changed since)
- [ ] OSHWA online self-certification form submitted + Certification Mark License Agreement
      signed (human-only, per-project registration, https://certification.oshwa.org/)
- [ ] If approved: certification mark usage follows OSHWA's guidelines (label hardware with
      version/date, clearly mark which parts are open-source)

## Sign-off

- Verified by:
- Date:
- Notes (anything that failed, was skipped, or needs a follow-up in `PROGRESS.md`):
