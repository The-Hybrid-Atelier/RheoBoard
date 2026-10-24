# RheoBoard v1.05 footprint audit — 2026-10-24

**Verdict: NEEDS ATTENTION; fabrication release remains BLOCKED.** All 160 placed footprints / 428 pad objects / 32 footprint types were inspected from the live board. The regulator land patterns, U5/U7 mask openings and silkscreen margins found by the exported-file review have now been corrected and rechecked. Exact D36 qualification, J19 metadata and physical/supplier acceptance remain open. Inspection coverage is complete; this is not a blanket declaration that every purchased part or assembly orientation is qualified.

Hardware design licensing follows the root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Findings that need action

The three production-geometry findings below are corrected. The separate supplier and physical acceptance items remain open.

### Additional Gerber findings — 2026-10-24

- **U5/U7 mask openings corrected in the project libraries and placed footprints.** Expansion is now 0.05 mm. Exported minimum webs are 0.1192 mm at U5 and 0.1794 mm at U7; their copper and paste geometry is unchanged. These meet the 0.10 mm bridge criterion for standard colored 1 oz production. U5 does **not** meet the 0.13 mm black/white criterion: select a compatible process or obtain supplier acceptance before changing mask color/copper weight. [JLCPCB capabilities](https://jlcpcb.com/capabilities/Capabilities).
- **Silkscreen production margins corrected.** Exported legend strokes are at least 0.15 mm and mask-opening clearance is at least 0.17 mm. Thin outlines were widened and locally shortened or offset; R83 moved 0.30 mm and U13 moved 2.50 mm. Previously 0.8 mm visible labels now use 1.0 mm text. Pin-1 and polarity markers remain. Two valve outlines were separated from neighboring diode outlines. [JLCPCB legend guidance](https://jlcpcb.com/capabilities/Capabilities).

### Earlier footprint findings

1. **U1/U2/U3 regulator lands corrected in the library and board.** All eighteen copper/paste lands now follow TI DRL0006A: 0.67 × 0.30 mm, 0.50 mm pitch, 1.48 mm opposing centers, 0.05 mm corner radius and 0.025 mm mask expansion. Placement and pad-to-net assignments are preserved; routing, refill and stencil containment were rechecked. [TI drawing, pp27–28](https://www.ti.com/lit/gpn/tps563203).
2. **LED numbering corrected after this audit.** All eleven SZYY0603R LEDs now use project-local symbols and footprints with manufacturer pin/pad 1 = A (+), 2 = K (-). All 22 physical pad-to-net mappings are preserved. The [pin-map record](LED_PIN_MAP.md) and [assembly guide](Manufacturing/Drawings/RheoBoard_v1.05_LED_assembly.pdf) replace the former cross-numbering instruction. Supplier placement-preview acceptance remains open; no arbitrary CPL rotation offset was applied. [Manufacturer drawing, p2](https://datasheet.lcsc.com/datasheet/pdf/7948f9a4498910c84f6e992ff3fa9e63.pdf).
3. **Mechanical acceptance remains open:** custom pump terminals/holes U8/U11, extra valve mounting features U9/U10, actual connector/header fit and enclosure/fastener access need physical checks. J19 also has generic schematic metadata versus a specific Adafruit 392 BOM selection. D36's current manufacturer drawing could not be retrieved, so its exact land comparison remains incomplete.
4. **U7 optional refinement:** custom pad pitch is rounded from a metric grid, with maximum center error 0.011 mm. Correct package/pins; no demonstrated assembly failure. [NXP package drawing](https://www.nxp.com/docs/en/package-information/SOT361-1.pdf).

The initial review was read-only. Historical LED-pass result: the subsequent LED correction changes symbol/footprint numbering and library IDs, updates manufacturing outputs and restores the LED reference labels to F.Fab. Component placement, pad dimensions, all 552 routes and all 301 vias are preserved. Teardrops are regenerated (363 valid, connected contours); refill differences are below 0.001 mm at the boundary. Fresh ERC and refilled DRC/connectivity/parity report zero findings. Other findings still require their stated correction or acceptance.

## Board and document checks

- All 160 pad queries returned live IPC evidence; no fallback or missing component. Within each of 32 footprint IDs, every instance has identical local pad geometry, drill, shape, rotation and layer settings after transforming placement.
- All expected exported schematic pins exist on the board. Functional net assignments agree. Only U12's intentionally unused compound pad 10 has two automatically generated NC aliases.
- BOM footprint strings agree with every purchased board item; quantities and reference sets have no duplicate or extra parts. The 18 board-only objects are four mounting holes, six solder jumpers and eight testpoints. J19's MPN description differs as noted above.
- All 135 SMT CPL rows match live X/Y, top side and rotation; SMT BOM/CPL reference sets agree. This does **not** validate the supplier's component-zero-angle convention.
- Final native refill/save and full info-level DRC: zero errors, zero unconnected items, zero schematic-parity findings and no missing categories. There are 23 library-mismatch warnings from intentional board-specific silkscreen geometry; no warning is hidden or excluded. The unchanged schematic previously passed ERC with zero errors/warnings.
- Existing rules include 0.20 mm copper clearance, 0.20 mm track width, 0.30 mm via drill, 0.50 mm via diameter, 0.50 mm copper-to-edge and 0.25 mm hole spacing. No DRC exclusions. Three existing checks are ignored: footprint filter mismatch, trace centering on vias, tuning-profile geometry. Minimum silkscreen clearance is zero. Therefore the DRC result is not evidence of a positive silkscreen-to-pad production margin or perfect trace centering.
- SMD lands include mask/paste layers as expected. U12 intentionally uses separate paste-only apertures and copper-only compound lands; these match its TI stencil drawing. Solder jumpers omit paste intentionally. Individual mask/paste release and assembler process approval remain manufacturing checks.

## Coverage by footprint family

“Compatible” means package family, terminal geometry and net/polarity mapping are supported by the checked drawings; it does not mean the alternate land pattern is identical to the manufacturer's example or physically solder-tested. Dimensions below are millimetres.

| References | Footprint | Result | Evidence / limitation |
| --- | --- | --- | --- |
| R1, R4, R7, R15, R16, R17, R18, R19, R20, R22, R23, R24, R25, R26, R29, R30, R31, R32, R33, R34, R35, R36, R37, R38, R39, R40, R42, R44, R45, R46, R51, R54, R55, R56, R59, R60, R61, R73, R74, R75, R76, R77, R82, R83, R84, R85 | Resistor_SMD:R_0603_1608Metric_Pad0.98x0.95mm_HandSolder | Compatible package family | 0.975 × 0.95 mm lands; 1.825 mm centers. 0603 resistors including UNI-ROYAL, Vishay CRCW and YAGEO RT match the specified body/terminal size family. Extended hand-solder lands, not an exact copy of each supplier recommendation. |
| D37 | Diode_SMD:D_SOD-123 | Compatible alternate lands | D37: 0.90 × 1.20 mm, 3.30 mm centers. Diodes SOD123 maximum lead span 3.85 mm and lead width 0.62 mm fit the land envelope; cathode on VIN_RAW. |
| JP2, JP3, JP4, JP5, JP6, JP7 | Jumper:SolderJumper-2_P1.3mm_Open_Pad1.0x1.5mm | Board feature checked | Two 1.0 × 1.5 mm lands, 1.30 mm centers, 0.30 mm open gap. No purchased part or paste required. |
| C3, C4, C5, C23, C24, C25, C26, C27, C28, C29, C30, C31, C33, C42, C44, C46, C55, C56, C58, C59, C62, C63, C68, C69 | Capacitor_SMD:C_0603_1608Metric_Pad1.08x0.95mm_HandSolder | Compatible package family | 1.075 × 0.95 mm lands; 1.725 mm centers. YAGEO CC, CCTC TCC and Vishay VJ specify 1608 bodies. These are extended lands; exact paste/process qualification remains with assembly. |
| C32, C34, C43, C45 | Capacitor_SMD:CP_Elec_6.3x5.8 | Compatible alternate lands | Panasonic EEEFK1H220P is diameter 6.3 × height 5.8 mm. Board lands 3.50 × 1.60 mm, gap 1.90 mm; Panasonic recommendation 3.20 × 1.60 mm, gap 1.80 mm. Positive pad 1 on supply, negative pad 2 on GND for all four. |
| R78, R79, R80, R81 | Resistor_SMD:R_0603_1608Metric | Compatible package family | 0.80 × 0.95 mm lands; 1.65 mm centers. R80 HP03 is also a 1608 body; high-power naming does not imply a larger package. |
| TP2, TP3, TP4, TP5, TP6, TP7, TP8, TP9 | RheoBoard:TestPoint_Plated_D1.0_Drill0.5 | Board feature checked | 1.0 mm circular copper / 0.5 mm plated hole; expected test net. No purchased part. |
| D6, D7, D8, D13, D14, D15, D17, D19 | RheoBoard:LED_SZYY0603R_A1_K2_HandSolder | Numbering corrected; preview pending | 1.05 × 0.95 mm lands, 1.75 mm centers, 0.70 mm gap; compatible enlarged SZYY0603R pattern. Manufacturer-matched 1=A, 2=K; physical polarity retained. |
| J2 | RheoBoard:PJ-082BH | Drawing matched | J2 center-positive pad 1, sleeve pad 2, four shield slots and locating hole. Signal drill 1.50 mm; sleeve slot 3.0 × 0.8; shields 2.0 × 0.8 on 9.70 mm rows and 4.70 mm pitch; locator 1.60 mm. Centers match Same Sky top-view drawing after rotation. |
| SW1, SW2, SW3, SW4 | RheoBoard:Tactile_Switch_4pad | Drawing matched | Four 2.10 × 1.40 mm lands on 9.10 × 4.50 mm centers, giving 11.20 × 5.90 mm outer envelope and 7.00 × 3.10 mm gaps. E-Switch D310515 matches. Pins 1/2 share signal; 3/4 share GND, so press closes the intended circuit. |
| D33, D34, D35 | RheoBoard:LED_SZYY0603R_A1_K2_Nominal | Numbering corrected; preview pending | 0.875 × 0.95 mm lands, 1.575 mm centers, 0.70 mm gap; compatible enlarged SZYY0603R pattern. Manufacturer-matched 1=A, 2=K; physical polarity retained. |
| J1, J9, J12, J17 | Connector_JST:JST_SH_SM04B-SRSS-TB_1x04-1MP_P1.00mm_Horizontal | Compatible connector pattern | Four 0.60 × 1.55 mm signal lands at 1.00 mm pitch; two 1.20 × 1.80 mm anchors. Qwiic pin order GND, power, SDA, SCL. J9 host domain; J1/J12/J17 local domain. Same geometry on all four. |
| C1, C2, C8, C9, C15, C16, C52, C53, C54, C57, C60, C61 | Capacitor_SMD:C_0805_2012Metric_Pad1.18x1.45mm_HandSolder | Compatible package family | 1.175 × 1.45 mm lands; 2.075 mm centers. CCTC 0805/2012 body matches family. Extended hand-solder lands; no polarity. |
| C67 | Capacitor_SMD:C_1206_3216Metric | Compatible package family | C67 YAGEO 1206/3216 body; lands 1.15 × 1.80 mm at 2.95 mm centers; no polarity. |
| D11, D12, D16, D18 | Diode_SMD:D_SMA | Compatible enlarged lands | SS34: 2.50 × 1.80 mm lands at 4.00 mm centers. Jingdao recommends 1.80 × 1.80 mm at the same centers; existing pads extend heel/toe. Cathode pad 1 on load supply; anode pad 2 on switched load net. |
| H1, H2, H3, H4 | MountingHole:MountingHole_3.5mm_Pad | Board feature checked | 3.50 mm drill and 7.00 mm pad. Mechanical fastener, washer and enclosure fit are physical checks. |
| L1, L2, L5 | Inductor_SMD:L_Changjiang_FXL0530 | Drawing matched | L1/L2/L5: 1.90 × 2.50 mm lands, 4.10 mm centers. Gap 2.20 mm and outer span 6.00 mm match CJIANG recommended pattern. No polarity. |
| Q3, Q4, Q8, Q10 | RheoBoard:AO3400A | Compatible alternate lands | Correct 1 gate / 2 source / 3 drain. 1.21 × 0.59 mm lands cover the specified terminal widths; see active-device detail. |
| C64, C66 | Capacitor_SMD:C_0603_1608Metric | Compatible package family | C64/C66 YAGEO 0603/1608. 0.90 × 0.95 mm lands; 1.55 mm centers; no polarity. |
| U9, U10 | RheoBoard:VALVE_v105 | Header fits; mechanical fit open | U9/U10 JST XH two-pin interface has 2.50 mm pitch and 1.00 mm holes, matching JST two-circuit layout. Pad 1 +6V; pad 2 switched return. Extra 3.50 mm hole and composite body artwork are custom mechanical features, not guaranteed by the JST drawing. |
| Q11 | Package_TO_SOT_SMD:SOT-223-3_TabPin2 | Compatible enlarged lands | Q11 correct gate 1, drain/tab 2, source 3. 2.30 mm lead pitch. Enlarged lands contain the Diodes recommended land rectangles; see active-device detail. |
| C65 | Capacitor_SMD:C_0805_2012Metric | Compatible package family | C65 YAGEO 0805/2012. 1.00 × 1.45 mm lands; 1.90 mm centers; no polarity. |
| D36 | Diode_SMD:D_SMB | Exact drawing acceptance incomplete | D36 uses SMB lands 2.50 × 2.30 mm at 4.30 mm centers and correct cathode VIN_RAW / anode GND. Current Littelfuse drawing links redirect to missing pages; exact current land-pattern comparison was not completed in this run. Do not count this as an exact drawing pass. |
| U1, U2, U3 | RheoBoard:TPS563203DRLR | Corrected to TI drawing | U1/U2/U3: corrected 0.67 × 0.30 mm copper/paste lands, 0.50 mm pitch, 1.48 mm row centers, 0.05 mm corner radius. Exported geometry verified; original placements and nets retained. |
| U8, U11 | RheoBoard:PUMP | Mechanical fit incomplete | U8/U11: two 5.0 mm copper pads with 4.0 mm holes, 18.24 mm centers, correct supply/switched-return nets. Custom motor terminal/mount geometry is not an ordinary standard connector footprint. Actual pump/lead fit, insulation and strain relief need physical confirmation. |
| U5 | RheoBoard:MPRLS0025PA00001A | Drawing matched | U5 all twelve lands match Honeywell after rotation: 0.70 × 0.65 mm, 1.27 mm pitch, 4.20 mm opposing rows. Pin functions correct; copper/paste unchanged. Mask expansion corrected to 0.05 mm; exported minimum web 0.1192 mm, requiring the stated colored 1 oz process. |
| U7 | RheoBoard:PCA9685 | Minor refinement | U7 pin map correct. Adjacent pitch 0.6350/0.6604 mm instead of exact 0.6500; maximum center error 0.0110 mm. Not a demonstrated fit failure; normalize when revising custom library. |
| U13 | RheoBoard:TI_DGK0008A_VSSOP_3x3mm_P0.65mm | Drawing matched | U13: 1.40 × 0.45 mm lands, 0.65 mm pitch, 4.40 mm rows; TI DGK pattern and all eight pin functions match. |
| U12 | RheoBoard:TI_RPW0010A_VQFN-HR_2x2mm | Drawing matched | U12 TI RPW copper and split stencil pattern match. Duplicate physical NC pad 10 pieces have distinct automatic unconnected aliases; intentional unused terminal, no functional signal mismatch. |
| J19 | Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical | Metadata / exact fit open | J19: 2.54 mm pitch, 1.00 mm drills, 1.70 mm lands. BOM selects Adafruit 392 cut to five; schematic remains generic. Pitch agrees with supplier; exact pin-tolerance drawing and purchased-part fit not verified. |
| J3 | Connector_JST:JST_EH_B4B-EH-A_1x04_P2.50mm_Vertical | Compatible connector pattern | J3: 2.50 mm pitch, 0.95 mm holes and 1.70 × 1.95 mm pads; JST EH four-pin body/pitch, correct GND/+3V3/SDA/SCL order. |
| F1 | RheoBoard:Fuse_JDT_JFC1032TS_10.25x3.2mm | Compatible drawing dimensions | F1: 3.40 × 3.43 mm lands at 9.20 mm centers; outer span 12.60 mm and gap 5.80 mm. Within JDT recommended 3.25 ±0.30 width, 3.43 ±0.30 height, 12.60 ±0.30 span, 5.72 ±0.30 gap. Nonpolar. |

## LED assembly polarity targets

Numbers now agree: manufacturer terminal 2/cathode belongs on board pad 2, and terminal 1/anode belongs on board pad 1. The end bar and package green mark identify the cathode. Coordinates are top-side KiCad board coordinates, X right and Y down, in mm. They are not the CPL's inverted-Y format. Values below retain the audit's pre-update coordinates (native replacement changed some last decimal places by 0.000001 mm); the assembly guide uses final live readback.

| Ref | Cathode / pad 2 (X,Y) | Anode / pad 1 (X,Y) | Cathode net | Anode net |
|---|---|---|---|---|
| D6 | (158.445000, 52.257378) | (158.445000, 54.007376) | Net-(D6-K) | /PUMP2_PWM |
| D7 | (155.075000, 52.257378) | (155.075000, 54.007376) | Net-(D7-K) | /PUMP3_PWM |
| D8 | (151.800000, 52.257378) | (151.800000, 54.007376) | Net-(D8-K) | /PUMP4_PWM |
| D13 | (169.505000, 100.750000) | (171.255000, 100.750000) | Net-(D13-K) | +4V5 |
| D14 | (137.537500, 100.750000) | (139.287500, 100.750000) | Net-(D14-K) | +4V5 |
| D15 | (162.700000, 70.353001) | (162.700000, 72.102999) | GND | Net-(D15-A) |
| D17 | (125.415283, 138.701199) | (125.415283, 136.951201) | Net-(D17-K) | +6V |
| D19 | (157.683200, 138.791199) | (157.683200, 137.041201) | Net-(D19-K) | +6V |
| D33 | (133.692936, 71.085910) | (135.267936, 71.085910) | GND | Net-(D33-A) |
| D34 | (133.718758, 58.458263) | (135.293758, 58.458263) | GND | Net-(D34-A) |
| D35 | (105.944395, 64.059454) | (107.519395, 64.059454) | GND | Net-(D35-A) |

## Other manufacturer references

- [Same Sky PJ-082BH, p2](https://www.sameskydevices.com/product/resource/pj-082bh.pdf); [JST SH](https://www.jst-mfg.com/product/pdf/eng/eSH.pdf), [XH](https://www.jst-mfg.com/product/pdf/eng/eXH.pdf), [EH](https://www.jst-mfg.com/product/pdf/eng/eEH.pdf).
- [E-Switch TL3301NF160QG drawing](https://datasheet.octopart.com/TL3301NF160QG-E-Switch-datasheet-574176.pdf); [JDT JFC1032TS, p4](https://www.jdtfuse.com/upload/PDF/JFC1032TS.pdf).
- [Panasonic FK dimensions and land pattern](https://industrial.panasonic.com/cdbs/www-data/pdf/RDE0000/ABA0000C776.pdf); [CJIANG FXL catalogue, p5](https://datasheet.lcsc.com/datasheet/pdf/40dfb6cb4606b2ccaa4e89438a42f563.pdf); [Jingdao SS34, p3](https://datasheet.lcsc.com/datasheet/pdf/169c250fdfd349c3b43ab20fc05ceb01.pdf); [Diodes BZT52, p4](https://www.diodes.com/datasheet/download/BZT52Cxx.pdf).
- [CCTC MLCC dimensions](https://datasheet.lcsc.com/datasheet/pdf/7c7c1f8fb8282eab2492065cbbfa1665.pdf), [YAGEO CC0603](https://www.yageogroup.com/download/specsheet/CC0603KRX7R9BB104), [CC0805](https://www.yageogroup.com/download/specsheet/CC0805KKX7R9BB105), [CC1206](https://www.yageogroup.com/download/specsheet/CC1206KKX7R9BB475), [Vishay VJ](https://www.vishay.com/docs/45199/vjcommercialseries.pdf).
- [UNI-ROYAL standard resistors](https://www.uni-royal.cn/en/images/userfile/file/1753752986c56505e6d9ab55c7.pdf), [HP03](https://www.uni-royal.cn/images/userfile/file/1745835494a2e6d381ea80b5d9.pdf), [Vishay CRCW](https://www.vishay.com/docs/20035/dcrcwe3.pdf), [YAGEO RT0603](https://www.yageogroup.com/component-documentation/download/specsheet/RT0603BRD07135KL).
- [Honeywell MPR, p21](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf), [TI TPS25947](https://www.ti.com/lit/ds/symlink/tps25947.pdf), [TI TCA9517A](https://www.ti.com/lit/ds/symlink/tca9517a.pdf), [AOS AO3400A](https://www.aosmd.com/res/data_sheets/AO3400A.pdf), [AOS SOT23](https://www.aosmd.com/sites/default/files/res/package/SOT23.pdf), [Diodes DMP6023LE](https://www.diodes.com/assets/Datasheets/DMP6023LE.pdf).
- [Adafruit header 392](https://www.adafruit.com/product/392), [specified pump 4699](https://www.adafruit.com/product/4699). Product selection does not establish physical fit of custom board features.

Detailed live queries, normalized per-instance geometry, independent IC/LED review, BOM/CPL comparison and ERC/DRC reports are retained locally in the dated verification evidence. Existing prototype U5 startup/reset and valve-rail margin acceptance is unchanged; this footprint audit does not establish measured performance.


## Exported-file reconciliation and stage gates — DFM pass, 2026-10-24

Before the subsequent Qwiic move documented below, the reviewed board contained 160 footprints, 549 traces, 361 vias and 363 valid connected teardrops. All 301 original vias remain, with 60 new GND vias forming 30 aligned triples. The final DFM pass preserves all placements, net assignments, traces and vias; only the eighteen U1/U2/U3 copper lands change. Native refilled DRC reports zero errors, unconnected items or schematic-parity findings, with 23 deliberate silkscreen/library differences. The unchanged schematic previously passed ERC with zero findings.

Independent logical reconciliation finds all 394 schematic nodes on the board. The one additional board-node alias is the intentional second physical U12 pad 10 NC piece. All 160 component references match. All 135 SMT CPL references match the 42 BOM groups, schematic footprint/LCSC assignments and native/IPC X/Y/rotation/side within 0.000002 mm numerical tolerance. Seven `1k`/`1K` value strings and R79 notation are benign text differences. The workbook SMT sheet agrees with the CSV: 142 fitted parts, seven manual parts and eighteen board-only features. J19's generic schematic MPN versus the specific Adafruit 392 cut-to-five selection remains open metadata work.

The independent Gerber review confirms a simple closed 109.00 × 126.40 mm centerline outline, copper-to-edge clearance at least 0.5005 mm, and all 368 top-paste objects contained within copper and mask. Paste references cover exactly the 135 intended SMT parts; bottom paste/silk are intentionally empty. The twelve-file fabrication archive matches the canonical files byte for byte. The mask, legend and regulator-land corrections above are included in the refreshed exports. The library mismatch warnings are intentional board-local artwork edits; retain these edits when updating footprints from libraries. They do not indicate changed electrical pads beyond the documented regulator correction.

| Stage | Status | Evidence / remaining action |
| --- | --- | --- |
| Floorplan and mechanical | BLOCKED for full acceptance | Existing outline and holes reviewed; purchased-part, enclosure and fastener fit still require physical confirmation. |
| Placement and lands | PARTIAL | Component/pin coverage complete; regulator lands corrected. Exact D36 and physical custom-part qualification remain open. |
| Routing connectivity | PASS file check | No unconnected items; all 1,098 track endpoints contact same-net copper; pours refilled; 363 teardrops valid. Return-path, current and thermal performance remain unqualified. |
| Fabrication DRC/DFM | PASS for reviewed geometry | Zero electrical/connection/parity errors; corrected lands, stencil containment, mask webs and silk margins verified for the stated colored 1 oz process. 23 intentional library/artwork differences remain visible in DRC. |
| Netlist / ERC / BOM / CPL | PASS logical reconciliation | Zero ERC/parity findings, 394 nodes reconciled, all SMT rows agree. Exact-part/physical exceptions above remain open. |
| Parasitic extraction / integrity | BLOCKED | No formal extraction or complete IR-drop/current-density/loop-inductance model. Actual peak/continuous rail loads and acceptance targets are required. |
| Post-layout performance | BLOCKED | No completed SI/PI/thermal simulation or hardware measurements; accepted prototype sensor-startup and valve-margin risks remain explicit. |
| Fabrication release | BLOCKED | Files generated and reviewed, but exact-part checks and supplier placement/stencil/process acceptance remain outstanding. |

Assumptions are the existing two-layer, 1 oz fabrication basis and single-side SMT population. Mask color, finish, stackup/service selection, tooling/fiducials and approved substitutions must be confirmed for the actual order. No controlled-impedance or EMI qualification is inferred from tidy routing or aligned stitching. No order, upload, commit or push was made.


### Qwiic connector access and pin mapping — 2026-10-24 recheck

Live PCB pads and schematic agree for all four JST SH SM04B-SRSS-TB connectors: board pad 1 GND, 2 3.3 V, 3 SDA, 4 SCL, matching [SparkFun's Qwiic specification](https://www.sparkfun.com/qwiic) and the actual [Qwiic Adapter board](https://github.com/sparkfun/Qwiic_Adapter/blob/master/Hardware/SparkFun%20Qwiic%20Adapter.brd). J9 correctly uses the separate HOST_3V3/HOST_SDA/HOST_SCL domain; J1/J12/J17 use the local domain. Opposite sides of the board use 180-degree-opposed orientations; numbering is preserved. MP pads are mechanical solder anchors, not extra signal pins.

All sockets face outward. Before the authorized move below, nominal F.Fab mating-face setback was 6.225 mm for left J1/J17 and 3.700 mm for right J9/J12. SparkFun's Adapter front-outline setback is approximately 0.48 mm. Rear solder-tail placement within each footprint is normal; the whole connectors are recessed substantially farther from the edge. Recommend moving the mating faces to a consistent 0.5–1.0 mm inside the edge for easier insertion/removal, with corresponding routing and label updates. This is an access recommendation, not a Qwiic mandatory edge-spacing rule or evidence of an electrical defect. Physical cable/enclosure fit remains untested. No connector movement or design change made during this review.


### Applied Qwiic edge-access correction — 2026-10-24

All four Qwiic sockets now have a nominal 0.75 mm mating-face setback from the board edge. J1/J17 moved outward 5.475 mm to X92.525; J9/J12 moved outward 2.950 mm to X194.875. Y positions, rotations, pad geometry and the SparkFun pin order (1 GND, 2 3.3 V, 3 SDA, 4 SCL) are unchanged; J9 retains its separate host domain. Local routes, four signal vias and 22 labels were adjusted, one SDA via added, copper refilled and teardrops regenerated. Existing polygon boundaries already cover the moved pads, so no polygon extensions were needed.

The saved board has 546 track segments, 362 vias and 365 valid connected teardrops. All 1,092 trace endpoints contact same-net pads, vias or tracks. Final synchronized/refilled DRC reports zero errors, unconnected items or schematic-parity findings, with the same 23 intentional board-local silkscreen/library warnings. Independent Gerber review verifies 0.550 mm connector copper-to-edge clearance, 0.185 mm connector silk-to-mask clearance and unchanged whole-board minimum silk-to-mask clearance of 0.170078 mm. All 368 top paste objects remain contained in copper and mask; all other 156 component placements and pad records are unchanged.

Gerbers, plated/non-plated drills, the exact twelve-file fabrication ZIP, native positions, 135-row JLCPCB CPL, assembly, copper and mask drawings are refreshed. Only the four connector X coordinates change in the CPL; BOM population and rotations are unchanged. Supplier placement preview and physical cable/enclosure fit remain open, along with the previously documented exact-part and mechanical acceptance items.


### Final approved-project export reconciliation — 2026-10-24

After the user’s explicit “pass,” refreshed the fabrication package from the saved, refilled v1.05 project. The final artwork omits all four Qwiic role captions and sixteen Qwiic pin captions, includes the 0.75 mm connector mating-face setbacks, and retains the nine GND vias aligned into three 1 mm-pitch rows. All 160 components, 546 tracks, 362 vias and 365 teardrops match the approved copper snapshot.

Final ERC has zero findings. DRC reports zero errors, unconnected items and schematic-parity findings, with the same 23 documented board-local silkscreen/library warnings. Independent export comparison confirms unchanged copper/drills/pads, only the intended silkscreen removals, 368 contained paste apertures and 0.170078 mm minimum legend-to-mask clearance. All 394 schematic nodes reconcile; all 135 SMT BOM/CPL references agree with fresh native/IPC placements. Supplier-specific rotation acceptance remains a placement-preview check.

The twelve-file Gerber/drill/job ZIP, native positions, JLCPCB placement file, assembly, copper, solder-mask and schematic PDFs are refreshed. BOM population and the LED polarity guide remain unchanged and applicable. Validation reports and SHA-256 output/source hashes are in Manufacturing/Validation. This “pass” authorizes output refresh; no upload or order was placed, and the previously documented exact-part, supplier and physical acceptance items remain open.
