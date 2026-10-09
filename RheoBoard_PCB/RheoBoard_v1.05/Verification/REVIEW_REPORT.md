# RheoBoard v1.05 review — 2026-10-09

**Assessment: NEEDS ATTENTION. The full recheck found one high-priority pressure-sensor startup/reset risk that was missed in the earlier review. ERC and DRC still pass, but production readiness is not established until that requirement is resolved. Supplier acceptance and physical qualification also remain open.**

Reviewed project: `RheoboardV1/1.kicad_pro`, main schematic/PCB `1`, KiCad 10.0.6 and Konnect 0.13.0. The other schematic files are retained standalone block references or empty stubs, not omitted hierarchy sheets. The final aggregate covered one sheet, 245 resolved schematic symbols (including power symbols), 160 board footprints and 431 pad records. All six audits completed with no coverage diagnostics.

## Direct checks

| Check | Result and scope |
| --- | --- |
| Saved PCB DRC | 0 errors, 0 warnings, 0 unconnected items, 0 schematic-parity mismatches. Independent saved-file DRC agrees; the previously verified filled copper is retained through the symbol-identity update. |
| Schematic ERC | 0 errors, 0 warnings after the controlled grid and library cleanup. Independent saved-file ERC agrees. |
| Short/orphan checks | 0 shorted nets, 0 orphan items, 0 suspicious single-pin nets. Exported netlist's 15 one-pin nets are intentionally unused pins. |
| Schematic/PCB connectivity | All 398 schematic pin nodes accounted for. 86 schematic nets; the board's 87 net names include one reviewed unused-pin naming artifact. |
| Original connectivity | All original pin groups preserved except the intentional D1/input-protection split; new protection and J18 connections verified. |
| BOM and placement | 141 fitted parts; 134 matching SMT BOM/CPL references and 7 manual parts. Fitted references and footprints agree with PCB; exported placement coordinates/rotations preserved. |
| Fabrication files | Nine Gerber layers, PTH and NPTH drills, Gerber job file; closed 109.0 × 126.4 mm outline. 330 plated holes/slots and 3 non-plated holes register with the respective copper/masks. |
| Probe-pad correction | TP2–TP9 now 1.00 mm copper / 0.50 mm hole, 0.25 mm radial ring, confirmed on both copper exports. |
| Other holes | Minimum fitted-component PTH ring 0.30 mm; 297 vias use 0.50/0.30 mm land/drill. Via limits are distinct from component-PTH limits. Checks use the documented two-layer 1 oz basis. |
| U12 footprint/stencil | RPW0010A power lands and split paste windows checked against the selected package; no new defect found in copper/mask/paste inspection. |
| Libraries and placement | All 139 present 3D links resolve without absolute paths. All 160 positions/rotations retained through the final probe-pad update; only eight intended pad geometries changed. |
| Original preservation | All 68 original v1 files and the original submitted BOM remain unchanged. |

## New finding from the full recheck

**U5 pressure-sensor startup is not guaranteed by the present reset circuit.** Honeywell requires a VDD rise of at least 10 V/ms or use of RES after power stabilizes. U3's typical 1.4 ms soft-start gives an illustrative average of about 2.36 V/ms for 3.3 V. This is an inference from the specifications, not a measured waveform. [Honeywell MPR, section 4, page 11](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf), [TI TPS563203, section 6.3.3](https://www.ti.com/lit/gpn/tps563203).

The fresh netlist shows RES connects only to U5 pin 9, R22 pin 2 and TP3 pin 1; R22 is a 10 kΩ pull-up to 3.3 V. There is no active reset controller or reset connection through J18. A pull-up alone does not guarantee a reset after the rail stabilizes. The sensor could fail to initialize consistently after startup or a brownout; this does not prove that every board fails.

Before production approval, add a suitable reset supervisor or a properly sequenced host reset connection, with TP3 available as an access point, and verify VDD/RES during cold starts and brownouts. Alternatively, establish manufacturer-accepted compliance from measured worst-case supply ramps. This review did not change the circuit, source files, BOM or delivered fabrication files.

The fresh recheck passed all **61 electrical connectivity/value assertions**, reconciled all **398 schematic pin connections** with the board, and returned **0 ERC errors/warnings and 0 DRC errors/warnings/unconnected/parity items**. All **123 previously delivered file hashes** matched before report updates. New native position output is byte-identical to the delivered reference. Fresh nine-layer Gerber geometry, both drill files and the job file match the delivered package after resolving drawing order and date metadata. Ten BOM checks pass: 141 fitted parts, including 134 matching SMT BOM/CPL references and seven manual parts. These checks do not evaluate dynamic sensor initialization.

See the [fresh independent full review](Recheck_2026-10-09/full-independent-review.json), [sensor finding](Recheck_2026-10-09/sensor-startup-finding.json), [61 assertions](Recheck_2026-10-09/expanded-electrical-assertions.json) and [fresh fabrication comparison](Recheck_2026-10-09/fresh-fabrication-geometry-comparison.json).

## Schematic warning cleanup and audit interpretation

All 759 inherited warnings have been resolved. Connected schematic blocks were aligned to the existing 1.27 mm connection grid with exported-netlist comparisons between changes. A project-local `PCA9685_Grid` symbol corrects the inherited U7 pin-origin offset while preserving all 28 pin numbers, names, types and lengths. U8/U11 were recreated from the consistent project-local pump symbol, and their PCB identity links were updated. Component value strings and purchasing selections remain intact. Overlapping pump labels, the input annotations, three section captions and the PCA9685 address note were corrected in the drawing.

The cleanup preserves all 86 named net partitions and 398 pin nodes, including pin functions/types and intentionally unused pins. Rule severities, connection-grid settings and exclusions were not relaxed to obtain zero warnings. Inherited ERC exclusions remain `single_global_label`, `four_way_junction`, `simulation_model_issue` and `footprint_filter`; the zero-warning result does not claim those checks ran.

Native export comparisons confirm unchanged fabrication geometry, component/package geometry and board outline. IPC2581 represents 24 F.Fab rectangles with swapped width/height between checkpoints, and repeated exports of the byte-identical current board also vary some courtyard rectangle representations. This limits claims about drawing-export identity; it does not change the verified copper, pads, holes, masks, paste or placement equivalence. The independent review preserves the details rather than treating every drawing primitive as identical.

The raw aggregate says `NOT READY` because its generic power-rail heuristic finds no capacitor on `+12V`. The netlist shows that segment contains only J2 pin 1 and F1 pin 1 (plus a power flag), with no IC supply pin. U12 input bypass C65/C66 is on VIN_RAW; C67 and the six converter input capacitors are on protected VIN. The finding is therefore a false positive for the claimed missing IC decoupling. The raw report is preserved alongside this adjudication.

The five informational suggestions concern dedicated test points on +12V, +3V3, +4V5, +6V and VIN. They are testability improvements; existing connector/component pads provide measurement access. Dedicated labelled rail pads would simplify a future production fixture. Verified measurement nodes are +12V at F1.1/J2.1, +3V3 at J3.2, pump rail at C32.1, valve rail at C43.1, and protected VIN at C67.1.

U12's unused ITIMER pin 10 has two overlapping footprint primitives with separately generated no-connect names. Exported copper confirms the intended continuous L-shaped physical land with no external route and 0.20 mm clearance to adjacent lands. This is a naming artifact, not an additional functional connection.

Three inherited ignored project keys could not be mapped to the current KiCad 10 severity UI: `footprint_filters_mismatch`, `track_not_centered_on_via`, `tuning_profile_track_geometries`. They remain outside demonstrated rule coverage. All displayed current rule severities were enabled; the zero DRC result is scoped to the configured supported checks. The schematic title-block revision remains blank; the PCB title/revision identifies 1.05.

An earlier full-design review process could not read the schematic and returned incomplete aggregate coverage. That attempt is retained in working evidence and is not treated as a pass. The current post-cleanup run completed all six audits without coverage diagnostics; independent KiCad DRC/ERC and exported-netlist checks corroborate the current state.

## Electrical and system conditions

The approved protection path is J2 → F1 → D1 → TPS259472A → three converters, with raw-input TVS, approximately 2 A current limit, 13.8 V nominal clamp and controlled startup. Selected component values and pin mapping agree with the exported netlist. Use a regulated center-positive 12 V source; the actual adapter and simultaneous startup/load current remain to be established.

Reduced pump/valve bulk capacitors bring each rail to approximately 88.3 µF nominal and 105.9 µF at the stated positive tolerances, below the 110 µF recommendation used from the [TI converter datasheet](https://www.ti.com/lit/gpn/tps563203). This calculation does not replace startup and load-transient measurement.

The board supplies its own 3.3 V. New J18 provides only GND/SDA/SCL for the master or mux. Multiple boards use one branch each of the external TCA9548A arrangement documented in [assembly and bringup](../ASSEMBLY_AND_BRINGUP.md); their fixed-address devices cannot all share one active bus segment. Do not join independently regulated 3.3 V outputs.

The design intentionally retains the last commanded outputs when communication stops. Pumps remain fixed-direction and intermittent-use devices. Firmware must handle initialization, PWM, mux selection, reconnection, button reading and run/rest limits. The repository's DIY firmware is not automatically compatible with this PCB.

## What remains before repeated production

1. Resolve U5 startup/reset as described above. Obtain a new JLCPCB v1.05 placement/stencil preview and check polarity, IC pin 1, the centered fuse and substitutions. Confirm exact D36 SMBJ13A identity because the distributor attribute table conflicts with the manufacturer part number. Current stock and final order settings have not been accepted.
2. On first assembled boards, measure the three rails, startup/hot-plug peaks, motor-start and simultaneous-load behavior, flyback/PWM waveforms, input current and component temperatures over the intended run/rest cycle. Confirm eFuse limiting does not cause startup cycling or brownouts.
3. Test the chosen master, mux and actual cables: per-board addressing, state retention/reconnection, bus rise times and absence of supply backfeed. Select the adapter using measured requirements.
4. Physically check pump terminal fit, support, nearby clearance, hoses, leaks and pressure calibration. The conservative pump-body/C34 rectangular clearance estimate is only about 0.22 mm, so dry-fit actual parts before soldering the pumps. CAD outlines cannot establish those results.

The old JLCPCB image and already manufactured v1 boards do not validate this changed v1.05 design. No board has been powered or measured by this review. No new order or upload was made.

## Evidence

See the [current independent cleanup review](independent-final-review.json), [DRC](final-drc.json), [ERC](final-erc.json), [raw aggregate](aggregate-review-raw.json), [pin map](board-pin-map-validation.json), [original-net comparison](connectivity-comparison.json), [PCB footprint comparison](board-export-comparison.json), [unchanged-output checks](artifact-preservation.json), [BOM validation](bom-validation.json), [manufacturing reconciliation](manufacturing-reconciliation.json), and [source hashes](checked-source-hashes.json).

The [full design audit before grid cleanup](independent-pre-cleanup-review.json), its [24 electrical assertions](electrical-assertions.json) and [manufacturing assumptions](manufacturing-contract.json) are retained as historical engineering evidence. Their electrical, part-selection and fabrication conclusions carry forward through the current connectivity and fabrication-equivalence checks; their old source hashes and 759-warning count describe the earlier checkpoint. Current ERC/DRC counts are in the reports linked above. Manufacturing files and drawings are indexed in [Manufacturing](../Manufacturing/README.md).

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
