# Build verification checklist

Run through before calling a build revision "done." Check items off here, or copy the relevant
items into `PROGRESS.md` for the change and check them off there.

> **Every item below requires a human to physically do or observe something.** An agent must
> never check these off itself — see `AGENTS.md` → "What the agent can and can't verify." Use
> the sign-off block at the bottom to record who actually did the checking.

## Build

- [ ] Electronic wiring matches [`hardware/electronic-wiring/`](hardware/electronic-wiring/)
- [ ] Tube wiring matches [`hardware/tube-wiring/`](hardware/tube-wiring/)
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

## Open Source Hardware Definition compliance

Point-by-point check against the [OSHWA Open Source Hardware Definition](https://www.oshwa.org/definition/)
(the 12 criteria a license/project must meet to count as OSHW at all — distinct from *certification*,
which is the separate registration process checklisted below). Unlike the rest of this file, this
section is about licensing/documentation choices, not physical verification, so an agent can assess
it directly. Re-check whenever a license file, scope statement, or design-file format changes.

1. **Documentation** — hardware design files are in native/editable formats, not obfuscated or
   compiled-only: `hardware/electronic-wiring/generate_wiring_diagram.py` (source) → PNG,
   `laser-cut/generate_panel_vector.py` (source) → `panel.svg`/`panel.dxf`, and
   `RheoBoard-PCB_V9/*.SchDoc`/`*.PcbDoc`/`*.SchLib`/`*.PcbLib` (Altium native format, not
   Gerber-only). Firmware ships as `.ino`/`.cpp`/`.h` source. All free to download from this repo. ✅
2. **Scope** — root `README.md` → License states explicitly what each of the three licenses
   covers, and `hardware/images/README.md` / `hardware/references/README.md` clearly flag
   third-party photos and datasheets as excluded (own attributions/licenses, not ours to
   relicense). ✅
3. **Necessary software** — firmware (`software/rheometer-firmware/`) is MIT-licensed (OSI-approved),
   satisfying 3(b) directly; the OSC API is also documented in
   `software/rheometer-firmware/README.md`, satisfying 3(a) as a fallback. ✅
4. **Derived works** — CERN-OHL-W-2.0, MIT, and CC BY-SA 4.0 all explicitly permit modification,
   redistribution, and manufacture/sale of derivatives. ✅
5. **Free redistribution** — none of the three licenses charge or permit charging a royalty for
   redistribution or derivatives. ✅
6. **Attribution** — all three licenses require attribution without dictating a display format;
   none is more restrictive than that. ✅
7. **No discrimination against persons/groups** — none of the three licenses restrict who may use
   the project. ✅
8. **No discrimination against fields of endeavor** — none of the three licenses carry a
   non-commercial or field-of-use restriction (this was explicitly checked — see OSHWA's own
   guidance that NC/ND-flavored licenses are incompatible with the definition; ours carry neither). ✅
9. **Distribution of license** — all three are perpetual, run-with-the-work licenses; no
   re-execution or additional agreement is required from downstream recipients. ✅
10. **Not specific to a product** — the license grants themselves are generic (CERN-OHL-W-2.0, MIT,
    CC BY-SA 4.0 text); only the accompanying scope/copyright notices name RheoBoard, which is
    normal practice, not a license restriction. ✅
11. **Must not restrict other hardware/software** — CERN-OHL-W's reciprocity only reaches
    modifications of the licensed design itself; a larger system merely incorporating this hardware
    isn't required to be open. ✅
12. **Technology-neutral** — none of the three licenses are tied to a specific technology, part, or
    interface style. ✅

Also noted (from the Definition's introduction, not a numbered criterion, but a stated obligation on
downstream producers): root `README.md` → License now tells anyone who builds/sells units based on
this design to make clear those units aren't sanctioned by the original designer and not to use the
project's names to imply endorsement.

**Net: all 12 criteria are met** by the current three-license setup (CERN-OHL-W-2.0 hardware / MIT
software / CC BY-SA 4.0 documentation) — this is independent of, and already ahead of, OSHWA
certification (below), which additionally requires the physical/registration steps.

## OSHWA self-certification readiness

Checklist for [OSHWA certification](https://certification.oshwa.org/) (free, self-certified,
annual renewal) — **not yet submitted**. Everything below needs a human to review and act on;
an agent can prepare files but can't submit the form or make the underlying physical/legal calls.
See root `README.md` → License for the license breakdown, and `hardware/REVISIONS.md` for
version tracking.

- [x] Hardware, software, and documentation each have an open license applied: root `LICENSE` is
      a clean CERN-OHL-W-2.0 copy (GitHub-detectable, covers hardware), firmware carries its own
      MIT copy (`software/rheometer-firmware/LICENSE`, genuinely different license so kept
      separate — not a duplicate), and documentation (CC BY-SA 4.0) is declared in root
      `README.md` → License with a link to the canonical text, no local copy needed
- [x] Firmware carries `SPDX-License-Identifier` headers
- [x] Hardware-design index files (BOM, electronic wiring, tube wiring, and laser-cut READMEs)
      link to the root `LICENSE`
- [x] Hardware revision scheme documented (`hardware/REVISIONS.md`)
- [x] Machine-readable open-hardware metadata published (`okh-RheoBoard.yml`, Open Know-How manifest) —
      keep in sync with README/REVISIONS when the design changes
- [x] Editable design-file sources exist for the wiring diagram
      (`hardware/electronic-wiring/generate_wiring_diagram.py`) and the laser-cut panel
      (`laser-cut/generate_panel_vector.py` → `panel.svg`/`panel.dxf`)
- [ ] Laser-cut vector file test-fit against real physical parts and corrected if needed
      (human-only — the current file is draft geometry: columns A–E are traced and the Rev B
      seesaw slots in column F are an explicit addition; see `laser-cut/README.md`)
- [ ] Panel physically cut at least once, confirming the vector file is accurate
- [ ] First physical unit labeled with its revision (`hardware/REVISIONS.md` — currently Rev B,
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
