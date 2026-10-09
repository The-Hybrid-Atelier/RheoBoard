# RheoBoard v1.05 revision record

CAD revision saved and fabrication/assembly exports reconciled on 2026-10-09. Supplier placement acceptance and physical qualification remain open. See the [review report](Verification/REVIEW_REPORT.md).

The subsequent full recheck identified an unresolved U5 startup/reset risk. No circuit change has yet been made for this finding; production approval remains pending its resolution.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Scope and operating decisions

This revision updates the cloned v1.05 project. The previously submitted v1 design and original BOM remain preserved. The old JLCPCB corrected placement image is evidence for that order only.

The owner chose to retain the last commanded pump and valve states when master communication stops, retain the current pumps for intermittent use, and operate multiple boards. The working system arrangement uses one board per channel of an external TCA9548A multiplexer. No communication watchdog or reversible motor driver is added. The board continues to generate its own 3.3 V, approximately 4.52 V and 6.0 V rails from 12 V.

## Change register

| Area | v1.05 change | Reason / validation target |
| --- | --- | --- |
| Pump and valve bulk capacitors | C32, C34, C43 and C45 become Panasonic EEEFK1H220P, 22 µF / 50 V, C128458. | Reduces each rail from about 244 µF to 88.3 µF nominal. Verify startup and load transients on hardware. |
| Input protection | Add U12 TPS259472ARPWR, D36 and R78–R81 / C64–C67 between D1 and the converters. | Controlled startup, approximately 2 A current limit and 13.8 V nominal clamp. Explicitly approved by the owner. |
| Input fuse | F1 uses a centered project-local JDT JFC1032TS footprint. | Match the selected JFC1032-1200TS fuse and eliminate the old placement-origin offset. |
| Inductors | L1, L2 and L5 use the FXL0530 footprint. | Match C177246 / FXL0530-4R7-M instead of the old Bourns purchasing link and undersized land pattern. |
| Diodes | D1 uses B540C-13-F / SMC; D11, D12, D16 and D18 use Jingdao SS34 / SMA. | Align the value, package, procurement code and cathode marks. |
| Master connection | Add J18, a JST EH 3-pin header: GND, SDA, SCL. | Connect a separately powered master or mux without connecting two regulated 3.3 V outputs. |
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

Each of the pump and valve rails has two local 22 µF ceramics, two 22 µF aluminum capacitors and approximately 0.3 µF of small bypass capacitors: 88.3 µF nominal. Applying +20% to the four main capacitors gives approximately 105.9 µF including small bypassing, below the 110 µF recommendation used from TI's design table. This is a nominal-capacitance comparison, not a measured stability result. MLCC bias loss, ESR and load behavior still require verification. [TI TPS563203 datasheet](https://www.ti.com/lit/gpn/tps563203), [Panasonic capacitor specification](https://industrial.panasonic.com/ww/products/pt/aluminum-cap-smd/models/EEEFK1H220P).

## Release evidence

Saved-board checks report zero DRC errors, warnings, unconnected items and schematic-parity mismatches. ERC reports zero errors and zero warnings after the grid and library cleanup. All 398 schematic pin nodes are represented on the board. The 141-part fitted BOM and 134-part SMT BOM/CPL pair are reconciled. The eight enlarged probe footprints are applied; all 160 component positions remain unchanged from the preceding revision checkpoint.

The six aggregate audits completed. A generic missing-decoupling finding on the connector-to-fuse +12V segment is adjudicated against the exported connectivity in the review report; the raw result is retained. Three inherited rule keys unavailable in the current KiCad UI remain outside demonstrated rule coverage. The schematic title-block revision remains blank. The warning cleanup did not disable rules or add exclusions; the pre-existing ERC exclusions remain documented in the review report.

Physical acceptance remains open: verify fitted parts and polarity, measure the three rails, startup and load switching, input hot plug, temperature, pump startup, valve behavior, I²C operation through the intended harness, and mechanical fit. Record those results using the assembly and bringup guide before releasing repeated production.
