# RheoBoard v1.05 revision record

Component spacing, routing, fabrication files, assembly placements and PCB drawings updated on 2026-10-24; BOM population and values retained. Supplier placement acceptance and physical qualification remain open.

The subsequent [full footprint review](FOOTPRINT_REVIEW.md) covers all 160 footprints and 428 pad objects. It identified U1/U2/U3 land-pattern margin, LED manufacturer/KiCad polarity-numbering differences, and remaining exact-part/mechanical acceptance gaps. [Per-component coverage](FOOTPRINT_COVERAGE.csv).

The LED numbering issue is now corrected for all eleven SZYY0603R / C434419 parts. Project-local symbols and two footprint variants use manufacturer pin 1 anode and pin 2 cathode. Physical polarity, pad sizes, component positions, routes and CPL angles are preserved. The native footprint update was followed by reference-layer restoration and teardrop regeneration. All 363 teardrops are valid and connected; fresh ERC and refilled DRC/connectivity/parity checks report zero findings. The BOM, fabrication archive and drawings are refreshed, with a [one-page LED assembly guide](Manufacturing/Drawings/RheoBoard_v1.05_LED_assembly.pdf). Other audit findings remain open.

The maintainer accepted the retained U5 startup/reset and valve-rail voltage-margin risks for this prototype on 2026-10-09. Those circuits are unchanged; their acceptance does not establish measured qualification.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Scope and operating decisions

This revision updates the cloned v1.05 project. The previously submitted v1 design remains preserved. The original assembly workbook is [BOM_RheoboardV1_JLCSMT.xlsx](Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx). The [JLCPCB corrected top placement](Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) is evidence for order 8815214A_Y73 / SMT026093063736 only.

The owner chose to retain the last commanded pump and valve states when master communication stops, retain the current pumps for intermittent use, and operate multiple boards. The working system arrangement uses one board per channel of an external TCA9548A multiplexer. No communication watchdog or reversible motor driver is added. The board continues to generate its own 3.3 V, approximately 4.52 V and 6.0 V rails from 12 V.

## Change register

| Area | v1.05 change | Reason / validation target |
| --- | --- | --- |
| Straight SCL connector run 2026-10-24 | Replace the top-layer SCL detour beside J9 with a straight vertical run from its via and a 45-degree approach into J12 pin 4; retain 0.2 mm width. | Seven segments become three (552 total); 549 unrelated traces unchanged. At least 0.6 mm clearance to other-net pads/vias/tracks. All 1,104 endpoints connected; placements/vias unchanged. Native refill updates teardrops to 363 valid shapes; DRC/connectivity/parity clean. Final Gerber inspected and fabrication archive/copper drawings refreshed. |
| Direct back-layer +3V3 feed 2026-10-24 | Replace the long offset feed with a straight 31.39 mm horizontal run and short controller-end branch; preserve 1.5 mm width and all four supply vias. | This is a necessary bridge from the regulator pour to the controller/distribution rail, not a redundant pour-covered trace. Six segments become four (556 total); 552 unrelated traces identical. Minimum new-route clearance to pads/vias/traces 0.875 mm. Refilled DRC/connectivity/parity clean; placements/vias unchanged and all 362 teardrops valid. Final Gerber inspected; archive and copper drawings refreshed. |
| +3V3 transition spur 2026-10-24 | Remove the 0.3125 mm sideways branch at (95.1875, 73.51) on F.Cu that protruded from the vertical rail. Preserve its 1.25/1.5 mm vertical segments and tapered teardrop transition. | 558 retained traces unchanged; all 1,116 endpoints connected. Placements and vias unchanged; 362 valid teardrops. Refilled DRC/connectivity/parity clean; before/after native-export geometry inspected and fabrication archive/copper drawings refreshed. |
| Straight C33 SDA run 2026-10-24 | Replace the complete back-layer SDA step with one 23.975 mm straight section and 45-degree transitions at its outer ends. Retain 0.2 mm width. | Seven segments become three, 559 total; 556 other traces unchanged. Minimum measured clearance to other-net tracks/pads/vias is 0.25 mm. All 1,118 endpoints connect; 160 placements and 301 vias unchanged, 362 valid teardrops. Refilled DRC/connectivity/parity are clean. Final Gerber inspected; fabrication archive and copper drawings refreshed. |
| Pad coverage and trace endpoints 2026-10-24 | Add 17 local power-polygon extensions at Q11, D11/D12/D16/D18, L1/L2/L5, C2/C16/C29/C59/C66, R44 and R73. Remove five traces fully replaced by the refilled pours. | Improve coverage of 17 pad regions while retaining clearances and compact switching-node copper. All 1,126 remaining trace endpoints meet pads, vias or other traces; 563 segments and 362 valid connected teardrops. Native fill preserves every removed full-width copper path. DRC/connectivity/parity are clean; placements/vias unchanged and fabrication archive/copper drawings refreshed. |
| C33 back-layer SDA detour 2026-10-24 | Replace the short stepped bends behind C33 with two continuous 45-degree transitions around the ground via; retain 0.2 mm trace width. | Nine segments become five, 568 total. All other traces, 160 placements, 301 vias and 367 teardrops unchanged. Refilled DRC/connectivity/parity report zero findings; final Gerber detail inspected and fabrication archive/copper drawings refreshed. |
| Final routing and teardrop review 2026-10-24 | Review both layers again; chamfer remaining simple corners, simplify regulator and valve jogs, straighten the A3 branch, merge collinear runs, and shorten three pour-covered power traces. Regenerate default teardrops. | All three former full-width copper paths remain covered after refill. Retain R85 ground bridge because removing it isolates the pad after refill. 572 tracks, 367 valid teardrop contours; all 160 placements and 301 vias unchanged. Final DRC/connectivity/parity report zero findings. Inspect final Gerber details and refresh fabrication archive and copper drawings. |
| Teardrops and Q11 pour overlap 2026-10-24 | Shorten the 6.3 mm Net-(D1-A) pad-to-tab trace to a 1.15 mm pad entry, retaining 1.5 mm width. Apply native KiCad default teardrops across PTH/SMD pads, vias and track-width transitions. | Refilled pour plus pad entry preserves the complete former copper path. Exported teardrop contours increase from 99 to 367; all 160 placements, pad geometry and 301 vias remain unchanged. 571 other tracks unchanged; 572 total. DRC/connectivity/parity report zero findings. Fabrication archive and copper drawings refreshed. |
| Bend and junction cleanup 2026-10-24 | Chamfer the +4V5 output and +3V3 via-row bends; straighten the HOST_3V3 resistor row, consolidate U13/R84 onto one rail with short branches, give R20 a horizontal SCL T-junction, and simplify the Q4 gate connection. Chamfer two remaining host-rail end bends. | Retain trace widths, all 160 placements and all 301 vias. 34 old segments replaced by 23; 583 → 572 total. Live readback, native pad/via comparison and final refilled DRC/connectivity/parity are clean. Refresh fabrication files and copper drawings. |
| Pour-aware routing cleanup 2026-10-24 | Simplify the +3V3 trunk above J17 and the Q11–R85–D37 gate connection; remove 48 trace segments fully replaced by same-net pours/pads. | After refill, all 48 former full-width copper paths remain covered. R73/R45 share the continuous +4V5 output pour with C53. Preserve all 160 placements, 301 vias and remaining trace widths. 638 → 583 segments; synchronized DRC, connectivity and schematic parity report zero findings. Refresh fabrication files and copper drawings. |
| Whole-board routing review 2026-10-24 | Review both copper layers; straighten valve flyback/power and pump power connections, improve C67 ground and host/sensor joins, merge the +3V3 and A5 overlapping-end runs, remove 58 redundant pad/track-covered segments and two unused connector ground tails. | Preserve existing widths, all 160 component placements and all 301 vias. 692 → 638 trace segments. Retain compact sub-millimetre regulator escapes. Live readback and native export comparison confirm the changes; synchronized/refilled DRC reports zero errors, warnings, unconnected items and schematic-parity findings. Refresh Gerbers, archive and copper PDFs. |
| Component spacing 2026-10-24 | Review all 160 footprints and reposition 55 crowded passives/diodes in local groups. Retain mechanical interfaces and close regulator input/bootstrap placement; update affected traces, refill copper, and refresh the 135-row SMT CPL and PCB drawings. | Use authored courtyards and package-specific assembly guidance, adding about 0.3 mm between courtyards where practical. D33–R46 gap increases from 0.154 to 0.354 mm. All 301 vias retained; 72 original segments replaced by 92 final segments. Final synchronized DRC: zero errors, warnings, unrouted or schematic-parity findings. |
| Routing cleanup 2026-10-24 | Replace slanted C1–C2, C8–C9 and C15–C16 VIN feeds with straight runs and 45° bends; straighten C65 input tap and VIN_RAW junction into C66, remove redundant C66 stub and the isolated-looking GND via tail, and explicitly join the +3V3 output route to its lower supply trunk. | Retain 0.8 mm buck feeds and 1.0 mm input trunk minimum; use 1.5 mm for the added +3V3 connection. R78 signal-pad-to-trunk edge gap increases from 0.325 to 0.525 mm. Footprints, vias and circuit unchanged; synchronized refilled DRC/parity/unrouted all zero. Refresh fabrication files and copper PDFs. |
| Schematic presentation 2026-10-24 | Give U12 input protection a wider, top-aligned column and U13 Qwiic interface a separate padded row; reposition the peripheral, switch and sensor sections on a 950 × 620 mm sheet and refresh the schematic PDF. | Match the existing blue section styling with clear gaps and room for labels. Connectivity unchanged; ERC zero errors and warnings. PCB layout unchanged. |
| Layout refinement 2026-10-22 | Rework three regulator cells/bootstrap paths, U12 power escape/heat spreading, pump bypass, connector-local valve clamps and U5 bypass. Add assigned power-routing classes. | 19 moved footprints; 160 retained identities/values/sides; ERC/DRC/parity zero. Refresh Gerbers, drawings and 135-row SMT CPL. 18 scoped power minima validated; native Gerber viewing complete. Physical/supplier qualification remains open. |
| Pump and valve bulk capacitors | C32, C34, C43 and C45 become Panasonic EEEFK1H220P, 22 µF / 50 V, C128458. | Reduces each rail from about 244 µF to 88.3 µF nominal. Verify startup and load transients on hardware. |
| Input protection | Add U12 TPS259472ARPWR, D36 and R78–R81 / C64–C67 downstream of the reverse-polarity stage and ahead of the converters. | Controlled startup, approximately 2 A current limit and 13.8 V nominal clamp. Explicitly approved by the owner. |
| Input fuse | F1 uses a centered project-local JDT JFC1032TS footprint. | Match the selected JFC1032-1200TS fuse and eliminate the old placement-origin offset. |
| Inductors | L1, L2 and L5 use the FXL0530 footprint. | Match C177246 / FXL0530-4R7-M instead of the old Bourns purchasing link and undersized land pattern. |
| Reverse-polarity stage and diodes | Q11/D37/R85 replace D1 with a 60 V P-MOS stage. D11, D12, D16 and D18 retain Jingdao SS34 / SMA. | Reduce series input loss while retaining the fuse and eFuse; verify polarity, transient behavior and temperature. |
| Qwiic master connection | Make J9 the dedicated four-wire Qwiic MASTER port. Add U13 TCA9517ADGKR, C68/C69 and R82–R84; keep J1/J12/J17 as locally powered Qwiic SENSOR ports. | Preserve standard Qwiic cables while separating the USB/battery-powered ESP32 supply from RheoBoard's regulator. Local bus is on U13 A; master is on B. |
| Local debug connection | Retain the existing J3 four-pin JST EH header. Remove the redundant J18 three-pin service header, its schematic stubs, two dedicated PCB branches and service labels. | J3 already exposes GND, onboard 3.3 V, SDA and SCL. J9 remains the intended Qwiic master/mux connection. |
| Actuator controller and buttons | Remove U4 and its dedicated support parts; route valves to U7 LED5/6. Add J19 GPIO header and move R16–R19 beside it, powered by HOST_3V3. | Reduce fitted count by six while retaining four buttons on a separate ESP32 cable. Keep valves FULL ON/OFF and existing pump PWM. |
| Valve connectors | U9 and U10 identify JST B2B-XH-A headers with 2.50 mm pitch. External valves are separate accessories. | Correct the footprint pitch/drill and include the missing fitted headers in the BOM. |
| Probe pads | TP2–TP9 use project-local 1.00 mm copper / 0.50 mm drill footprints. | Increase radial copper ring from 0.175 mm to 0.25 mm; verified in both copper Gerbers and drill exports. |
| Schematic definitions | Correct TPS GND pin types, power flags, unused outputs, component values and project-local library links. | Resolve electrical-rule errors and make the selected parts reproducible. |
| Schematic grid and symbol cleanup | Align connected blocks to the 1.27 mm connection grid, normalize the local PCA9685 symbol, and replace the two inherited pump symbol instances with consistent project-local definitions. | Resolve all 757 grid and two library-cache warnings while preserving the netlist. Update the two pump-to-PCB identity links and retain their assembly exclusions. |
| Schematic presentation | Correct the driver, switch and pressure-sensor captions; clarify the PCA9685 address annotation and component labels. | Make the saved schematic and PDF easier to read without changing the circuit. |
| Assembly metadata | Separate SMT parts from manually fitted headers, jack and pumps. | Keep manual parts out of the SMT placement file and make purchasing quantities explicit. |
| Manufacturing outputs | Generate a fresh BOM, placement file, Gerbers and Excellon drill files after routing checks. | Prevent the copied v1 outputs from being mistaken for v1.05. |

## Selective Altium features — 2026-10-10

The lower-loss input stage replaces D1 with Q11 DMP6023LE-13 (60 V P-channel MOSFET, SOT-223), D37 BZT52C10-7-F (nominal 10 V gate-source zener) and R85 10 kΩ. Q11 pin 2 and its tab are the drain at the fused input; pin 3 is the source at VIN_RAW. Pin 1 is the gate, pulled to GND by R85. D37's cathode connects to the source and its anode to the gate. F1, the downstream U12 eFuse, D36 TVS, converter values, Qwiic interface and mechanical outline are retained.

The selected manufacturer parts are Q11 C154901, D37 C155227 and R85 C25804. Using the MOSFET's maximum 35 mΩ specification at −4.5 V gate drive and 25 °C gives approximately 71 mV drop and 0.144 W conduction loss at the nominal 2.03 A current limit. Resistance increases with temperature. This estimate does not establish the actual board's thermal limit, and the data-sheet headline current rating is not a board current rating. [Diodes DMP6023LE](https://www.diodes.com/datasheet/download/DMP6023LE.pdf).

D37 is nominally 10 V at 5 mA; its current in this circuit is lower, so do not assume exactly −10 V gate drive or use the MOSFET's lower −10 V on-resistance without measurement. Q11 provides reverse-polarity protection but is not a controlled ideal diode: live polarity reversal or removal can briefly conduct backward. U12's downstream reverse-current blocking is retained. Input hot-plug, reversal, startup and temperature remain physical qualification items. [Diodes BZT52C series](https://www.diodes.com/datasheet/download/BZT52Cxx.pdf), [TI TPS25947](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

The later cost-reduction update consolidates actuator control in U7 PCA9685: LED0/1 remain pump PWM, LED2–4 retain their indicators, and LED5/6 now provide steady valve ON/OFF. It removes U4 TCA9534APWR, C22, C47, R10, R11, R13, R14 and its obsolete TP1. The four switches and their 10 kΩ/100 nF networks remain. J19 adds a manually fitted five-pin GPIO header; R16–R19 now use HOST_3V3 from the master Qwiic port. The user explicitly chose a separate button cable to the ESP32, matching Altium's architecture. The exact [wiring and firmware interface](ASSEMBLY_AND_BRINGUP.md#button-gpio-cable) must be used.

Seven SMT parts are removed and one manual header added: 148 → 142 fitted parts, 142 → 135 SMT placements. This reduces component count; actual savings depend on the new assembly quote and the added cable/header. No current price saving is claimed. Valves still use FULL ON/OFF, with no PWM holding-power qualification. All actuators now share the PCA9685's global controls, so firmware must treat resets and frequency changes accordingly. [NXP PCA9685](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf).


## Input-protection design basis

The input path is J2 → F1 → Q11 drain/tab → Q11 source / VIN_RAW → U12 → protected VIN → the three existing buck converters. D36 is a unidirectional SMBJ13A from VIN_RAW to GND, with its cathode on VIN_RAW. The TVS alone is not the buck overvoltage protection.

U12 uses TI's 10-pad RPW0010A HotRod land pattern. R78/R79 form the 680 kΩ / 100 kΩ undervoltage divider. R80 is 390 kΩ on OVCSEL; R81 is 1.65 kΩ on ILM. C64 is 3.3 nF on DVDT. PG and ITIMER are intentionally unconnected; PGTH is grounded. Local input and output ceramic capacitors must sit next to the corresponding power and ground connections.

C65 is 1 µF / 50 V with C66 = 100 nF on the input. C67 is YAGEO CC1206KKX7R9BB475, 4.7 µF / 50 V / X7R in 1206, directly at the output. Its published typical DC-bias curve retains approximately 2.1 µF at 12 V and 1.8 µF at 14.6 V. A conservative −65% bias allowance combined with −10% initial tolerance and −15% temperature allowance gives approximately 1.26 µF, above TI's local 1 µF recommendation. This uses a typical simulation, not a guaranteed minimum specification. [YAGEO capacitor data](https://www.yageogroup.com/download/specsheet/CC1206KKX7R9BB475).

Calculated nominal settings are approximately 9.36 V turn-on and 8.50 V turn-off at VIN_RAW, 2.03 A current limit, 13.8 V output clamp and 17.7 ms startup at 12 V. Device and component tolerances apply. The input remains a regulated 12 V supply. These settings do not qualify the board for arbitrary adapters, sustained overvoltage or an unspecified surge waveform. [TI TPS25947 datasheet](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

The Littelfuse SMBJ13A procurement code is C151252. The distributor's attribute table conflicts with the named part: require the exact manufacturer part number and the manufacturer's 13 V standoff specification during the assembly review. Do not approve a substitute solely from the distributor's generic attribute table.

## Capacitor calculation

Each of the pump and valve rails has two local 22 µF ceramics, two 22 µF aluminum capacitors and approximately 0.3 µF of small bypass capacitors: 88.3 µF nominal. Applying +20% to the four main capacitors gives approximately 105.9 µF including small bypassing, below the 110 µF recommendation used from TI's design table. That estimate covers positive initial tolerance only; it is not a combined temperature/DC-bias worst case or a measured stability result. More capacitance can affect the converter's compensation and startup, so the 22 µF / 50 V selections are retained. MLCC bias loss, ESR and load behavior still require verification. [TI TPS563203 datasheet](https://www.ti.com/lit/gpn/tps563203), [Panasonic capacitor specification](https://industrial.panasonic.com/ww/products/pt/aluminum-cap-smd/models/EEEFK1H220P).

## Qwiic interface design basis

The intended master is a separately USB/battery-powered ESP32 Thing Plus. All four SH connectors keep Qwiic's pin order: GND, 3.3 V, SDA, SCL. J9 pin 2 feeds HOST_3V3; its SDA/SCL pins feed HOST_SDA/HOST_SCL. These are separate from local +3V3/SDA/SCL. J1, J12 and J17 retain the local nets for sensor expansion. The connector bodies and their board positions are retained. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

U13's physical pins are 1 VCCA/local +3V3, 2 SCLA/local SCL, 3 SDAA/local SDA, 4 common GND, 5 EN/HOST_3V3, 6 SDAB/HOST_SDA, 7 SCLB/HOST_SCL and 8 VCCB/HOST_3V3. C68/C69 provide 100 nF bypassing. R82/R83 provide host-side 4.7 kΩ pullups, and R84 provides 10 kΩ host-rail discharge. Existing local pullups remain. The IC's powered-off high-impedance behavior prevents a powered host from driving the unpowered local bus through U13; both 3.3 V domains share ground and are not galvanically isolated. [TI TCA9517A, SCPS245E](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

The local bus must be on A: the MPR sensor's specified low output can be 0.66 V at 3.3 V, above the B-side 0.45 V contention threshold but below A's 0.99 V input threshold. The specified ESP32's maximum 0.33 V low output fits B's input requirement, and B's 0.60 V maximum output low fits the ESP32's 0.825 V input limit. Keep modest pullup loading and verify the actual harness. The checked board is SparkFun WRL-15663 Micro-B, with Qwiic SDA on GPIO23 and SCL on GPIO22. The interface still requires testing with the actual harness.

Multiple RheoBoards require one channel at a time through a TCA9548A, supplying master 3.3 V to each J9 host interface. Connecting multiple enabled B sides together is prohibited by TI and also causes address collisions. Upstream B-side repeaters and rise-time accelerators are outside the supported arrangement. Keep total selected host-segment pullup current within 3 mA and selected-branch effective pullups at least 2.2 kΩ after parallel combinations and tolerances. The latter preserves the mux ACK low-level margin through its pass-switch resistance; the board's 4.7 kΩ alone is suitable. The [assembly guide](ASSEMBLY_AND_BRINGUP.md) records the wiring, firmware constraints and power-state measurements.

## Prior checkpoint evidence

Before the Qwiic interface addition, saved-board checks reported zero DRC errors, warnings, unconnected items and schematic-parity mismatches. ERC reported zero errors and zero warnings after the grid and library cleanup. All 398 schematic pin nodes were represented on the board. The 141-part fitted BOM and 134-part SMT BOM/CPL pair were reconciled. The eight enlarged probe footprints were applied; all 160 component positions were unchanged from that preceding checkpoint. These counts describe the pre-Qwiic design. Current manufacturing files include the later Qwiic and layout updates.

The six aggregate audits completed. A generic missing-decoupling finding on the connector-to-fuse +12V segment is adjudicated against the exported connectivity in the review report; the raw result is retained. Three inherited rule keys unavailable in the current KiCad UI remain outside demonstrated rule coverage. The schematic title-block revision remains blank. The warning cleanup did not disable rules or add exclusions; the pre-existing ERC exclusions remain documented in the review report.

Physical acceptance remains open: verify fitted parts and polarity, measure the three rails, startup and load switching, input hot plug, temperature, pump startup, valve behavior, I²C operation through the intended harness, and mechanical fit. Record those results using the assembly and bringup guide before releasing repeated production.

## GND via cleanup — 2026-10-22

Removed the five free GND vias below C52/C53 at Y = 84.025 mm,
X = 132.7, 133.4125, 134.125, 134.8375 and 135.55 mm. They occupied the
front +4V5 pour without an attached ground trace. KiCad refilled the copper
and saved the board; the rail no longer has the row of clearance holes.

The board now has 290 vias (199 on GND). All 160 footprints, 594 track
segments, retained vias, zone definitions, graphics and board settings are
unchanged. Only zone fills were recalculated. Fresh DRC, unconnected and
schematic-parity checks report zero findings. The BOM, placement files and
schematic are unaffected. Gerbers, drills, fabrication ZIP and copper/mask
drawings were refreshed.

### Component spacing basis

Spacing was reviewed against [KiCad courtyard conventions](https://klc.kicad.org/footprint/f5/f5.3/) and [JLCPCB package-specific SMD spacing guidance](https://jlcpcb.com/help/article/minimum-spacing-for-smd-components), retrieved 2026-10-24. The extra approximately 0.3 mm courtyard-to-courtyard target is a layout preference for access and readability, not a universal assembly minimum. Courtyards already include component/pad clearance allowances. Close regulator input/inductor and bootstrap relationships remain compact; the pump interfaces and nearby bulk capacitors retain their established mechanical locations. Copper clearances and routing widths remain governed by the existing project rules. Supplier acceptance and physical fit remain open.


## U5 pressure-sensor verification — 2026-10-24

U5 is Honeywell MPRLS0025PA00001A, the same part identified by [SparkFun SEN-16476](https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html). The screenshot submitted for this check depicts U12 (TPS259472ARPWR input eFuse), not U5.

Compared the live U5 footprint and exported connectivity with [Honeywell datasheet Issue K](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf), Figures 5/10 and Table 13, and [SparkFun’s schematic](https://cdn.sparkfun.com/assets/8/7/2/1/f/SparkFun_MicroPressure_Sensor_Schematic.pdf):

- All 12 lands match: 0.70 × 0.65 mm, 1.27 mm pitch, 4.20 mm opposing row spacing. Pin order is correct after rotation, without mirroring; pin-1 mark is present. The absolute-pressure variant does not require the gage-reference vent hole.
- Pin 12 is +3V3; pin 10 ground; pin 2 SDA; pin 3 SCL; C27 is 1 nF between pins 4/6. Pins 1/7 are unused for I²C and pads 5/11 remain unconnected. EOC reaches TP2; RES reaches R22/TP3.
- C46 is a local 100 nF bypass, about 1.97 mm from the supply pad center; C28 supplies local bulk capacitance. Short filter routes reach C27 beside U5. Ground pad has continuous same-net pour contact. SDA/SCL traces are 0.2 mm with short pad escapes and tented vias; native copper views and exported mask/paste openings inspected.
- Local bus pull-ups are 4.7 kΩ; SparkFun uses 2.2 kΩ. This difference is intentional in the shared bus design; cable/module loading still needs the existing bring-up checks.
- Native position and JLC placement row agree on U5 at X145.67/Y−84.90, top, 0°. Supplier preview must still confirm actual part pin-1 orientation.

Fresh full-severity ERC and synchronized/refilled DRC/connectivity/parity report zero findings. No footprint, placement, or routing edits were needed. The previously accepted startup limitation remains: Honeywell requires at least 10 V/ms supply rise or an explicit reset sequence; R22 and TP3 alone do not generate that sequence. This review verifies design data, not measured startup or pressure performance.


### +4V5 output-tail cleanup — 2026-10-24

The +4V5 cleanup removes the three-segment dangling tail from L1 pad 2. After refill, the +4V5 polygon geometry is unchanged and fully covers the output pad, continuously connecting L1 to C52/C53. All other 549 traces, 160 component placements and 301 vias remain unchanged; all 363 teardrops are valid and connected. Full synchronized/refilled DRC reports zero errors, warnings, unconnected items and parity findings. The inspected Gerbers, exact twelve-file fabrication archive and copper PDFs include this correction.


## Aligned GND stitching triples and export review — 2026-10-24

Organized 30 isolated stitching locations into straight triples by retaining the existing center via and adding two vias at each location. Twenty-eight groups use 1.0 mm pitch; two constrained locations use 0.8 mm pitch. All additions are GND, 0.50 mm copper diameter and 0.30 mm drill. Existing trace-following fences and component connection vias remain intact. Native-editor removal was authorized, but unreliable canvas targeting was avoided by preserving the centers and using verified Konnect additions.

After native refill/save, exported comparison confirms exactly 60 additions: 361 total vias, all 301 originals unchanged, all 549 traces unchanged, all 160 placements and 1,274 pad-layer records unchanged. Each new land has at least 0.10 mm margin within the connected GND fill on both layers. All 1,098 track endpoints contact same-net pad/via/track copper; all 363 teardrop contours remain valid and connected. Full info-level synchronized DRC reports zero errors, warnings, unconnected items and schematic-parity findings. Group count follows the requested visual organization; it is not a validated EMI or current-capacity target.

The refreshed fabrication archive matches the twelve Gerber/drill/job exports exactly. Copper drawings are refreshed. Independent exported-file review verifies outline, copper-to-edge, paste containment, netlist and assembly-file consistency, but identifies unresolved U5/U7 mask webs and silk production margins alongside the existing U1/U2/U3 land-pattern issue. See [footprint and export review](FOOTPRINT_REVIEW.md). Release remains blocked; no fabrication order or upload was made.


### 2026-10-24 — regulator lands, mask bridges and silkscreen corrections

Corrected U1/U2/U3 to TI DRL0006A lands and U5/U7 mask expansion to 0.05 mm in both project libraries and placed footprints. Widened thin silkscreen, adjusted outlines for pad clearance and moved two reference labels. Actual Gerber minimum mask webs are 0.1192 mm (U5) and 0.1794 mm (U7); legend strokes are at least 0.15 mm, with at least 0.17 mm to mask openings. Regulator paste follows the new copper; all other copper pads, placements, nets, 549 traces and 361 vias are preserved. Teardrops regenerated and copper refilled. Final DRC: zero errors/unconnected/parity, 23 intentional board-specific silkscreen/library mismatch warnings. No warning exclusions added. Fabrication ZIP and relevant drawings refreshed. Exact-part/supplier/physical acceptance remains open as recorded in FOOTPRINT_REVIEW.md; this is not fabrication release approval.


### 2026-10-24 — Qwiic connectors moved toward board edges

All four Qwiic sockets now have a nominal 0.75 mm mating-face setback from the board edge. J1/J17 moved outward 5.475 mm to X92.525; J9/J12 moved outward 2.950 mm to X194.875. Y positions, rotations, pad geometry and the SparkFun pin order (1 GND, 2 3.3 V, 3 SDA, 4 SCL) are unchanged; J9 retains its separate host domain. Local routes, four signal vias and 22 labels were adjusted, one SDA via added, copper refilled and teardrops regenerated. Existing polygon boundaries already cover the moved pads, so no polygon extensions were needed.

The saved board has 546 track segments, 362 vias and 365 valid connected teardrops. All 1,092 trace endpoints contact same-net pads, vias or tracks. Final synchronized/refilled DRC reports zero errors, unconnected items or schematic-parity findings, with the same 23 intentional board-local silkscreen/library warnings. Independent Gerber review verifies 0.550 mm connector copper-to-edge clearance, 0.185 mm connector silk-to-mask clearance and unchanged whole-board minimum silk-to-mask clearance of 0.170078 mm. All 368 top paste objects remain contained in copper and mask; all other 156 component placements and pad records are unchanged.

Gerbers, plated/non-plated drills, the exact twelve-file fabrication ZIP, native positions, 135-row JLCPCB CPL, assembly, copper and mask drawings are refreshed. Only the four connector X coordinates change in the CPL; BOM population and rotations are unchanged. Supplier placement preview and physical cable/enclosure fit remain open, along with the previously documented exact-part and mechanical acceptance items.


### 2026-10-24 — aligned address-area GND vias

Aligned the nine scattered GND stitching vias shown beside A0–A3 into three horizontal rows of three, with 1.00 mm pitch and common X coordinates 187/188/189 mm. Row Y coordinates are 64.5, 72.5 and 81.5 mm. Existing 0.50 mm lands, 0.30 mm drills, nets and total via count (362) are retained. All 160 placements, pads, 546 trace segments and 365 teardrops are unchanged. Both-layer GND fills cover every moved land with at least 0.10 mm margin after refill. Final DRC has zero errors, unconnected items or schematic-parity findings; the same 23 documented library/artwork warnings remain.

Actual Gerbers and drills were inspected: exactly nine plated-hole positions changed; all other hole records are unchanged. Gerbers, drill files, the verified twelve-file fabrication ZIP and front/back copper drawings are refreshed. BOM/CPL and assembly positions are unchanged. This is a local organization correction, not a new EMI or physical qualification result; existing supplier/physical acceptance items remain open.


### 2026-10-24 — approved-project output refresh

After the user’s explicit “pass,” refreshed the fabrication package from the saved, refilled v1.05 project. The final artwork omits all four Qwiic role captions and sixteen Qwiic pin captions, includes the 0.75 mm connector mating-face setbacks, and retains the nine GND vias aligned into three 1 mm-pitch rows. All 160 components, 546 tracks, 362 vias and 365 teardrops match the approved copper snapshot.

Final ERC has zero findings. DRC reports zero errors, unconnected items and schematic-parity findings, with the same 23 documented board-local silkscreen/library warnings. Independent export comparison confirms unchanged copper/drills/pads, only the intended silkscreen removals, 368 contained paste apertures and 0.170078 mm minimum legend-to-mask clearance. All 394 schematic nodes reconcile; all 135 SMT BOM/CPL references agree with fresh native/IPC placements. Supplier-specific rotation acceptance remains a placement-preview check.

The twelve-file Gerber/drill/job ZIP, native positions, JLCPCB placement file, assembly, copper, solder-mask and schematic PDFs are refreshed. BOM population and the LED polarity guide remain unchanged and applicable. Validation reports and SHA-256 output/source hashes are in Manufacturing/Validation. This “pass” authorizes output refresh; no upload or order was placed, and the previously documented exact-part, supplier and physical acceptance items remain open.
