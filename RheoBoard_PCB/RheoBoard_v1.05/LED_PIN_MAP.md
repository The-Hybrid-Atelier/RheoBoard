# SZYY0603R LED pin-map acceptance

Yongyu Photoelectric SZYY0603R, 0603 / 1608 red LED, LCSC/JLCPCB C434419.
Source: [manufacturer specification, page 2 of 11](https://datasheet.lcsc.com/datasheet/pdf/7948f9a4498910c84f6e992ff3fa9e63.pdf), package profile and polarity diagram. The supplied PDF has no explicit revision identifier. Checked 2026-10-24.

The manufacturer's optical-face drawing places the green-marked cathode at terminal 2 and the anode at terminal 1. Rotate that drawing 90 degrees counterclockwise to the footprint's top/component view: cathode left, anode right. The reverse-face drawing is not used to infer left/right placement.

Symbol: `RheoBoard_LEDs:SZYY0603R`. Two footprint variants retain the existing board's land geometry:

| Lead | Function | Symbol pin / type / local XY mm | Footprint pad / local XY mm | View / evidence |
|---|---|---|---|---|
| 1 | Anode (+) | 1 / A / passive / (+3.81, 0) | Nominal: 1 / (+0.7875, 0); HandSolder: 1 / (+0.875, 0) | Top/component view, anode right; manufacturer p2 positive terminal 1 |
| 2 | Cathode (-) | 2 / K / passive / (-3.81, 0) | Nominal: 2 / (-0.7875, 0); HandSolder: 2 / (-0.875, 0) | Top/component view, marked cathode left; manufacturer p2 negative terminal 2 |

Footprint IDs: `RheoBoard:LED_SZYY0603R_A1_K2_Nominal` and `RheoBoard:LED_SZYY0603R_A1_K2_HandSolder`. Nominal pads are 0.875 × 0.95 mm; HandSolder pads are 1.05 × 0.95 mm. Both retain a 0.70 mm gap and 25% rounded corners. These are enlarged compatible lands, not identical to the manufacturer's suggested 0.70 mm lands. Pads have F.Cu, F.Paste and F.Mask, with no drills. Body is 1.60 × 0.80 mm. The U-shaped silkscreen end bar and fab chamfer identify cathode/pad 2; they are polarity markings, not a generic pin-1 dot.

Counts reconcile: two physical leads, two symbol pins, two electrical pads per variant; zero duplicate pads and zero mechanical-only pads. Library queries and the disposable placement IPC-2581 export confirmed pin numbers, positions and pad sizes. Disposable schematic and board SVG renders were inspected for symbol polarity, top-view orientation, land geometry, cathode marking and courtyard. The schematic symbol's cathode bar is on pin 2.

The production migration swaps symbol and footprint numbering together. Physical cathode/anode positions and connected nets must remain unchanged. See the manufacturing LED assembly guide for per-reference polarity targets. Supplier rotation conventions still require the normal placement-preview check; no unverified 180-degree CPL offset is applied.
