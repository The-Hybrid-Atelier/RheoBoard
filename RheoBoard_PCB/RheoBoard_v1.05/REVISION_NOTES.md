# RheoBoard v1.05 revision record

CAD revision saved and fabrication/assembly exports reconciled on 2026-10-09. Supplier placement acceptance and physical qualification remain open. See the [review report](Verification/REVIEW_REPORT.md).

The subsequent full recheck identified an unresolved U5 startup/reset risk. No circuit change has yet been made for this finding; production approval remains pending its resolution.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Scope and operating decisions

This revision updates the cloned v1.05 project. The previously submitted v1 design remains preserved. The original assembly workbook is [BOM_RheoboardV1_JLCSMT.xlsx](Archive_v1_outputs/BOM/BOM_RheoboardV1_JLCSMT.xlsx). The [JLCPCB corrected top placement](Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) is evidence for order 8815214A_Y73 / SMT026093063736 only.

The owner chose to retain the last commanded pump and valve states when master communication stops, retain the current pumps for intermittent use, and operate multiple boards. The working system arrangement uses one board per channel of an external TCA9548A multiplexer. No communication watchdog or reversible motor driver is added. The board continues to generate its own 3.3 V, approximately 4.52 V and 6.0 V rails from 12 V.

## Change register

| Area | v1.05 change | Reason / validation target |
| --- | --- | --- |
| Pump and valve bulk capacitors | C32, C34, C43 and C45 become Panasonic EEEFK1H220P, 22 µF / 50 V, C128458. | Reduces each rail from about 244 µF to 88.3 µF nominal. Verify startup and load transients on hardware. |
| Input protection | Add U12 TPS259472ARPWR, D36 and R78–R81 / C64–C67 between D1 and the converters. | Controlled startup, approximately 2 A current limit and 13.8 V nominal clamp. Explicitly approved by the owner. |
| Input fuse | F1 uses a centered project-local JDT JFC1032TS footprint. | Match the selected JFC1032-1200TS fuse and eliminate the old placement-origin offset. |
| Inductors | L1, L2 and L5 use the FXL0530 footprint. | Match C177246 / FXL0530-4R7-M instead of the old Bourns purchasing link and undersized land pattern. |
| Diodes | D1 uses B540C-13-F / SMC; D11, D12, D16 and D18 use Jingdao SS34 / SMA. | Align the value, package, procurement code and cathode marks. |
| Qwiic master connection | Make J9 the dedicated four-wire Qwiic MASTER port. Add U13 TCA9517ADGKR, C68/C69 and R82–R84; keep J1/J12/J17 as locally powered Qwiic SENSOR ports. | Preserve standard Qwiic cables while separating the USB/battery-powered ESP32 supply from RheoBoard's regulator. Local bus is on U13 A; master is on B. |
| Local debug connection | Retain the existing J3 four-pin JST EH header. Remove the redundant J18 three-pin service header, its schematic stubs, two dedicated PCB branches and service labels. | J3 already exposes GND, onboard 3.3 V, SDA and SCL. J9 remains the intended Qwiic master/mux connection. |
| Valve connectors | U9 and U10 identify JST B2B-XH-A headers with 2.50 mm pitch. External valves are separate accessories. | Correct the footprint pitch/drill and include the missing fitted headers in the BOM. |
| Probe pads | TP2–TP9 use project-local 1.00 mm copper / 0.50 mm drill footprints. | Increase radial copper ring from 0.175 mm to 0.25 mm; verified in both copper Gerbers and drill exports. |
| Schematic definitions | Correct TPS GND pin types, power flags, unused outputs, component values and project-local library links. | Resolve electrical-rule errors and make the selected parts reproducible. |
| Schematic grid and symbol cleanup | Align connected blocks to the 1.27 mm connection grid, normalize the local PCA9685 symbol, and replace the two inherited pump symbol instances with consistent project-local definitions. | Resolve all 757 grid and two library-cache warnings while preserving the netlist. Update the two pump-to-PCB identity links and retain their assembly exclusions. |
| Schematic presentation | Correct the driver, switch and pressure-sensor captions; clarify the PCA9685 address annotation and component labels. | Make the saved schematic and PDF easier to read without changing the circuit. |
| Assembly metadata | Separate SMT parts from manually fitted headers, jack and pumps. | Keep manual parts out of the SMT placement file and make purchasing quantities explicit. |
| Manufacturing outputs | Generate a fresh BOM, placement file, Gerbers and Excellon drill files after routing checks. | Prevent the copied v1 outputs from being mistaken for v1.05. |

## Input-protection design basis

The input path is J2 → F1 → D1 → VIN_RAW → U12 → protected VIN → the three existing buck converters. D36 is a unidirectional SMBJ13A from VIN_RAW to GND, with its cathode on VIN_RAW. The TVS alone is not the buck overvoltage protection.

U12 uses TI's 10-pad RPW0010A HotRod land pattern. R78/R79 form the 680 kΩ / 100 kΩ undervoltage divider. R80 is 390 kΩ on OVCSEL; R81 is 1.65 kΩ on ILM. C64 is 3.3 nF on DVDT. PG and ITIMER are intentionally unconnected; PGTH is grounded. Local input and output ceramic capacitors must sit next to the corresponding power and ground connections.

C65 is 1 µF / 50 V with C66 = 100 nF on the input. C67 is YAGEO CC1206KKX7R9BB475, 4.7 µF / 50 V / X7R in 1206, directly at the output. Its published typical DC-bias curve retains approximately 2.1 µF at 12 V and 1.8 µF at 14.6 V. A conservative −65% bias allowance combined with −10% initial tolerance and −15% temperature allowance gives approximately 1.26 µF, above TI's local 1 µF recommendation. This uses a typical simulation, not a guaranteed minimum specification. [YAGEO capacitor data](https://www.yageogroup.com/download/specsheet/CC1206KKX7R9BB475).

Calculated nominal settings are approximately 9.36 V turn-on and 8.50 V turn-off at VIN_RAW, 2.03 A current limit, 13.8 V output clamp and 17.7 ms startup at 12 V. Device and component tolerances apply. The input remains a regulated 12 V supply. These settings do not qualify the board for arbitrary adapters, sustained overvoltage or an unspecified surge waveform. [TI TPS25947 datasheet](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

The Littelfuse SMBJ13A procurement code is C151252. The distributor's attribute table conflicts with the named part: require the exact manufacturer part number and the manufacturer's 13 V standoff specification during the assembly review. Do not approve a substitute solely from the distributor's generic attribute table.

## Capacitor calculation

Each of the pump and valve rails has two local 22 µF ceramics, two 22 µF aluminum capacitors and approximately 0.3 µF of small bypass capacitors: 88.3 µF nominal. Applying +20% to the four main capacitors gives approximately 105.9 µF including small bypassing, below the 110 µF recommendation used from TI's design table. That estimate covers positive initial tolerance only; it is not a combined temperature/DC-bias worst case or a measured stability result. More capacitance can affect the converter's compensation and startup, so the 22 µF / 50 V selections are retained. MLCC bias loss, ESR and load behavior still require verification. [TI TPS563203 datasheet](https://www.ti.com/lit/gpn/tps563203), [Panasonic capacitor specification](https://industrial.panasonic.com/ww/products/pt/aluminum-cap-smd/models/EEEFK1H220P).

## Qwiic interface design basis

The intended master is a separately USB/battery-powered ESP32 Thing Plus. All four SH connectors keep Qwiic's pin order: GND, 3.3 V, SDA, SCL. J9 pin 2 feeds HOST_3V3; its SDA/SCL pins feed HOST_SDA/HOST_SCL. These are separate from local +3V3/SDA/SCL. J1, J12 and J17 retain the local nets for sensor expansion. The connector bodies and their board positions are retained. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

U13's physical pins are 1 VCCA/local +3V3, 2 SCLA/local SCL, 3 SDAA/local SDA, 4 common GND, 5 EN/HOST_3V3, 6 SDAB/HOST_SDA, 7 SCLB/HOST_SCL and 8 VCCB/HOST_3V3. C68/C69 provide 100 nF bypassing. R82/R83 provide host-side 4.7 kΩ pullups, and R84 provides 10 kΩ host-rail discharge. Existing local pullups remain. The IC's powered-off high-impedance behavior prevents a powered host from driving the unpowered local bus through U13; both 3.3 V domains share ground and are not galvanically isolated. [TI TCA9517A, SCPS245E](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

The local bus must be on A: the MPR sensor's specified low output can be 0.66 V at 3.3 V, above the B-side 0.45 V contention threshold but below A's 0.99 V input threshold. The specified ESP32's maximum 0.33 V low output fits B's input requirement, and B's 0.60 V maximum output low fits the ESP32's 0.825 V input limit. Keep modest pullup loading and verify the actual harness. The checked board is SparkFun WRL-15663 Micro-B, with Qwiic SDA on GPIO23 and SCL on GPIO22. The full source-based interface review is included with the current verification evidence.

Multiple RheoBoards require one channel at a time through a TCA9548A, supplying master 3.3 V to each J9 host interface. Connecting multiple enabled B sides together is prohibited by TI and also causes address collisions. Upstream B-side repeaters and rise-time accelerators are outside the supported arrangement. Keep total selected host-segment pullup current within 3 mA and selected-branch effective pullups at least 2.2 kΩ after parallel combinations and tolerances. The latter preserves the mux ACK low-level margin through its pass-switch resistance; the board's 4.7 kΩ alone is suitable. The [assembly guide](ASSEMBLY_AND_BRINGUP.md) records the wiring, firmware constraints and power-state measurements.

## Prior checkpoint evidence

Before the Qwiic interface addition, saved-board checks reported zero DRC errors, warnings, unconnected items and schematic-parity mismatches. ERC reported zero errors and zero warnings after the grid and library cleanup. All 398 schematic pin nodes were represented on the board. The 141-part fitted BOM and 134-part SMT BOM/CPL pair were reconciled. The eight enlarged probe footprints were applied; all 160 component positions were unchanged from that preceding checkpoint. These counts describe the pre-Qwiic design. The current [review report](Verification/REVIEW_REPORT.md) and release manifest govern the updated files.

The six aggregate audits completed. A generic missing-decoupling finding on the connector-to-fuse +12V segment is adjudicated against the exported connectivity in the review report; the raw result is retained. Three inherited rule keys unavailable in the current KiCad UI remain outside demonstrated rule coverage. The schematic title-block revision remains blank. The warning cleanup did not disable rules or add exclusions; the pre-existing ERC exclusions remain documented in the review report.

Physical acceptance remains open: verify fitted parts and polarity, measure the three rails, startup and load switching, input hot plug, temperature, pump startup, valve behavior, I²C operation through the intended harness, and mechanical fit. Record those results using the assembly and bringup guide before releasing repeated production.
