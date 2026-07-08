# Step 02: Assemble the platform

- **Time:** ~30–45 min hands-on (excludes laser-cutting turnaround, which can be days if
  outsourced to a cut-to-order service)
- **Difficulty:** requires laser cutter access (or a cut-to-order service); assembly itself is
  hand tools only (zip ties, no screws)

## What you'll need for this step

- **Parts:** acrylic panel, zip ties (~20), Ø10 bulkhead fitting, 4× rubber/plastic feet — see
  [`../../../BOM.md`](../../../BOM.md).
- **Design files:** [`../../../laser-cut/`](../../../laser-cut/) — cut the panel first if not
  pre-cut. **Note:** as of this writing only a raster design reference exists there (placement
  map + cut-geometry preview); the laser-ready vector file (`.svg`/`.dxf`) still needs to be
  produced from `laser-cut/panel-cut-lines.png` before this step can actually be cut.
- **Tools:** small zip-tie cutters/flush cutters, laser cutter (or cut-to-order service).

## Instructions

1. Cut the panel per [`laser-cut/panel-cut-lines.png`](../../../laser-cut/panel-cut-lines.png)
   (290 × 200 × 3 mm acrylic) — once the vector file exists.
2. Attach the 4 corner feet.
3. Install the Ø10 bulkhead fitting at the CHAMBER position.
4. Place and zip-tie each component per the numbered map in
   [`laser-cut/panel-placement-map.png`](../../../laser-cut/panel-placement-map.png) and the table
   in [`laser-cut/README.md`](../../../laser-cut/README.md#component-placement--zip-tie-map):
   PUMP1/PUMP2 lying flat, VALVE2 (leave the VALVE1 slot empty — unpopulated in this 2P1V build),
   MPRLS + Button next to the ESP32, both L298N boards clear of their heatsinks, ESP32, and the
   power terminal block.
5. Don't wire anything yet — this step is mechanical placement only. Electrical wiring is
   [step 03](../03-wire-electronics/).

## Media

Pictographic or photo sequence strongly recommended (one photo per sub-step).
`laser-cut/panel-placement-map.png` doubles as the primary pictographic reference here.

## Tips / common mistakes

- Leave the VALVE1 zip-tie slot empty — it's a reserved position for a future 2-valve (2P2V)
  variant, not part of this build. See `laser-cut/README.md` for why.
- Zip-tie snug but not so tight it deforms the pump/valve housings.

## Check before moving on

- [ ] Platform is rigid and square — no wobble that would affect measurements
- [ ] All mechanical parts from this step's BOM rows are installed
- [ ] Component positions match `laser-cut/panel-placement-map.png` (right components, right
      orientation, VALVE1 slot left empty)
