# Build verification checklist

Run through before calling a build revision "done." Check items off here, or copy the relevant
items into `PROGRESS.md` for the change and check them off there.

> **Every item below requires a human to physically do or observe something.** An agent must
> never check these off itself — see `AGENTS.md` → "What the agent can and can't verify." Use
> the sign-off block at the bottom to record who actually did the checking.

## Build

- [x] Electronic wiring matches
      [`BuildYourOwn/hardware/electronic-wiring/`](../BuildYourOwn/hardware/electronic-wiring/)
- [x] Tube wiring matches
      [`BuildYourOwn/hardware/tube-wiring/`](../BuildYourOwn/hardware/tube-wiring/)
- [x] Continuity/short check on all new connections before first power-up
- [x] Powers up without excessive current draw or heat on any component
- [x] Every signal/sensor reads plausible values (not stuck, not noise)

## Parts & docs

- [ ] Every part in [`BuildYourOwn/hardware/README.md`](../BuildYourOwn/hardware/README.md) has a
      product/datasheet link (see
      [`BuildYourOwn/hardware/references/`](../BuildYourOwn/hardware/references/))
- [x] No unresolved/placeholder parts
- [x] [`BuildYourOwn/README.md`](../BuildYourOwn/README.md) (step-by-step guide) matches what was actually built (steps, part
      orientation, media) — someone should be able to follow it cold and get the same result

## Documentation

- [x] `PROGRESS.md` updated with what changed and why (this is the revision history for this
      track; no separate hardware revision file is maintained)

## Open Source Hardware Definition compliance

Point-by-point check against the [OSHWA Open Source Hardware Definition](https://www.oshwa.org/definition/)
(the 12 criteria a license/project must meet to count as OSHW at all — distinct from *certification*,
which is the separate registration process checklisted below). Unlike the rest of this file, this
section is about licensing/documentation choices, not physical verification, so an agent can assess
it directly. Re-check whenever a license file, scope statement, or design-file format changes.

1. **Documentation** — hardware design files are in native/editable formats, not obfuscated or
   compiled-only: `BuildYourOwn/hardware/electronic-wiring/generate_wiring_diagram.py` (source) → PNG,
   `BuildYourOwn/laser-cut/generate_panel_vector.py` (source) → `panel.svg`/`panel.dxf`, and
   `../RheoBoard-PCB_V9/*.SchDoc`/`*.PcbDoc`/`*.SchLib`/`*.PcbLib` (Altium native format, not
   Gerber-only). Firmware ships as `.ino`/`.cpp`/`.h` source. All free to download from this repo. ✅
2. **Scope** — root `README.md` → License states explicitly what each of the three licenses
   covers, and `BuildYourOwn/images/README.md` / `BuildYourOwn/hardware/references/README.md`
   clearly flag
   third-party photos and datasheets as excluded (own attributions/licenses, not ours to
   relicense). ✅
3. **Necessary software** — firmware (`BuildYourOwn/software/`) is MIT-licensed (OSI-approved),
   satisfying 3(b) directly; the OSC API is also documented in
   `BuildYourOwn/software/README.md`, satisfying 3(a) as a fallback. ✅
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
See root `README.md` → License for the license breakdown. The current version identifier is stored
in `../okh-RheoBoard.yml`.

- [x] Hardware, software, and documentation each have an open license applied in the single root
      `LICENSE`: CERN-OHL-W-2.0 for hardware, MIT for firmware, and a CC BY-SA 4.0 declaration
      with canonical-text link for documentation
- [x] Firmware carries `SPDX-License-Identifier` headers
- [x] Hardware-design index files (hardware, electronic wiring, tube wiring, and laser-cut READMEs)
      link to the root `LICENSE`
- [x] Machine-readable open-hardware metadata published (`okh-RheoBoard.yml`, Open Know-How manifest) —
      keep in sync with README when the design changes
- [x] Editable design-file sources exist for the wiring diagram
      (`BuildYourOwn/hardware/electronic-wiring/generate_wiring_diagram.py`) and the laser-cut panel
      (`BuildYourOwn/laser-cut/generate_panel_vector.py` → `panel.svg`/`panel.dxf`)
- [x] Rev B laser-cut vector file test-fit against real physical parts and corrected if needed
      (columns A–E originated from the raster trace; the Rev B seesaw slots in column F were an
      explicit addition; the complete panel was physically verified on 2026-07-15)
- [x] Rev B panel physically cut at least once, confirming that revision's vector file was accurate
- [x] Rev B physical unit labeled with its version/date and recorded in `PROGRESS.md`
- [x] Rev C non-L298N placement envelopes matched to manufacturer dimensions (including the
      user-confirmed Adafruit 4699 / ZR370-02PM pump drawing); generic L298N outlines intentionally
      remain placeholders pending the full physical test-fit
- [ ] Rev C laser-cut vector printed/cut at 1:1 and test-fit against all real parts
- [ ] Rev C panel physically cut, assembled, and labeled with the version/date from
      `okh-RheoBoard.yml`
- [ ] Third-party components double-checked as either fully open or clearly marked non-open with
      accessible datasheets (currently: all COTS parts, all datasheets vendored in
      `hardware/references/` — re-confirm nothing changed since)
- [ ] OSHWA online self-certification form submitted + Certification Mark License Agreement
      signed (human-only, per-project registration, https://certification.oshwa.org/)
- [ ] If approved: certification mark usage follows OSHWA's guidelines (label hardware with
      version/date, clearly mark which parts are open-source)

## Sign-off

- Verified by: Charlie Vuong
- Date: 2026-07-15
- Notes: User confirmed the Rev B unit was cut from the current `panel.svg`/`.dxf`, includes the
  ATtiny1616 seesaw and one-valve configuration, matches both wiring guides, and passed continuity,
  power, sensor, REP, pressure-trace, and documentation checks. BOM link completeness, third-party
  component review, and OSHWA submission remain open.
