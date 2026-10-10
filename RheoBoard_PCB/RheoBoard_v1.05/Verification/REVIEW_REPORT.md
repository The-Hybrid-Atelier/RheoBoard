# RheoBoard v1.05 review — 2026-10-10

**The selective Altium update is saved: Q11/D37/R85 replace D1 with a lower-loss reverse-polarity stage. ERC and saved/refilled DRC pass with zero errors, warnings, unrouted connections or schematic-parity findings.** Open [RheoBoard_v1.05.kicad_pro](../RheoboardV1/RheoBoard_v1.05.kicad_pro). Supplier preview and physical qualification remain open.

The maintainer delegated the valve-control choice after cross-checking. The valves retain TCA9534 on/off control; pump PWM remains unchanged. FA0520E has no qualified PWM holding duty in its supplier specification, and the PCA9685 shares one PWM frequency across channels. The board outline and mounting positions remain unchanged. The existing Qwiic host/local separation, eFuse, converter and capacitor improvements are retained.

## Current verification

| Check | Result |
| --- | --- |
| Independent change audit | 27/27 checks passed; saved sources unchanged throughout review. |
| Schematic ERC | 0 errors, 0 warnings. |
| Saved PCB DRC, with refilled copper | 0 violations, 0 unconnected items, 0 schematic-parity findings. |
| Components and connectivity | 167 board components/features and 418 schematic pin nodes. Exactly D1 removed; Q11, D37 and R85 added. All 164 retained components keep their values, footprint assignments, electrical groups and pin net names. |
| Placement and pads | All 164 retained placements and copper-pad geometry preserved. U12's 26 physical pad records, including UUIDs and net assignments, remain exact. 452 physical pad records in the current board. |
| Routing | 610 existing trace segments retained exactly; one obsolete D1 stub removed and 11 local segments added. All 301 existing vias and 163 zone definitions retained; one local GND via added. Filled copper recomputed. |
| Rules and outline | Board outline and electrical/design-rule settings unchanged; no new exclusions or relaxed rules. |
| BOM and assembly | 148 fitted parts; 142 matching SMT BOM/CPL references and six manual parts. Every fitted footprint ID matches the saved board. Native coordinates and rotations preserved; no assumed JLCPCB rotation correction. |
| Fabrication | Nine Gerber layers, two drill files and job file freshly exported. ZIP contains exactly those 12 files. Five current native PDF drawings supplied. |
| Portability and preservation | All 138 present 3D links resolve through portable paths. The 129 checked non-session files in original v1 and Altium reference projects are unchanged. KiCad view/lock files are recorded separately. |

Current source hashes and detailed results are in [Altium feature verification](Altium_Features_2026-10-10/README.md) and the release manifest. Native export name escaping on U4 INT and the inherited unused U12 pin-10 alias are documented in the comparison; native schematic parity is clean. The internal fused-input name `Net-(D1-A)` remains intentionally to preserve existing copper identity; D1 itself is removed.

The new power traces use 1.5 mm width and approximately 8.73 mm total length. The trace-sizing calculation is an estimate for 1 oz external copper, not measured thermal qualification. Q11's 35 mΩ maximum at −4.5 V gate drive/25 °C gives about 0.144 W at 2.03 A. Its resistance rises with temperature. The nominal 10 V zener operates below its 5 mA test current; exact gate voltage needs measurement. Q11 is not an ideal-diode reverse-current controller; U12's downstream reverse-current blocking remains. See the [design basis](../REVISION_NOTES.md).

The delivered assembly PDF uses black-and-white mask/silkscreen to keep the new part markings legible. Full reference positions and purchasing identities are in the BOM/CPL; not every inherited reference is printed on silkscreen. No dedicated board fiducial footprint was added. Confirm panel/tooling fiducials and component rotations with the assembler.

Raw native JLC package population warnings are retained: mechanical holes appear in positions but not the schematic BOM, while TP1 and manually fitted pumps are excluded from positions. The delivered SMT pair is reconciled separately to its 142 purchased parts; filtering does not establish supplier acceptance.

The pressure-sensor startup/reset and valve-voltage-margin findings remain accepted prototype risks, as requested by the maintainer. Acceptance does not record a circuit correction or hardware test. The following retained engineering notes describe those unchanged circuits.

## Full-board electrical and layout review

The retained circuit review covers the input fuse, TVS/eFuse, all three bucks, U4 GPIO expander, U5 pressure sensor, U7 PWM driver, four MOSFET load stages, switches, address straps and all bus ports. Both copper layers, solder mask, paste and silkscreen were inspected by region. The 109 × 126.4 mm outline is closed. Power/ground paths and local bypass placement were inspected; no gross routing or mask fault was found. This is a geometry review, not thermal, EMC or transient qualification.

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
2. **Accepted prototype risk — valve-rail voltage margin.** U2's R60 = 270 kΩ / R74 = 30 kΩ divider targets 6.0 V, but specified tolerances permit about 6.231 V before ripple. The exact FA0520E supplier specification lists 5.0–6.0 V operation. Lower the target with tolerance/transient margin (approximately 5.5–5.7 V), or obtain explicit supplier acceptance of the higher voltage. This is a confirmed missing operating margin, not proof the valves will immediately fail. No resistor was changed during review. [Valve specification §3-2](https://cdn-shop.adafruit.com/product-files/4663/4663_C14660_DC_6V.pdf), [calculation](Thorough_2026-10-09/electrical-review.json).
3. Obtain a new JLCPCB placement/stencil preview. Check Q11 pin order, D37 cathode, U12/U13 pin 1, F1, diode cathodes, capacitor polarity and exact substitutions, especially D36. Live availability for every BOM line and final order settings have not been accepted.
4. Test host-only, board-only, both-powered and both power-up orders, leakage/backfeed, bus edges and reconnect behavior. Datasheet powered-off behavior is not a measurement of these boards.
5. Complete rail startup, load-step/PWM, hot-plug, eFuse, temperature, pump/valve and pressure measurements, including narrow power connections and actual accessories. Check the adapter, cables, mechanical fit and hoses. No enclosure, EMC/ESD, thermal simulation or physical testing was performed.

Communication loss intentionally retains commanded outputs while board power stays stable. Pumps remain fixed-direction and intermittent-use. The repository's DIY firmware is not automatically compatible with this PCB.

No board was powered or physically measured during this work. The [JLCPCB v1 top placement](../Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) (order 8815214A_Y73 / SMT026093063736) does not validate v1.05. No manufacturing upload or new order was made.

## Evidence and prior checkpoints

- [Current change, independent checks and package reconciliation](Altium_Features_2026-10-10/README.md).
- [v1 to v1.05 comparison through the 2026-10-09 checkpoint](V1_TO_V105_COMPARISON.md); the current selective input-stage delta is recorded above and in the linked current verification.
- Retained [full-board findings](Thorough_2026-10-09/full-independent-review.json), [electrical calculations](Thorough_2026-10-09/electrical-review.json), and [Qwiic interface review](Qwiic_2026-10-09/interface-engineering-review.json).
- Historical [J18 removal](Remove_J18_2026-10-09/README.md) and [filename-only cleanup](Rename_2026-10-10/validation.json) describe earlier sources, hashes and populations.

Inherited ignored ERC categories remain single_global_label, four_way_junction, simulation_model_issue and footprint_filter. The inherited ignored DRC keys are footprint_filters_mismatch, track_not_centered_on_via and tuning_profile_track_geometries; zero findings does not claim those checks ran. No new instance exclusions or severity relaxations were introduced. All six generic aggregate audits completed; their inherited missing-decoupling heuristic on the jack-to-fuse +12V segment was adjudicated against connectivity, because that segment has no IC supply pin. The raw verdict and reasoning remain in the independent report.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
