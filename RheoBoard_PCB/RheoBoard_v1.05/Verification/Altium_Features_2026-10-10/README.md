# Selective Altium features applied to v1.05 — 2026-10-10

**CAD change audit passed: 27/27 independent checks. ERC and saved/refilled DRC report zero findings, unrouted connections and schematic-parity findings.** This verifies the scoped change and preservation of the remaining CAD; it is not physical qualification or supplier acceptance.

## Implemented change

D1 is replaced with Q11 DMP6023LE-13 (60 V P-MOS, C154901), D37 BZT52C10-7-F (nominal 10 V zener, C155227) and R85 10 kΩ (C25804). The input is J2 → F1 → Q11 drain/tab → Q11 source/VIN_RAW → retained U12 eFuse → the three converters. D37 cathode connects to source, anode to gate; R85 connects gate to GND. The legacy fused-input net name `Net-(D1-A)` is retained intentionally; there is no D1 component.

Valves keep their existing on/off control. Pump PWM, Qwiic master/local interface, fuse/eFuse, converter values, board dimensions and mounting positions remain. No board shrink or firmware change is included. The [design basis](../../REVISION_NOTES.md) explains the part selection and physical limits.

## Verified scope

| Item | Result |
| --- | --- |
| Population | 165 → 167 components/features; 413 → 418 schematic pin nodes. D1 removed; Q11/D37/R85 added. |
| Retained design | All 164 retained values, footprint assignments, electrical groups, pin net names, placements and copper pads preserved. |
| U12 | All 26 saved physical pad records exactly preserved, including identities and original unused-pin aliases. |
| Routes and copper | 610 original traces retained, one old D1 stub replaced, 11 new local segments and one GND via. All 301 old vias and 163 zone definitions preserved. Copper refilled around the new stage. |
| Rules | Same board/ ERC / netclass settings and outline; no new exclusions or relaxed rules. |
| Assembly metadata | U8/U11/TP1 position exclusions preserved. 148 fitted parts; 142 SMT and six manual. |
| Manufacturing | All 142 SMT references, footprint IDs, native coordinates and rotations reconcile. ZIP matches exactly 12 fresh fabrication files. Five native PDF drawings refreshed. |
| Sources | All 39 source/library hashes match owner and independent handoffs; original v1 and Altium non-session files unchanged. |

The generic six-audit tool completed all six audits. Its raw “NOT READY” verdict comes from the inherited request for a capacitor on the jack-to-fuse +12V segment. Direct connectivity shows no IC supply on that segment; local input bypassing remains on VIN_RAW. The [independent report](final-independent-review.json) retains and adjudicates that heuristic, plus five optional rail-testpoint suggestions. Clean ERC/DRC applies to the unchanged configured rule coverage, not to ignored categories.

The [raw native package population warning](manufacturing-reconciliation.json) is also retained: board-only holes and manually fitted/measurement features do not have identical raw BOM/CPL inclusion. The delivered pair is explicitly reconciled to the purchased SMT population. Supplier rotation conventions and placement acceptance still require the new JLCPCB preview.

## Evidence

- [Independent review and 27 checks](final-independent-review.json), [saved design comparison](saved-design-comparison.json), [rules and U12](rules-and-U12-comparison.json), [assembly exclusions](assembly-flag-comparison.json).
- [Independent ERC](final-erc-decoded.json), [saved DRC](final-saved-drc-decoded.json), [analysis-refilled DRC](final-refilled-drc-decoded.json), [short check](final-short-check-decoded.json), [aggregate raw findings](final-aggregate-decoded.json).
- [Owner preservation proof](retention-proof.json), [electrical delta](electrical-delta.json), [routing calculation](input-routing-basis.json), [part-selection review](proposed-input-review.json).
- [BOM validation](bom-validation.json), [manufacturing reconciliation](manufacturing-reconciliation.json), [3D link checks](model-path-validation.json), [original project preservation](preserved-original-projects.json).
- [Source hashes](source-hashes.json), [schematic netlist](schematic.net), [board IPC-2581 export](board.xml).
- [Input schematic](input-schematic.png), [input copper and markings](input-layout.png), [assembly detail](assembly-input.png).

## Qualification still open

Measure startup, voltage drop, gate-source voltage, temperature and required input-removal/reversal transients with the actual supply and loads. A single P-MOS is not a controlled ideal diode while on; U12's downstream reverse-current blocking remains. The 0.144 W estimate uses 35 mΩ at 25 °C and 2.03 A; hot resistance and actual copper affect temperature. The low-current zener bias does not guarantee exactly 10 V.

The maintainer's accepted pressure-reset and valve-voltage-margin prototype risks remain unchanged. No physical board was powered or measured, and no order was submitted. Follow the [assembly and bringup guide](../../ASSEMBLY_AND_BRINGUP.md).

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
