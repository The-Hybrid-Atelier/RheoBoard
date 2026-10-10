# RheoBoard v1 versus v1.05 — comparison checkpoint of 2026-10-09

This is a historical checkpoint. The later [PCA/button update](PCA_Control_2026-10-10/README.md) changes actuator control and adds J19; use the current review for release status.
**This detailed comparison records the 2026-10-09 design.** On 2026-10-10, D1 was subsequently replaced by Q11 DMP6023LE-13, D37 BZT52C10-7-F and R85 10 kΩ. Current totals are 167 components/features, 418 schematic pin nodes, 148 fitted parts and 142 SMT parts. The new input stage adds local routing and one ground via while preserving all 164 retained components and their connections, placement and pad geometry. See the [current review](REVIEW_REPORT.md) and [selective change verification](Altium_Features_2026-10-10/README.md). The tables below, including D1 and the 165-component/611-trace counts, remain historical evidence for the preceding checkpoint.


The saved v1.05 design adds input protection, separates the Qwiic master interface from the local sensor supply, and corrects several footprints and part specifications. The pump, valve, pressure-sensor and controller functions are retained. The board outline remains 109 × 126.4 mm with two copper layers.

This comparison is between the saved KiCad projects on 2026-10-09. The previously assembled v1 may already contain JLCPCB component substitutions that differ from its schematic labels. A changed CAD value or purchasing label does not by itself establish which part JLCPCB fitted to v1.

Updated after the authorized J18 removal on 2026-10-09. The original v1 remains unchanged. Current v1.05 exports are compared with the preserved native v1 exports; the removal check also verifies that all remaining v1.05 component values, placements and pin nets are unchanged. The pressure-sensor startup/reset and valve voltage-margin findings remain accepted prototype risks, as instructed by the maintainer.

## Schematic changes by circuit

| Item | v1 | v1.05 | Effect |
| --- | --- | --- | --- |
| Input path | J2 → F1 → D1 → VIN → converters | J2 → F1 → D1 → VIN_RAW → U12 → VIN → converters | Adds protection ahead of all three converters. |
| U12 and supporting parts | Absent | TPS259472ARPWR; R78–R81 and C64–C67 | Approximately 2 A current limit, 13.8 V nominal clamp, undervoltage control and controlled startup. These are design settings, not measurements. |
| D36 | Absent | SMBJ13A from VIN_RAW to ground | Adds input surge suppression. |
| Pump capacitors C32/C34 | 100 µF / 16 V each | 22 µF / 50 V each | Pump-rail nominal capacitance falls from 244.3 µF to 88.3 µF. |
| Valve capacitors C43/C45 | 100 µF / 16 V each | 22 µF / 50 V each | Valve-rail nominal capacitance falls from 244.3 µF to 88.3 µF. |
| R45 pump feedback | Schematic label 197 kΩ ±1% | 196 kΩ ±1% | Label-based nominal setpoint changes from 4.54 V to 4.52 V. A v1 purchasing substitution may already have used 196 kΩ. |
| R61 logic feedback | 135 kΩ ±1% label | 135 kΩ ±0.1% | Nominal 3.3 V unchanged; tighter specified tolerance. |
| R73 indicator resistor | 550 Ω label | 549 Ω ±1% | Aligns the schematic to the selected standard value. |
| Valve feedback R60/R74 | 270 kΩ / 30 kΩ | Same ratio | Nominal 6.0 V remains unchanged. |
| J9 master port | Shares local +3V3, SDA and SCL directly | HOST_3V3, HOST_SDA and HOST_SCL through U13 | Separates the ESP32 supply from the board supply while retaining a standard four-wire Qwiic connector. Ground stays common. |
| U13 and supporting parts | Absent | TCA9517ADGKR, C68/C69 100 nF, R82/R83 4.7 kΩ, R84 10 kΩ | Adds the host/local I²C interface, bypassing, host pullups and host-rail discharge. Local bus is A; host is B. |
| J1/J12/J17 sensor ports | Local 3.3 V, SDA, SCL and GND | Same connections | Continue powering local Qwiic sensor modules. |
| J3 four-pin EH port | Local bus and 3.3 V | Same connections | Retained local bus connection; this larger connector is not an SH Qwiic socket. |
| J18 service header | Absent | Removed from the intermediate v1.05 design | Existing J3 already provides local debug access. |
| D1 | SS54 schematic value/custom footprint | B540C-13-F / SMC | Aligns the CAD definition with the selected diode. Its cathode now feeds VIN_RAW. |
| F1 | Generic 2 A label | 2 A time-delay JFC1032-1200TS | Makes the selected fuse explicit. |
| L1/L2/L5 | 4.7 µH / 4 A generic label | 4.7 µH FXL0530-4R7-M | Inductance unchanged; specific purchasing identity and footprint corrected. |
| U9/U10 | VALVE and VALVE2 symbols/labels | JST XH two-pin 2.50 mm headers | Represents the actual fitted connectors; external valves are accessories. Electrical connections are retained. |
| U8/U11 pumps | Generic PUMP symbols | ZR370-02PM / 4.5 V / 2.5 LPM / approximately 500 mA | Identifies the existing pumps and normalizes symbols. No direction-control circuit is added. |
| U1/U2/U3 symbol definitions | VIN=input; SW=output; GND=power_out; BST=output | VIN=power_in; SW=power_out; GND=power_in; BST=passive | Corrects ERC pin classifications; no corresponding pin is rewired. |
| Other passive labels | Several generic or inaccurate voltage/tolerance labels | Selected capacitor dielectric, voltage and tolerance; normalized resistor labels | Mainly specification and purchasing cleanup. Exact before/after values are listed below. |
| Schematic presentation | Misleading driver/sensor/switch captions and inherited grid/cache warnings | Correct captions, grid alignment, consistent pump/PCA9685 definitions and clearer address notes | Improves readability and library/ERC consistency. The earlier 757 grid plus two pump-cache warnings were removed during v1.05 cleanup. |

All 367 original schematic pin identities remain. Only four existing pins change net names: **D1 pin 1** from VIN to VIN_RAW, and **J9 pins 2/3/4** from the local power/data nets to their HOST equivalents. Added components extend existing nets as expected. No other existing-pin connectivity group changes occur outside these power/master-interface splits.

## PCB changes by area

| Area | v1 | v1.05 |
| --- | --- | --- |
| Outline and layer count | 109 × 126.4 mm, two copper layers | Identical outline and layer count. |
| Main arrangement | Input/converters at top, controls near centre, pumps and valve connectors below | Same arrangement; 140 of 149 existing footprint origins are unchanged. Eight parts/testpoints move and F1 gets a corrected origin. |
| New input protection | No U12 cluster | U12/D36/R78–R81/C64–C67 near the input; VIN_RAW/protected VIN routed separately. |
| New host interface | J9 tied directly into local bus | U13/C68/C69/R82–R84 near the right-side master connector, with separate host routing. |
| Service header | No J18 | No J18; its three holes, two dedicated bus branches and service labels were removed. J3 is retained. |
| F1 lands and origin | Off-centre origin; two 5.00 × 3.81 mm lands with 7.18 mm centre spacing | Centred origin; 3.40 × 3.43 mm lands with 9.20 mm spacing. The pad-pair midpoint stays at (123.40, 48.01) mm. SMT classification corrected. |
| L1/L2/L5 lands | Custom pattern with 1.8 × 2.0 mm lands | FXL0530 pattern with 1.9 × 2.5 mm lands and corrected spacing/orientation. Component origins unchanged. |
| D1 footprint | Custom SS54 lands | Standard SMC pattern with cathode marking. Component origin unchanged. |
| D11/D12/D16/D18 footprints | Custom SS34 lands | Standard SMA patterns and polarity markings. Component origins unchanged. |
| C32/C34/C43/C45 footprints | CP_Elec_6.3x5.4_Nichicon | CP_Elec_6.3x5.8. Copper pad sizes are retained; C32 moves by 1.00 mm, while the other three centres remain fixed. |
| U9/U10 connector holes | 2.54 mm pitch, 0.8636 mm drills | 2.50 mm pitch, 1.00 mm drills; existing footprint origins retained. |
| TP1 | 0.50 mm copper disc despite D1.0 footprint name | 1.00 mm copper disc, moved to clear surrounding circuitry; short interrupt routing added. |
| TP2–TP9 | 0.85 mm square copper pads, 0.50 mm drills | 1.00 mm circular copper pads, same drills/locations; minimum radial ring grows from 0.175 to 0.25 mm. |
| SW1–SW4 | Generic switch footprint link | Project-local four-pad definition. Existing copper pad geometry and placement retained. |
| Existing Qwiic bodies | Four SH sockets | Same positions, orientation, pin order and pad geometry. J9 has different net assignments and MASTER labeling; other SH sockets are SENSOR ports. |
| Pump footprint locations | U8/U11 positions and 4 mm terminal holes | Retained. A 0.000001 mm native-export coordinate difference is treated as numerical normalization. Assembly category changes from SMT to through-hole. |
| Mounting holes | H1–H4 | Same centres, copper and drills. Other unchanged mechanical geometry is preserved in the exported profile/pads. |
| Routing | 527 exported copper line/arc primitives; 281 vias | 611 primitives; 301 vias. Added routing serves input protection, U13 and the moved TP1; host routing is split from local routing. |
| Filled copper | Original filled contours and teardrops | Recomputed around revised pads and routing. Contour changes include teardrops, so they must not be interpreted as entire zones being deleted. |
| Silkscreen/artwork | Rheoboard V1, generic connector/part labels | RheoBoard v1.05, MASTER/SENSOR labels and corrected footprint artwork/polarity identification. |
| Library portability | Khach Footprint/Sensor_Pressure dependencies | Standard or project-local RheoBoard library links. A library-name change does not necessarily change the physical pads. |

## Exact placement changes

Coordinates below are KiCad board millimetres, with positive Y downward. All inherited component rotations and board sides are retained. F1's origin correction is shown separately from the eight physical placement changes.

| Reference | v1 centre/origin | v1.05 centre/origin | Distance or interpretation |
| --- | --- | --- | --- |
| C29 | (161.9267, 67.6194) | (162.1500, 67.6194) | 0.223 mm |
| C30 | (160.4420, 67.6220) | (160.6500, 67.6220) | 0.208 mm |
| C32 | (178.4000, 105.2000) | (179.2000, 104.6000) | 1.000 mm |
| C33 | (146.2278, 109.8042) | (146.8000, 109.5000) | 0.648 mm |
| C46 | (143.4875, 79.7040) | (143.4875, 79.4500) | 0.254 mm |
| D15 | (161.9660, 71.1780) | (162.1500, 71.1780) | 0.184 mm |
| F1 | (125.3000, 53.3700) | (123.4000, 48.0100) | Origin recentered; pad-pair midpoint unchanged |
| R44 | (160.4420, 71.1780) | (160.6500, 71.1780) | 0.208 mm |
| TP1 | (170.0784, 78.0796) | (172.4000, 80.6500) | 3.464 mm |

## Added components

The 16 additions comprise ten input-protection parts and six Qwiic-interface parts. The redundant J18 was removed from the intermediate v1.05 design. No original component reference was removed.

| Reference | Value/part | Board position in mm |
| --- | --- | --- |
| C64 | 3.3nF 50V X7R ±10% | (148.0000, 52.5000) |
| C65 | 1uF 50V X7R ±10% | (140.5000, 43.5000) |
| C66 | 100nF 50V X7R ±10% | (144.2500, 46.6000) |
| C67 | 4.7uF 50V X7R ±10% | (146.8000, 55.3000) |
| C68 | 100nF 50V X7R ±10% | (189.0000, 109.0000) |
| C69 | 100nF 50V X7R ±10% | (193.0000, 109.0000) |
| D36 | SMBJ13A | (146.0000, 42.3000) |
| R78 | 680k ±1% | (139.0000, 46.5000) |
| R79 | 100k ±1% | (140.0000, 50.5000) |
| R80 | 390k ±1% | (141.5000, 53.7000) |
| R81 | 1.65k ±1% | (148.5000, 50.0000) |
| R82 | 4K7/0603/1% | (190.0000, 117.0000) |
| R83 | 4K7/0603/1% | (192.5000, 117.0000) |
| R84 | 10K/0603/1% | (195.5000, 114.0000) |
| U12 | TPS259472ARPWR | (144.5000, 50.5000) |
| U13 | TCA9517ADGKR | (190.7500, 112.0000) |

## Rule settings and validation

Eight PCB checks that v1 ignored are enabled in v1.05: courtyard overlap is an error; missing courtyard, silkscreen-to-edge, text height, footprint-library mismatch, footprint type mismatch, footprint-library issues and silkscreen-over-copper are warnings. Other board settings, numerical clearances, ERC settings and netclass settings are unchanged. No explicit rule exclusions were added.

Fresh post-removal v1.05 ERC reports zero errors/warnings. Saved/refilled DRC reports zero violations, unrouted connections or schematic-parity findings. Project rule settings are unchanged; current source hashes are recorded in the removal evidence. This comparison does not claim a fresh v1 ERC/DRC run.

## Functions retained

Two fixed-direction, intermittent-use pumps and two valve channels remain. The four AO3400A low-side stages and their flyback circuits retain their topology. The TPS563203 converters, PCA9685 PWM controller, TCA9534A expander, MPRLS pressure sensor, four switches and address straps remain. Communication loss still retains the last commanded state. No communication watchdog, H-bridge, active sensor reset or lower valve-voltage setting was added. Multiple boards use the external mux arrangement described in the assembly guide; the mux is not an added on-board component.

## Full component comparison

Every one of the 165 current references is included below. “Value/spec” can mean a label, tolerance or procurement correction; it does not imply that a different part was actually assembled on v1. “Pads” includes shape, size, drill or position changes. Artwork is assessed separately from connectivity. Sub-0.00001 mm export rounding is normalized.

| Reference | v1 value | v1.05 value | Schematic/library change | Layout change |
| --- | --- | --- | --- | --- |
| C1 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C2 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C3 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C4 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C5 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C8 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C9 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C15 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C16 | 10uF/25V/0805/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C22 | 10nF/25V/0603/1% | 10nF 50V ±10% | Value/spec | Artwork |
| C23 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C24 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C25 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C26 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C27 | 1nF/25V/0603/1% | 1nF 50V X7R ±5% | Value/spec | Artwork |
| C28 | 10uF/25V/0603/1% | 10uF 25V X5R ±10% | Value/spec | Artwork |
| C29 | 10uF/25V/0603/1% | 10uF 25V X5R ±10% | Value/spec | Moved; Pads; Artwork |
| C30 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Moved; Pads; Artwork |
| C31 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C32 | 100uF/16V | 22uF 50V ±20% | Value/spec; Footprint link | Moved; Pads; Artwork |
| C33 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Moved; Pads; Artwork |
| C34 | 100uF/16V | 22uF 50V ±20% | Value/spec; Footprint link | Artwork |
| C42 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C43 | 100uF/16V | 22uF 50V ±20% | Value/spec; Footprint link | Artwork |
| C44 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C45 | 100uF/16V | 22uF 50V ±20% | Value/spec; Footprint link | Artwork |
| C46 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Moved; Pads; Artwork |
| C47 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C52 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C53 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C54 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C55 | 18pF/25V/0603/1%/C0G/NP0 | 18pF 50V C0G ±2% | Value/spec | Artwork |
| C56 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C57 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C58 | 18pF/25V/0603/1%/C0G/NP0 | 18pF 50V C0G ±2% | Value/spec | Artwork |
| C59 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C60 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C61 | 22uF/25V/0805/1% | 22uF 25V X5R ±20% | Value/spec | Artwork |
| C62 | 18pF/25V/0603/1%/C0G/NP0 | 18pF 50V C0G ±2% | Value/spec | Artwork |
| C63 | 0.1uF/25V/0603/1% | 100nF 50V X7R ±10% | Value/spec | Artwork |
| C64 | Absent | 3.3nF 50V X7R ±10% | Added | New footprint and routing |
| C65 | Absent | 1uF 50V X7R ±10% | Added | New footprint and routing |
| C66 | Absent | 100nF 50V X7R ±10% | Added | New footprint and routing |
| C67 | Absent | 4.7uF 50V X7R ±10% | Added | New footprint and routing |
| C68 | Absent | 100nF 50V X7R ±10% | Added | New footprint and routing |
| C69 | Absent | 100nF 50V X7R ±10% | Added | New footprint and routing |
| D1 | SS54 | B540C-13-F | Value/spec; Footprint link; pin 1: VIN → VIN_RAW | Pads; Artwork |
| D6 | LED_0 | LED_Red | Value/spec | Artwork |
| D7 | LED_1 | LED_Red | Value/spec | Artwork |
| D8 | LED_2 | LED_Red | Value/spec | Artwork |
| D11 | SS34 | SS34 | Footprint link | Pads; Artwork |
| D12 | SS34 | SS34 | Footprint link | Pads; Artwork |
| D13 | LED_Red | LED_Red | Unchanged | Artwork |
| D14 | LED_Red | LED_Red | Unchanged | Artwork |
| D15 | LED_RED | LED_Red | Value/spec | Moved; Pads; Artwork |
| D16 | SS34 | SS34 | Footprint link | Pads; Artwork |
| D17 | LED_Red | LED_Red | Unchanged | Artwork |
| D18 | SS34 | SS34 | Footprint link | Pads; Artwork |
| D19 | LED_Red | LED_Red | Unchanged | Artwork |
| D33 | LED_RED | LED_Red | Value/spec | Artwork |
| D34 | LED_RED | LED_Red | Value/spec | Artwork |
| D35 | LED_RED | LED_Red | Value/spec | Artwork |
| D36 | Absent | SMBJ13A | Added | New footprint and routing |
| F1 | 2A | 2A time-delay JFC1032-1200TS | Value/spec; Footprint link | Origin correction; Pads; Assembly category; Artwork |
| H1 | MountingHole | MountingHole | Unchanged | Unchanged |
| H2 | MountingHole | MountingHole | Unchanged | Unchanged |
| H3 | MountingHole | MountingHole | Unchanged | Unchanged |
| H4 | MountingHole | MountingHole | Unchanged | Unchanged |
| J1 | I2C_4 | JST SH 4-pin 1.00mm | Value/spec | Artwork |
| J2 | PJ-082BH | PJ-082BH | Footprint link | Unchanged |
| J3 | I2C_5 | JST EH 4-pin 2.50mm | Value/spec | Unchanged |
| J9 | I2C_1 | JST SH 4-pin 1.00mm | Value/spec; pin 2: +3V3 → /HOST_3V3; pin 3: /SDA → /HOST_SDA; pin 4: /SCL → /HOST_SCL | Artwork |
| J12 | I2C_2 | JST SH 4-pin 1.00mm | Value/spec | Artwork |
| J17 | I2C_3 | JST SH 4-pin 1.00mm | Value/spec | Artwork |
| JP2 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| JP3 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| JP4 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| JP5 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| JP6 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| JP7 | SolderJumper_2_Open | SolderJumper_2_Open | Unchanged | Unchanged |
| L1 | 4.7uH/4A | 4.7uH FXL0530-4R7-M | Value/spec; Footprint link | Pads; Artwork |
| L2 | 4.7uH/4A | 4.7uH FXL0530-4R7-M | Value/spec; Footprint link | Pads; Artwork |
| L5 | 4.7uH/4A | 4.7uH FXL0530-4R7-M | Value/spec; Footprint link | Pads; Artwork |
| Q3 | AO3400A | AO3400A | Footprint link | Unchanged |
| Q4 | AO3400A | AO3400A | Footprint link | Unchanged |
| Q8 | AO3400A | AO3400A | Footprint link | Unchanged |
| Q10 | AO3400A | AO3400A | Footprint link | Unchanged |
| R1 | 100K/25V/0603/1% | 100K/0603/1% | Value/spec | Artwork |
| R4 | 100K/25V/0603/1% | 100K/0603/1% | Value/spec | Artwork |
| R7 | 100K/25V/0603/1% | 100K/0603/1% | Value/spec | Artwork |
| R10 | 2K2/25V/0603/1% | 2K2/0603/1% | Value/spec | Artwork |
| R11 | 4K7/25V/0603/1% | 4K7/0603/1% | Value/spec | Artwork |
| R13 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R14 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R15 | 4K7/25V/0603/1% | 4K7/0603/1% | Value/spec | Artwork |
| R16 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R17 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R18 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R19 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R20 | 4K7/25V/0603/1% | 4K7/0603/1% | Value/spec | Artwork |
| R22 | 10k/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R23 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R24 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R25 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R26 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R29 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R30 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R31 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R32 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R33 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R34 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R35 | 100R/25V/0603/1% | 100R/0603/1% | Value/spec | Artwork |
| R36 | 100R/25V/0603/1% | 100R/0603/1% | Value/spec | Artwork |
| R37 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R38 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R39 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R40 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R42 | 100R/25V/0603/1% | 100R/0603/1% | Value/spec | Artwork |
| R44 | 330/25V/0603/1% | 330/0603/1% | Value/spec | Moved; Pads; Artwork |
| R45 | 197K/25V/0603/1% | 196k ±1% | Value/spec | Artwork |
| R46 | 30K/25V/0603/1% | 30K/0603/1% | Value/spec | Artwork |
| R51 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R54 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R55 | 100R/25V/0603/1% | 100R/0603/1% | Value/spec | Artwork |
| R56 | 10K/25V/0603/1% | 10K/0603/1% | Value/spec | Artwork |
| R59 | 1K/25V/0603/1% | 1K/0603/1% | Value/spec | Artwork |
| R60 | 270K/25V/0603/1% | 270K/0603/1% | Value/spec | Artwork |
| R61 | 135K/25V/0603/1% | 135k ±0.1% | Value/spec | Artwork |
| R73 | 550/25V/0603/1% | 549R ±1% | Value/spec | Artwork |
| R74 | 30K/25V/0603/1% | 30K/0603/1% | Value/spec | Artwork |
| R75 | 1k/25V/0603/1% | 1k/0603/1% | Value/spec | Artwork |
| R76 | 30K/25V/0603/1% | 30K/0603/1% | Value/spec | Artwork |
| R77 | 330/25V/0603/1% | 330/0603/1% | Value/spec | Artwork |
| R78 | Absent | 680k ±1% | Added | New footprint and routing |
| R79 | Absent | 100k ±1% | Added | New footprint and routing |
| R80 | Absent | 390k ±1% | Added | New footprint and routing |
| R81 | Absent | 1.65k ±1% | Added | New footprint and routing |
| R82 | Absent | 4K7/0603/1% | Added | New footprint and routing |
| R83 | Absent | 4K7/0603/1% | Added | New footprint and routing |
| R84 | Absent | 10K/0603/1% | Added | New footprint and routing |
| SW1 | SW2 | TL3301NF160QG/P010515 | Value/spec; Footprint link | Artwork |
| SW2 | SW2 | TL3301NF160QG/P010515 | Value/spec; Footprint link | Artwork |
| SW3 | SW2 | TL3301NF160QG/P010515 | Value/spec; Footprint link | Artwork |
| SW4 | SW2 | TL3301NF160QG/P010515 | Value/spec; Footprint link | Artwork |
| TP1 | TestPoint | TestPoint | Unchanged | Moved; Pads |
| TP2 | TestPoint_EOC | TestPoint_EOC | Footprint link | Pads; Artwork |
| TP3 | TestPoint_RES | TestPoint_RES | Footprint link | Pads; Artwork |
| TP4 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| TP5 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| TP6 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| TP7 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| TP8 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| TP9 | TestPoint | TestPoint | Footprint link | Pads; Artwork |
| U1 | TPS563203DRLR | TPS563203DRLR | Footprint link | Unchanged |
| U2 | TPS563203DRLR | TPS563203DRLR | Footprint link | Unchanged |
| U3 | TPS563203DRLR | TPS563203DRLR | Footprint link | Unchanged |
| U4 | TCA9534APWR | TCA9534APWR | Footprint link | Unchanged |
| U5 | MPRLS0025PA00001A | MPRLS0025PA00001A | Footprint link | Unchanged |
| U7 | PCA9685 | PCA9685PW,118 | Value/spec; Footprint link | Unchanged |
| U8 | PUMP | ZR370-02PM / 4.5V / 2.5 LPM / ~500 mA | Value/spec; Footprint link | Assembly category; Artwork |
| U9 | VALVE | JST XH 2-pin 2.50mm | Value/spec; Footprint link | Pads; Assembly category; Artwork |
| U10 | VALVE2 | JST XH 2-pin 2.50mm | Value/spec; Footprint link | Pads; Assembly category; Artwork |
| U11 | PUMP | ZR370-02PM / 4.5V / 2.5 LPM / ~500 mA | Value/spec; Footprint link | Assembly category; Artwork |
| U12 | Absent | TPS259472ARPWR | Added | New footprint and routing |
| U13 | Absent | TCA9517ADGKR | Added | New footprint and routing |

## Footprint link changes

| References | v1 footprint | v1.05 footprint |
| --- | --- | --- |
| C32, C34, C43, C45 | Capacitor_SMD:CP_Elec_6.3x5.4_Nichicon | Capacitor_SMD:CP_Elec_6.3x5.8 |
| D1 | Sensor_Pressure:SS54 | Diode_SMD:D_SMC |
| D11, D12, D16, D18 | Khach Footprint:SS34 | Diode_SMD:D_SMA |
| F1 | Sensor_Pressure:DAT_FUSE | RheoBoard:Fuse_JDT_JFC1032TS_10.25x3.2mm |
| J2 | Khach Footprint:PJ-082BH | RheoBoard:PJ-082BH |
| L1, L2, L5 | Khach Footprint:4.7uH_2A | Inductor_SMD:L_Changjiang_FXL0530 |
| Q3, Q4, Q8, Q10 | Khach Footprint:AO3400A | RheoBoard:AO3400A |
| SW1, SW2, SW3, SW4 | Button_Switch_SMD:SW_Push_1P1T_NO_E-Switch_TL3301NxxxxxG | RheoBoard:Tactile_Switch_4pad |
| TP2, TP3, TP4, TP5, TP6, TP7, TP8, TP9 | Connector_PinHeader_1.00mm:PinHeader_1x01_P1.00mm_Vertical | RheoBoard:TestPoint_Plated_D1.0_Drill0.5 |
| U1, U2, U3 | Khach Footprint:TPS563203DRLR | RheoBoard:TPS563203DRLR |
| U4 | Khach Footprint:TCA9534APWR | RheoBoard:TCA9534APWR |
| U5 | Khach Footprint:MPRLS0025PA00001A | RheoBoard:MPRLS0025PA00001A |
| U7 | Khach Footprint:PCA9685 | RheoBoard:PCA9685 |
| U8, U11 | Khach Footprint:PUMP | RheoBoard:PUMP |
| U9, U10 | Khach Footprint:VALVE | RheoBoard:VALVE_v105 |

## Drawings and evidence

| View | v1 | v1.05 |
| --- | --- | --- |
| Schematic | [v1](Comparison_2026-10-09/v1-schematic.pdf) | [v1.05](../Manufacturing/Drawings/RheoBoard_v1.05_schematic.pdf) |
| Front copper (v1 view includes silkscreen) | [v1](Comparison_2026-10-09/v1-F.pdf) | [v1.05](../Manufacturing/Drawings/RheoBoard_v1.05_front_copper.pdf) |
| Back copper (v1 view includes silkscreen) | [v1](Comparison_2026-10-09/v1-B.pdf) | [v1.05](../Manufacturing/Drawings/RheoBoard_v1.05_back_copper.pdf) |

[Current structured comparison](Remove_J18_2026-10-09/v1-comparison.json), [symbol pin classifications](Comparison_2026-10-09/symbol-pin-definition-changes.json), [rule-setting comparison](Comparison_2026-10-09/rule-metadata-comparison.json), [current source integrity](Remove_J18_2026-10-09/validation.json). Current v1.05 before/after netlists and IPC-2581 exports are in Remove_J18_2026-10-09. Comparison_2026-10-09 retains original v1 exports and the historical pre-removal v1.05 checkpoint; its v1.05 drawings and hashes are superseded. [Current silkscreen](Remove_J18_2026-10-09/silkscreen.pdf).

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
