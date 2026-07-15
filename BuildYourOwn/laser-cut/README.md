# Laser-cut panel

Use [`panel.svg`](panel.svg) or [`panel.dxf`](panel.dxf) to cut the mounting panel for the
rheometer components.

License: CERN-OHL-W-2.0 — see [`../../LICENSE`](../../LICENSE).

> **Draft — test-fit before cutting the final acrylic.** The panel has not been physically cut or
> verified against real parts. In particular, confirm every zip-tie slot and the ATtiny1616 seesaw
> placement.

## Specifications

- Material: 3 mm acrylic
- Finished size: 290 × 200 mm
- Chamber fitting: Ø10 mm panel-mount bulkhead
- Mounting: zip ties through the cut slots; four feet at the corner holes

## Files

- [`panel.svg`](panel.svg) — vector file for most laser-cutting software
- [`panel.dxf`](panel.dxf) — alternate vector file for software that requires DXF
- [`panel-placement-map.png`](panel-placement-map.png) — component positions and zip-tie routing
- [`panel-cut-lines.png`](panel-cut-lines.png) — visual reference for the cut geometry

## Cut the panel

1. Open `panel.svg` or `panel.dxf` in the laser-cutter software.
2. Confirm the imported panel measures exactly **290 × 200 mm**. Do not scale it.
3. Print the design at 1:1 scale on paper or make a low-cost cardboard test cut.
4. Place every real component over the test pattern and verify its slots against
   [`panel-placement-map.png`](panel-placement-map.png).
5. Run a kerf test on scrap 3 mm acrylic. Apply the measured compensation in the cutter software.
6. Use the machine manufacturer's acrylic power, speed, focus, ventilation, and fire-safety
   guidance. Never cut PVC or unknown plastic.
7. Cut the panel and let it cool before handling.

Power, speed, and kerf values are intentionally not specified because they depend on the laser and
acrylic. Record the verified settings after the first successful cut.

## After cutting

1. Remove the protective film and clean any sharp or melted edges.
2. Check the overall dimensions, corner holes, Ø10 mm chamber hole, and every zip-tie slot.
3. Install the four feet and chamber fitting.
4. Position components using `panel-placement-map.png`; leave the unused **VALVE1** position empty.
5. Loosely install the zip ties, confirm cable and tube clearance, then tighten and trim them.
6. Continue with
   [Step 02 — Connect electronics and tubing](../README.md#step-02-connect-electronics-and-tubing).

## Modify or regenerate the design

Edit [`generate_panel_vector.py`](generate_panel_vector.py), then regenerate both vector formats:

```bash
python3 generate_panel_vector.py
```

Do not hand-edit only one export; `panel.svg` and `panel.dxf` must remain synchronized.
