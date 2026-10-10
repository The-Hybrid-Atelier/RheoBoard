# RheoBoard v1.05 review — 2026-10-10

**The PCA/button update is saved and independently checked. ERC and saved/refilled DRC report zero findings, unconnected items and schematic-parity findings under the retained rule settings.** Open [RheoBoard_v1.05.kicad_pro](../RheoboardV1/RheoBoard_v1.05.kicad_pro). Supplier placement acceptance and physical qualification remain open.

U7 controls both pumps and valves, and J19 carries the four buttons to separate ESP32 GPIO inputs. This implements the user's selected Altium-style arrangement. U4 and its dedicated support parts are removed. Valves remain steady ON/OFF; no PWM holding-power change is introduced. J19 has no supply pin; R16–R19 use HOST_3V3 from J9 and retain their 100 nF filters. Use the updated [wiring and control instructions](../ASSEMBLY_AND_BRINGUP.md).

## Current verification

| Check | Result |
| --- | --- |
| Independent change audit | 36/36 checks passed; see the source-bound detailed report. |
| ERC / DRC | 0 schematic errors/warnings; 0 saved/refilled PCB findings, unconnected items and parity findings. |
| Connectivity | 160 components/features and 394 schematic pin nodes. Only the specified U4 removal, valve reassignment and button-header/host-pullup changes. |
| Layout | J19 and R16–R19 placed in the former U4 region. Independent comparison records changed tracks/zones; all unrelated retained placements, pads and routes checked. U12's 26 physical pad records preserved. |
| Rules and mechanics | Outline and rule settings retained; no new exclusions or relaxed constraints. |
| BOM and assembly | 142 fitted parts: 135 matched SMT BOM/CPL references and seven manual parts. J19 is fitted manually. |
| Fabrication | Nine Gerber layers, two drill files and job file. ZIP has exactly those twelve native files. Five current PDF drawings. |
| Portability | 132 present 3D links resolve through portable paths; 87 checked original v1/Altium source files remain unchanged. |

Detailed electrical/layout comparisons, native check reports and source hashes are in [PCA control verification](PCA_Control_2026-10-10/README.md) and the release manifest. The previous [input-stage checkpoint](Altium_Features_2026-10-10/README.md) remains historical evidence for Q11/D37/R85. Those parts and the eFuse, converters and Qwiic interface remain in the design.

Seven SMT components were removed and one manual header added, giving six fewer fitted parts than the preceding v1.05. No current cost saving is claimed without a supplier quote including the header and button cable.

Normal output commands remain independent. PCA9685 reset, SLEEP, OE, ALL_LED and frequency changes are shared by pumps and valves. Communication loss intentionally retains the commanded states. The new button cable requires four input GPIOs per button bank and J9 host power. Firmware must be adapted to this interface; the existing DIY sketch is not verified compatible.

The raw native package has a broader mechanical/manual population than the SMT order. Its warnings are retained in the reconciliation report; the delivered BOM/CPL contains exactly the same 135 purchased SMT references. Native coordinate matching does not establish JLCPCB's physical rotation conventions. Obtain a new placement/stencil preview, including the absence of U4 and its removed support parts.

The following electrical notes cover the retained circuits; accepted sensor-reset and valve-voltage risks remain unchanged. No physical board was powered or measured.

## Full-board electrical and layout review

The retained circuit review covers the input fuse, TVS/eFuse, all three bucks, U5 pressure sensor, U7 PWM driver, four MOSFET load stages, switches, address straps and all bus ports. Both copper layers, solder mask, paste and silkscreen were inspected by region. The 109 × 126.4 mm outline is closed. Power/ground paths and local bypass placement were inspected; no gross routing or mask fault was found. This is a geometry review, not thermal, EMC or transient qualification.

U12 retains a short **0.3 mm × 2 mm output power neck**, with another short 0.3 mm input approach. These are candidates for widening and temperature measurement near the current limit, not demonstrated overheating. The nearest buck input-capacitor pads are about 2.32–2.35 mm from their supply pins; U13 bypass placement remains about 2.07/2.22 mm. No exposed switch-node trace openings were found beyond intended component pads. Pump/hose fit and the inherited conservative pump-body/C34 clearance estimate of about 0.22 mm still need physical confirmation.

Fresh netlist calculations give these output setpoints. Ranges include the regulator's full-temperature reference specification and initial resistor tolerance; they exclude resistor temperature drift, ripple and load transients.

| Rail | Nominal | Calculated range | Onboard nominal output capacitance |
| --- | --- | --- | --- |
| U1 pumps | 4.52 V | 4.354–4.691 V | 88.3 µF |
| U2 valves | 6.00 V | **5.775–6.231 V** | 88.3 µF |
| U3 logic/sensors | 3.30 V | 3.205–3.397 V | 64.40 µF |

The three 100 kΩ VIN-to-EN resistors are explicitly permitted by TI; internal EN clamping is described in its datasheet. The eFuse divider gives about 9.36 V turn-on at VIN_RAW. Its 1.65 kΩ setting is approximately 2 A, and the 390 kΩ clamp selection remains below the buck's steady-state input ceiling. These are component specifications and calculations, not measured surge protection. [Buck datasheet](https://www.ti.com/lit/gpn/tps563203), [eFuse datasheet](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

Two nominal 500 mA pumps plus two valves at the specified 276 mA upper current give about 7.83 W of load power. At an illustrative 85% conversion efficiency this is about 0.77 A from 12 V, before logic, accessories and additional input-stage losses. Pump start/stall current is not established by that running-current estimate. The 2 A input protection does not establish a guaranteed total output-current budget. [Pump](https://www.adafruit.com/product/4699), [valve specification](https://cdn-shop.adafruit.com/product-files/4663/4663_C14660_DC_6V.pdf).

## Controller and multiple-board conditions

The user confirmed **SparkFun ESP32 Thing Plus Micro-B WRL-15663**, GPIO23 SDA / GPIO22 SCL, through its [product page](https://www.sparkfun.com/sparkfun-thing-plus-esp32-wroom-micro-b.html). The current page links the same reviewed schematic. The MPR sensor's specified low output is compatible with U13 A; the checked ESP32 satisfies B's stricter input threshold, and B's output-low limit satisfies the ESP32 input requirement. The [interface review](Qwiic_2026-10-09/interface-engineering-review.json) records calculations and primary sources.

Use an external **TCA9548A at 0x71 with exactly one channel enabled** for multiple boards. Supply master 3.3 V, GND and channel SDA/SCL to each J9. Multiple enabled branches would join buffer B sides, which TI prohibits, and cause address collisions. Upstream B-side repeaters and rise-time accelerators are outside the reviewed arrangement.

Keep total selected host-segment pullup current within **3 mA** and selected-branch effective pullups **at least 2.2 kΩ per signal**, including parallel module resistors and tolerances. The branch limit preserves mux ACK low-level margin through its pass resistance. The board's 4.7 kΩ alone satisfies it. Inspect the actual module and measure low levels/rise times with final cables. Start at 100 kHz; maximum buffer rate is 400 kHz. See [assembly and bringup](../ASSEMBLY_AND_BRINGUP.md).

The additional sensor modules are unspecified. Their parallel pullups, addresses, current demand and added capacitance must be included before qualifying the complete system. The 64.40 µF onboard logic-rail total does not grant an unrestricted capacitor budget, and the buck's rating is not a Qwiic cable-current rating.

## Accepted findings and remaining qualification

1. **U5 startup/reset remains unresolved electrically.** RES still connects only U5.9, R22.2 and TP3.1. Honeywell requires a sufficiently fast VDD rise or reset after stable power; the pullup alone does not guarantee it. Earlier prototype-risk acceptance is not measured qualification. See the [existing finding](Recheck_2026-10-09/sensor-startup-finding.json).
2. **Accepted prototype risk — valve-rail voltage margin.** U2's R60 = 270 kΩ / R74 = 30 kΩ divider targets 6.0 V, but specified tolerances permit about 6.231 V before ripple. The exact FA0520E supplier specification lists 5.0–6.0 V operation. Lower the target with tolerance/transient margin (approximately 5.5–5.7 V), or obtain explicit supplier acceptance of the higher voltage. This is a confirmed missing operating margin, not proof the valves will immediately fail. No resistor was changed during review. [Valve specification §3-2](https://cdn-shop.adafruit.com/product-files/4663/4663_C14660_DC_6V.pdf), [calculation](Thorough_2026-10-09/electrical-review.json).
3. Obtain a new JLCPCB placement/stencil preview. Check Q11 pin order, D37 cathode, U12/U13 pin 1, F1, diode cathodes, capacitor polarity and exact substitutions, especially D36. Live availability for every BOM line and final order settings have not been accepted.
4. Test host-only, board-only, both-powered and both power-up orders, leakage/backfeed, bus edges and reconnect behavior. Datasheet powered-off behavior is not a measurement of these boards.
5. Complete rail startup, load-step/PWM, hot-plug, eFuse, temperature, pump/valve and pressure measurements, including narrow power connections and actual accessories. Check the adapter, cables, mechanical fit and hoses. No enclosure, EMC/ESD, thermal simulation or physical testing was performed.

Communication loss intentionally retains commanded outputs while board power stays stable. Pumps remain fixed-direction and intermittent-use. The repository's DIY firmware is not automatically compatible with this PCB.

No board was powered or physically measured during this work. The [JLCPCB v1 top placement](../Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) (order 8815214A_Y73 / SMT026093063736) does not validate v1.05. No manufacturing upload or new order was made.

## Evidence and prior checkpoints

- [Current PCA/button change, independent checks and package reconciliation](PCA_Control_2026-10-10/README.md).
- [Earlier input-stage checkpoint](Altium_Features_2026-10-10/README.md).
- [v1 to v1.05 comparison through the 2026-10-09 checkpoint](V1_TO_V105_COMPARISON.md); the later input-stage and PCA/button deltas are recorded above and in the linked current verification.
- Retained [full-board findings](Thorough_2026-10-09/full-independent-review.json), [electrical calculations](Thorough_2026-10-09/electrical-review.json), and [Qwiic interface review](Qwiic_2026-10-09/interface-engineering-review.json).
- Historical [J18 removal](Remove_J18_2026-10-09/README.md) and [filename-only cleanup](Rename_2026-10-10/validation.json) describe earlier sources, hashes and populations.

Inherited ignored ERC categories remain single_global_label, four_way_junction, simulation_model_issue and footprint_filter. The inherited ignored DRC keys are footprint_filters_mismatch, track_not_centered_on_via and tuning_profile_track_geometries; zero findings does not claim those checks ran. No new instance exclusions or severity relaxations were introduced. All six generic aggregate audits completed; their inherited missing-decoupling heuristic on the jack-to-fuse +12V segment was adjudicated against connectivity, because that segment has no IC supply pin. The raw verdict and reasoning remain in the independent report.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
