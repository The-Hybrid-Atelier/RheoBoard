# RheoBoard v1.05 review — 2026-10-09

Filename update, 2026-10-10: open [RheoBoard_v1.05.kicad_pro](../RheoboardV1/RheoBoard_v1.05.kicad_pro). Main schematic/PCB names now match that project. [Rename validation](Rename_2026-10-10/validation.json) proves all schematic netlists and all PCB/library bytes were preserved; native ERC/DRC results are unchanged. Earlier source paths and hashes below describe their original checkpoints. The current release manifest records the new paths and source hashes. This is a filename change, not a new hardware qualification.

**Latest change: J18 removed; fresh ERC and saved/refilled DRC pass with zero errors, warnings, unconnected items or parity findings. J3 remains the local debug header and J9 remains the Qwiic master port. The maintainer accepted the U5 startup/reset and valve-rail voltage-margin risks for this prototype on 2026-10-09 and requested no further action on them. Supplier acceptance and physical qualification remain open.**

The [J18 removal verification](Remove_J18_2026-10-09/README.md) binds the current saved sources and regenerated manufacturing outputs. It confirms 165 components, 413 schematic pin nodes, 146 fitted parts, 140 SMT parts and six manually fitted parts. All retained component values, pad geometry, placements and pin connections are unchanged. The earlier full-board review below remains supporting evidence for the retained circuitry; its pre-removal source hashes and counts are historical.

The two findings below are retained as engineering evidence; acceptance does not record a circuit correction or a successful hardware test. If the valve voltage is adjusted later, the feedback-divider values must be selected appropriately; simply removing a divider resistor is not an equivalent voltage adjustment. No CAD or manufacturing files were changed for this disposition.

Reviewed project: RheoboardV1/1.kicad_pro, main schematic/PCB 1, KiCad 10.0.6 and Konnect 0.13.0. This full-board review supersedes the earlier Qwiic-only acceptance. All 39 CAD and library files remained unchanged during this review. Other schematic files beside the main sheet are retained standalone block references or empty stubs; fresh hierarchy inspection confirms one active sheet.

## Changes

**J9 is the Qwiic MASTER port**, with standard pins GND, HOST_3V3, HOST_SDA and HOST_SCL. A normal four-wire Qwiic cable connects the USB/battery-powered ESP32 Thing Plus. Host 3.3 V powers only the host side of U13; it is separate from RheoBoard's regulator output. Grounds are common, so this is not galvanic isolation.

**J1, J12 and J17 remain Qwiic SENSOR ports**, with GND, local 3.3 V, SDA and SCL. Their connector bodies, pin order and positions are unchanged. **J3 remains the four-pin local debug header**, bypassing the buffer; J18 has been removed. Use J9 for the intended master connection. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

Added U13 TCA9517ADGKR, C68/C69 100 nF bypass capacitors, R82/R83 4.7 kΩ host pullups and R84 10 kΩ host-rail discharge. Local bus uses U13 A; host uses B; EN follows host power. The local footprint follows TI's DGK recommended pattern: 1.4 × 0.45 mm pads, 0.65 mm pitch, 4.4 mm row-center span. Copper, mask, paste and pin numbering were checked. [TI TCA9517A](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

C32, C34, C43 and C45 remain **22 µF / 50 V**: approximately **88.3 µF nominal** per pump/valve rail. The earlier 105.9 µF estimate includes positive initial tolerance only, not combined temperature/DC-bias worst case. This follows TI's recommended range but does not establish measured startup or transient stability. [TI TPS563203](https://www.ti.com/lit/gpn/tps563203).

## Pre-removal full-board verification

The pre-removal full-board results are saved in **Thorough_2026-10-09**. Qwiic_2026-10-09 remains implementation evidence. The following table records that historical 166-component checkpoint; current removal checks are linked above.

| Check | Result |
| --- | --- |
| Schematic ERC | **0 errors, 0 warnings** in this full-board recheck. |
| Saved PCB DRC | **0 errors, 0 warnings, 0 unrouted connections, 0 schematic-parity findings**; saved-copper and analysis-refilled checks agree. No refill was saved over the design. |
| Connectivity | All **416 schematic pin nodes** electrically reconcile with the PCB; **85/85 circuit assertions pass**. 166 components, 89 schematic nets; 90 PCB net names include the isolated U12.10 same-pin NC alias documented in the independent report. |
| Structural checks | 0 shorted nets, 0 orphan items, 0 suspicious single-pin nets. |
| Existing layout | All **160 inherited footprints** retain positions, rotations, sides and copper-pad geometry. Board outline is unchanged. J9's three intended host-net changes are verified. |
| New layout | Six added components and routing inspected on copper/mask/paste exports. C68/C69 supply-pad distances are about 2.07/2.22 mm. |
| U12 preservation | All 26 physical pad primitives remain equivalent; object identities and unused-pin aliases were normalized. |
| Rules | Board design settings, ERC settings and net settings match baseline. No new exclusions or reduced clearances. |
| BOM / placement | **147 fitted parts; 140 matching SMT BOM/CPL references; 7 manual parts.** All fitted footprint IDs match. Native placement coordinates and rotations retained. |
| Fabrication / drawings | All nine Gerbers, PTH/NPTH drills and job file match fresh native exports except creation timestamps. The 12-file ZIP matches the packaged files; fresh native placement rows match the delivered native positions. Five current PDF drawings remain supplied. |
| Integrity | Sources bound by SHA-256; all 137 present 3D links resolve using portable paths. The prior integrity check of all 68 original v1 files is retained as baseline evidence; v1 was not edited during this review. |

The native JLC package reports mismatched raw populations because mechanical features and manual parts appear differently in its BOM and CPL. The delivered SMT pair is independently reconciled to 140 purchased SMT references. Filtering does not change the raw package verdict or establish supplier rotation conventions.

No dedicated board fiducial footprint appears in the inventory. Confirm panel/tooling fiducials with the assembler. Dense component identification is available in the assembly drawing; not every reference is printed on silkscreen.

The aggregate completed **all six audits**: one sheet, 257 resolved symbols, 166 footprints, 449 pad records, no coverage diagnostics. Its inherited error says +12V lacks decoupling. That net contains only the jack-to-fuse segment J2.1/F1.1, with no IC supply pin; actual input bypassing is downstream on VIN_RAW/protected VIN. Direct connectivity therefore does not support a missing-IC-decoupling defect. The raw finding is retained. Five dedicated-rail-testpoint suggestions remain optional; component/connector pads provide access.

Inherited ERC exclusions remain single_global_label, four_way_junction, simulation_model_issue and footprint_filter; zero warnings does not claim those checks ran. Previously documented unsupported legacy rule keys remain outside demonstrated coverage. Sandbox-limited attempts were not counted as passes; successful native reruns and their hashes are current evidence.

## Full-board electrical and layout review

The review covers the input fuse/diode/TVS/eFuse, all three bucks, U4 GPIO expander, U5 pressure sensor, U7 PWM driver, four MOSFET load stages, switches, address straps and all bus ports. Both copper layers, solder mask, paste and silkscreen were inspected by region. The 109 × 126.4 mm outline is closed. Power/ground paths and local bypass placement were inspected; no gross routing or mask fault was found. This is a geometry review, not thermal, EMC or transient qualification.

U12 retains a short **0.3 mm × 2 mm output power neck**, with another short 0.3 mm input approach. These are candidates for widening and temperature measurement near the current limit, not demonstrated overheating. The nearest buck input-capacitor pads are about 2.32–2.35 mm from their supply pins; U13 bypass placement remains about 2.07/2.22 mm. No exposed switch-node trace openings were found beyond intended component pads. Pump/hose fit and the inherited conservative pump-body/C34 clearance estimate of about 0.22 mm still need physical confirmation.

Fresh netlist calculations give these output setpoints. Ranges include the regulator's full-temperature reference specification and initial resistor tolerance; they exclude resistor temperature drift, ripple and load transients.

| Rail | Nominal | Calculated range | Onboard nominal output capacitance |
| --- | --- | --- | --- |
| U1 pumps | 4.52 V | 4.354–4.691 V | 88.3 µF |
| U2 valves | 6.00 V | **5.775–6.231 V** | 88.3 µF |
| U3 logic/sensors | 3.30 V | 3.205–3.397 V | 64.51 µF |

The three 100 kΩ VIN-to-EN resistors are explicitly permitted by TI; internal EN clamping is described in its datasheet. The eFuse divider gives about 9.36 V turn-on at VIN_RAW. Its 1.65 kΩ setting is approximately 2 A, and the 390 kΩ clamp selection remains below the buck's steady-state input ceiling. These are component specifications and calculations, not measured surge protection. [Buck datasheet](https://www.ti.com/lit/gpn/tps563203), [eFuse datasheet](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

Two nominal 500 mA pumps plus two valves at the specified 276 mA upper current give about 7.83 W of load power. At an illustrative 85% conversion efficiency this is about 0.77 A from 12 V, before logic, accessories and additional input-stage losses. Pump start/stall current is not established by that running-current estimate. The 2 A input protection does not establish a guaranteed total output-current budget. [Pump](https://www.adafruit.com/product/4699), [valve specification](https://cdn-shop.adafruit.com/product-files/4663/4663_C14660_DC_6V.pdf).

## Controller and multiple-board conditions

The user confirmed **SparkFun ESP32 Thing Plus Micro-B WRL-15663**, GPIO23 SDA / GPIO22 SCL, through its [product page](https://www.sparkfun.com/sparkfun-thing-plus-esp32-wroom-micro-b.html). The current page links the same reviewed schematic. The MPR sensor's specified low output is compatible with U13 A; the checked ESP32 satisfies B's stricter input threshold, and B's output-low limit satisfies the ESP32 input requirement. The [interface review](Qwiic_2026-10-09/interface-engineering-review.json) records calculations and primary sources.

Use an external **TCA9548A at 0x71 with exactly one channel enabled** for multiple boards. Supply master 3.3 V, GND and channel SDA/SCL to each J9. Multiple enabled branches would join buffer B sides, which TI prohibits, and cause address collisions. Upstream B-side repeaters and rise-time accelerators are outside the reviewed arrangement.

Keep total selected host-segment pullup current within **3 mA** and selected-branch effective pullups **at least 2.2 kΩ per signal**, including parallel module resistors and tolerances. The branch limit preserves mux ACK low-level margin through its pass resistance. The board's 4.7 kΩ alone satisfies it. Inspect the actual module and measure low levels/rise times with final cables. Start at 100 kHz; maximum buffer rate is 400 kHz. See [assembly and bringup](../ASSEMBLY_AND_BRINGUP.md).

The additional sensor modules are unspecified. Their parallel pullups, addresses, current demand and added capacitance must be included before qualifying the complete system. The 64.51 µF onboard logic-rail total does not grant an unrestricted capacitor budget, and the buck's rating is not a Qwiic cable-current rating.

## Accepted findings and remaining qualification

1. **U5 startup/reset remains unresolved electrically.** RES still connects only U5.9, R22.2 and TP3.1. Honeywell requires a sufficiently fast VDD rise or reset after stable power; the pullup alone does not guarantee it. Earlier prototype-risk acceptance is not measured qualification. See the [existing finding](Recheck_2026-10-09/sensor-startup-finding.json).
2. **New finding — valve-rail voltage margin.** U2's R60 = 270 kΩ / R74 = 30 kΩ divider targets 6.0 V, but specified tolerances permit about 6.231 V before ripple. The exact FA0520E supplier specification lists 5.0–6.0 V operation. Lower the target with tolerance/transient margin (approximately 5.5–5.7 V), or obtain explicit supplier acceptance of the higher voltage. This is a confirmed missing operating margin, not proof the valves will immediately fail. No resistor was changed during review. [Valve specification §3-2](https://cdn-shop.adafruit.com/product-files/4663/4663_C14660_DC_6V.pdf), [calculation](Thorough_2026-10-09/electrical-review.json).
3. Obtain a new JLCPCB placement/stencil preview. Check U12/U13 pin 1, F1, diode cathodes, capacitor polarity and exact substitutions, especially D36. Live availability for every BOM line and final order settings have not been accepted.
4. Test host-only, board-only, both-powered and both power-up orders, leakage/backfeed, bus edges and reconnect behavior. Datasheet powered-off behavior is not a measurement of these boards.
5. Complete rail startup, load-step/PWM, hot-plug, eFuse, temperature, pump/valve and pressure measurements, including narrow power connections and actual accessories. Check the adapter, cables, mechanical fit and hoses. No enclosure, EMC/ESD, thermal simulation or physical testing was performed.

Communication loss intentionally retains commanded outputs while board power stays stable. Pumps remain fixed-direction and intermittent-use. The repository's DIY firmware is not automatically compatible with this PCB.

No board was powered or physically measured during this work. The [JLCPCB v1 top placement interpretation](../../interpretation/8815214A_Y73_SMT026093063736_top.png) (order 8815214A_Y73 / SMT026093063736) does not validate v1.05. No manufacturing upload or new order was made.

## Evidence

- [Complete v1 versus v1.05 schematic and layout comparison](V1_TO_V105_COMPARISON.md), including every current component reference and fresh drawings.
- [Full independent findings and checklist coverage](Thorough_2026-10-09/full-independent-review.json), [85 circuit assertions](Thorough_2026-10-09/full-block-checks.json), [connectivity and layout inventory](Thorough_2026-10-09/full-connectivity-layout-inventory.json).
- Current [J18 removal checks and source binding](Remove_J18_2026-10-09/README.md).
- Pre-removal full-board check: [ERC](Thorough_2026-10-09/run_erc-decoded.json), [DRC](Thorough_2026-10-09/get_drc_violations-decoded.json), [DRC with analysis-only refill](Thorough_2026-10-09/drc-refill-decoded.json), [aggregate and coverage](Thorough_2026-10-09/run_design_review-decoded.json).
- Retained [electrical calculations](Thorough_2026-10-09/electrical-review.json), historical [BOM/placement/fabrication reconciliation](Thorough_2026-10-09/artifact-review.json), historical [CAD/library hashes](Thorough_2026-10-09/design-integrity.json).

- [Independent review](Qwiic_2026-10-09/independent-review.json), [connectivity](Qwiic_2026-10-09/final-connectivity-review.json), [layout](Qwiic_2026-10-09/final-layout-comparison.json), [copper comparison](Qwiic_2026-10-09/final-copper-visual-comparison.json).
- [Independent ERC](Qwiic_2026-10-09/independent-erc.json), [independent DRC](Qwiic_2026-10-09/independent-drc.json), [native DRC](Qwiic_2026-10-09/final-drc-native.json), [aggregate](Qwiic_2026-10-09/run_design_review.json).
- [BOM validation](Qwiic_2026-10-09/bom-validation.json), [manufacturing reconciliation](Qwiic_2026-10-09/manufacturing-reconciliation.json), [drawing validation](Qwiic_2026-10-09/drawing-export-validation.json), [source hashes](Qwiic_2026-10-09/final-hashes.json).

Remove_J18_2026-10-09 contains the current change verification. Thorough_2026-10-09, Qwiic_2026-10-09 and the previous [full review](Recheck_2026-10-09/full-independent-review.json) describe historical checkpoints. Their differing component/pin counts do not describe the current revision. The release manifest binds current sources, libraries, BOM, exports and reports.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
