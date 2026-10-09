# RheoBoard v1.05 review — 2026-10-09

**The Qwiic master-interface fix is implemented and passes the saved-design checks. Overall assessment remains NEEDS ATTENTION for production: the separate U5 startup/reset risk, supplier placement acceptance and physical qualification remain open.**

Reviewed project: RheoboardV1/1.kicad_pro, main schematic/PCB 1, KiCad 10.0.6 and Konnect 0.13.0. This report supersedes the pre-Qwiic checkpoint. Other schematic files beside the main sheet are retained standalone block references or empty stubs.

## Changes

**J9 is the Qwiic MASTER port**, with standard pins GND, HOST_3V3, HOST_SDA and HOST_SCL. A normal four-wire Qwiic cable connects the USB/battery-powered ESP32 Thing Plus. Host 3.3 V powers only the host side of U13; it is separate from RheoBoard's regulator output. Grounds are common, so this is not galvanic isolation.

**J1, J12 and J17 remain Qwiic SENSOR ports**, with GND, local 3.3 V, SDA and SCL. Their connector bodies, pin order and positions are unchanged. **J18 is LOCAL BUS SERVICE**, bypassing the buffer. Use J9 for the intended master connection. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

Added U13 TCA9517ADGKR, C68/C69 100 nF bypass capacitors, R82/R83 4.7 kΩ host pullups and R84 10 kΩ host-rail discharge. Local bus uses U13 A; host uses B; EN follows host power. The local footprint follows TI's DGK recommended pattern: 1.4 × 0.45 mm pads, 0.65 mm pitch, 4.4 mm row-center span. Copper, mask, paste and pin numbering were checked. [TI TCA9517A](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

C32, C34, C43 and C45 remain **22 µF / 50 V**: approximately **88.3 µF nominal** per pump/valve rail. The earlier 105.9 µF estimate includes positive initial tolerance only, not combined temperature/DC-bias worst case. This follows TI's recommended range but does not establish measured startup or transient stability. [TI TPS563203](https://www.ti.com/lit/gpn/tps563203).

## Current verification

| Check | Result |
| --- | --- |
| Schematic ERC | **0 errors, 0 warnings**, including fresh independent checks after the final schematic spacing correction. |
| Saved PCB DRC | **0 errors, 0 warnings, 0 unrouted connections, 0 schematic-parity findings**; independent saved-file check agrees. |
| Connectivity | All **416 schematic pin nodes** match PCB. 166 components, 89 schematic nets; 90 PCB net names include an unused naming artifact. |
| Structural checks | 0 shorted nets, 0 orphan items, 0 suspicious single-pin nets. |
| Existing layout | All **160 inherited footprints** retain positions, rotations, sides and copper-pad geometry. Board outline is unchanged. J9's three intended host-net changes are verified. |
| New layout | Six added components and routing inspected on copper/mask/paste exports. C68/C69 supply-pad distances are about 2.07/2.22 mm. |
| U12 preservation | All 26 physical pad primitives remain equivalent; object identities and unused-pin aliases were normalized. |
| Rules | Board design settings, ERC settings and net settings match baseline. No new exclusions or reduced clearances. |
| BOM / placement | **147 fitted parts; 140 matching SMT BOM/CPL references; 7 manual parts.** All fitted footprint IDs match. Native placement coordinates and rotations retained. |
| Fabrication / drawings | Nine fresh Gerbers, PTH/NPTH drills and job file; 12-file ZIP matches native exports. Five current PDF drawings supplied. |
| Integrity | Sources bound by SHA-256; all 137 present 3D links resolve using portable paths. All 68 original v1 files match prior hashes. |

The native JLC package reports mismatched raw populations because mechanical features and manual parts appear differently in its BOM and CPL. The delivered SMT pair is independently reconciled to 140 purchased SMT references. Filtering does not change the raw package verdict or establish supplier rotation conventions.

The aggregate completed **all six audits**: one sheet, 257 resolved symbols, 166 footprints, 449 pad records, no coverage diagnostics. Its inherited error says +12V lacks decoupling. That net contains only the jack-to-fuse segment J2.1/F1.1, with no IC supply pin; actual input bypassing is downstream on VIN_RAW/protected VIN. Direct connectivity therefore does not support a missing-IC-decoupling defect. The raw finding is retained. Five dedicated-rail-testpoint suggestions remain optional; component/connector pads provide access.

Inherited ERC exclusions remain single_global_label, four_way_junction, simulation_model_issue and footprint_filter; zero warnings does not claim those checks ran. Previously documented unsupported legacy rule keys remain outside demonstrated coverage. Sandbox-limited attempts were not counted as passes; successful native reruns and their hashes are current evidence.

## Controller and multiple-board conditions

The checked master is **SparkFun ESP32 Thing Plus Micro-B WRL-15663**, GPIO23 SDA / GPIO22 SCL. Other variants require their own pin/power check. The MPR sensor's specified low output is compatible with U13 A; the checked ESP32 satisfies B's stricter input threshold, and B's output-low limit satisfies the ESP32 input requirement. The [interface review](Qwiic_2026-10-09/interface-engineering-review.json) records calculations and primary sources.

Use an external **TCA9548A at 0x71 with exactly one channel enabled** for multiple boards. Supply master 3.3 V, GND and channel SDA/SCL to each J9. Multiple enabled branches would join buffer B sides, which TI prohibits, and cause address collisions. Upstream B-side repeaters and rise-time accelerators are outside the reviewed arrangement.

Keep total selected host-segment pullup current within **3 mA** and selected-branch effective pullups **at least 2.2 kΩ per signal**, including parallel module resistors and tolerances. The branch limit preserves mux ACK low-level margin through its pass resistance. The board's 4.7 kΩ alone satisfies it. Inspect the actual module and measure low levels/rise times with final cables. Start at 100 kHz; maximum buffer rate is 400 kHz. See [assembly and bringup](../ASSEMBLY_AND_BRINGUP.md).

## Remaining production work

1. **U5 startup/reset remains unresolved electrically.** RES still connects only U5.9, R22.2 and TP3.1. Honeywell requires a sufficiently fast VDD rise or reset after stable power; the pullup alone does not guarantee it. Earlier prototype-risk acceptance is not measured qualification. See the [existing finding](Recheck_2026-10-09/sensor-startup-finding.json).
2. Obtain a new JLCPCB placement/stencil preview. Check U12/U13 pin 1, F1, diode cathodes, capacitor polarity and exact substitutions, especially D36. Current stock and final order settings have not been accepted.
3. Test host-only, board-only, both-powered and both power-up orders, leakage/backfeed, bus edges and reconnect behavior. Datasheet powered-off behavior is not a measurement of these boards.
4. Complete rail startup, load-step/PWM, hot-plug, eFuse, temperature, pump/valve and pressure measurements. Check the actual adapter, cables, mechanical fit and hoses. The inherited conservative pump-body/C34 clearance estimate is about 0.22 mm.

Communication loss intentionally retains commanded outputs while board power stays stable. Pumps remain fixed-direction and intermittent-use. The repository's DIY firmware is not automatically compatible with this PCB.

No board was powered or physically measured during this work. The old JLCPCB v1 image does not validate v1.05. No manufacturing upload or new order was made.

## Evidence

- [Independent review](Qwiic_2026-10-09/independent-review.json), [connectivity](Qwiic_2026-10-09/final-connectivity-review.json), [layout](Qwiic_2026-10-09/final-layout-comparison.json), [copper comparison](Qwiic_2026-10-09/final-copper-visual-comparison.json).
- [Independent ERC](Qwiic_2026-10-09/independent-erc.json), [independent DRC](Qwiic_2026-10-09/independent-drc.json), [native DRC](Qwiic_2026-10-09/final-drc-native.json), [aggregate](Qwiic_2026-10-09/run_design_review.json).
- [BOM validation](Qwiic_2026-10-09/bom-validation.json), [manufacturing reconciliation](Qwiic_2026-10-09/manufacturing-reconciliation.json), [drawing validation](Qwiic_2026-10-09/drawing-export-validation.json), [source hashes](Qwiic_2026-10-09/final-hashes.json).

The previous [full review](Recheck_2026-10-09/full-independent-review.json) and evidence outside Qwiic_2026-10-09 describe historical checkpoints. Their 160-component/398-pin/134-SMT counts do not describe the current revision. The release manifest binds current sources, libraries, BOM, exports and reports.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
