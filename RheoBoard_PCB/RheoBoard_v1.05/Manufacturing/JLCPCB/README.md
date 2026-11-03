# Order RheoBoard v1.05 from JLCPCB

This folder contains the three matching upload files for PCB fabrication and
assembly of **142 component positions per board**: 137 surface-mount parts and
five through-hole connectors (`J2`, `J3`, `J19`, `U9`, `U10`). The two pumps
(`U8`, `U11`) are excluded.

The external ESP32 controller, power supplies, valves, cables and tubing are
separate purchases. `U9` and `U10` in this assembly are the valve connectors.
Controller and firmware requirements are in the [PCB overview](../../../README.md).
See the [wiki BOM](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/BOM)
for the assembly scope and separate parts.

## Upload these files together

Download and extract the repository ZIP, then open this folder. Upload the
fabrication ZIP as-is, and upload the two CSV files at the assembly step.

| File | Upload at |
| --- | --- |
| [RheoBoard_v1.05_fabrication.zip](RheoBoard_v1.05_fabrication.zip) | PCB/Gerber upload |
| [BOM_RheoBoard_v1.05_no_pumps.csv](BOM_RheoBoard_v1.05_no_pumps.csv) | Assembly BOM upload |
| [CPL_RheoBoard_v1.05_no_pumps.csv](CPL_RheoBoard_v1.05_no_pumps.csv) | Assembly CPL / pick-and-place upload |

Use the BOM and CPL in this folder for the complete 142-position assembly.
The older SMT-only exports elsewhere contain 137 positions and omit the five
through-hole connectors. Do not mix those exports with this package.

## Order settings

Start a PCB order on [JLCPCB](https://jlcpcb.com/), upload the fabrication ZIP,
and enable PCB Assembly before checkout. PCB fabrication and assembly belong
in the same order; see [JLCPCB's ordering steps](https://jlcpcb.com/help/article/how-do-i-place-a-pcba-order).

| Setting | Value |
| --- | --- |
| Material / layers / thickness | FR-4 TG135 / 2 layers / 1.6 mm |
| PCB / silkscreen colour | Green / white |
| Copper / finish | 1 oz / ENIG, 1 microinch gold |
| Via covering / minimum via drill | Tented / 0.3 mm |
| Delivery format / outline tolerance | Single PCB / regular ±0.2 mm |
| Electrical test / PCB marking | Flying Probe Fully Test / Remove Mark |
| Assembly | Standard PCBA, Top Side |
| PCB and assembly quantities | Set both to the quantity wanted; use **5 and 5** for five assembled boards |
| Edge rails and fiducials | Added by JLCPCB |
| Depanel boards and edge rails | Yes |
| Board cleaning / conformal coating | No / No |
| Solder paste | High temp. |
| Production-file / parts-placement confirmation | Yes / Yes |

The bare PCB is **109 × 126.4 mm**. Let JLCPCB add its assembly rails; two
5 mm rails give an overall size of **119 × 126.4 mm**.

## Assembly and checkout

1. Upload the BOM and CPL above and process them.
2. Select every intended BOM group, including rows sharing the same part code.
   Expect **51 BOM groups, 142 designators and 46 distinct LCSC codes**. Confirm
   that `J2`, `J3`, `J19`, `U9` and `U10` are selected and `U8`/`U11` are absent.
3. Resolve stock shortages or unmatched parts against the listed part numbers.
   JLCPCB populates selected items only; see its [component-matching guide](https://jlcpcb.com/help/article/component-matching-guidelines-for-pcba-orders).
4. Review placements, pin 1 and polarity in the preview. Coordinates are in
   millimetres; preserve the supplied positions and rotations.
5. Enter the assembly remark below. Select **Research/Education/DIY/Entertainment
   → Development Board** as the product description, save to cart, and review
   quantities, selected parts, shipping and the final charges before payment.

Paste this into **Assembly remark**:

```text
Populate all 142 listed positions per board, including THT J2/J3/J19/U9/U10. Exclude U8/U11 pumps. U5 MPRLS0025PA00001A: reflow <=250C/15s; no-clean flux; protect port/gel; no washing, immersion or cleaning jets. U12 TPS259472ARPWR: follow TI RPW0010A solder-paste guidance and inspect hidden joints. J19 plated holes 1.10mm. Preserve CPL positions/rotations; no substitutions.
```

Component handling references: [Honeywell MPR datasheet](https://prod-edam.honeywell.com/content/dam/honeywell-edam/sps/siot/en-us/products/sensors/pressure-sensors/board-mount-pressure-sensors/micropressure-mpr-series/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf)
and [TI TPS25947 datasheet](https://www.ti.com/lit/ds/symlink/tps25947.pdf).

## Buying equivalent passive parts

The BOM lists orderable manufacturer parts. Use common 0603 resistors and the
listed capacitor/inductor packages. Prefer stocked JLCPCB Basic parts when their
electrical specifications match. Basic and Extended describe assembly-library
categories; Extended parts can also be widely available. Check the current
available order quantity, including assembly attrition, before checkout.

The selected 1% resistors can be used throughout. For replacement sourcing,
the 40 ordinary resistors may use up to **5%** tolerance. Keep **1% or better**
for `R45`, `R46`, `R60`, `R61`, `R74`, `R76`, `R78`, `R79`, `R80` and `R81`;
these set regulator voltages and input-protection thresholds. Preserve nominal
values, package, temperature coefficient and adequate power/voltage ratings.
The [full BOM](../../BOM/BOM_RheoBoard_v1.05.xlsx) records the allowed tolerance
and substitution requirements. TI also recommends 1% divider resistors for the
[TPS563203](https://www.ti.com/lit/gpn/tps563203).

For `R45`, **196 kΩ is intentional**. An equivalent to the listed Vishay
CRCW0603196KFKEA is [UNI-ROYAL 0603WAF1963T5E, C22930](https://jlcpcb.com/partdetail/UniRoyal-0603WAF1963T5E/C22930):
196 kΩ, 1%, 0603, 100 mW, 75 V, ±100 ppm/°C. Select and review any replacement
explicitly in the BOM before checkout; the assembly remark prohibits unreviewed
factory substitutions. Keep `R81` at **1.65 kΩ, 1%** to preserve the current-limit
setting; [C22845](https://jlcpcb.com/partdetail/UniRoyal-0603WAF1651T5E/C22845)
is the listed manufactured part.

For capacitors, preserve capacitance, voltage rating, dielectric and the listed
tolerance. Keep the 18/20/24 pF feedback capacitors C0G/NP0 at 5% or better.
For inductors, match the land pattern, inductance, saturation/RMS current and
DC resistance. A matching printed value alone does not establish equivalence.

Hardware license: CERN-OHL-W-2.0 — see [LICENSE](../../../../LICENSE).
