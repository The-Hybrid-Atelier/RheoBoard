# Laser-cut panel (archived, version 1)

> **Archived.** RheoBoard no longer uses a laser-cut panel. Version 2 uses a 3D-printed panel;
> see [`../../cad/encloser/README.md`](../../cad/encloser/README.md). These files are the
> laser-cut panel of version 1, the build certified by OSHWA as US002865. They are kept for
> reference only; the full version 1 documentation is at the `v1` git tag:
> https://github.com/The-Hybrid-Atelier/RheoBoard/tree/v1

Use [`panel.svg`](panel.svg) or [`panel.dxf`](panel.dxf) to cut the mounting panel for the
rheometer components.

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

Version 1 placement: both pumps are rotated 90° with VALVE2 centered between them.
The two-pump L298N is on the left, the valve L298N is on the right, MPRLS is directly below
VALVE2, and the seesaw, ESP32, and Button are behind the drivers. PWR, VALVE1, and the former
chamber/bulkhead hole are removed.

## Specifications

- Material: 3 mm acrylic
- Finished size: 230 × 200 mm
- Mounting: zip ties through the cut slots; four feet at the corner holes

Physical envelopes used for version 1 placement:

- Pumps: Adafruit 4699 / ZR370-02PM, **58.2 × Ø27.0 mm nominal**; the placement outline uses
  the tolerance-max 58.3 × 27.2 mm body, rotated 90°
- VALVE2: Adafruit 4663 / FA0520E, **36.02 × 14.5 mm** top-view envelope
- ATtiny1616 seesaw: **25.5 × 17.8 mm**
- ESP32 Thing Plus WRL-15663: **64.77 × 22.86 mm**
- SparkFun MPRLS and Qwiic Button: **25.4 × 25.4 mm** each
- L298N modules: retained placeholder outlines because generic module dimensions vary; test-fit
  the exact boards before cutting

The pump, valve, and MPRLS envelopes do not reserve pneumatic tube bend radius. Confirm port and tubing
clearance when you place parts on the paper pattern.

## Files

- [`panel.svg`](panel.svg) — vector file for most laser-cutting software
- [`panel.dxf`](panel.dxf) — alternate vector file for software that requires DXF
- [`panel-placement-map.png`](panel-placement-map.png) — component positions and zip-tie routing
- [`panel-cut-lines.png`](panel-cut-lines.png) — visual reference for the cut geometry

## Cut the panel

1. Open `panel.svg` or `panel.dxf` in the laser-cutter software.
2. Confirm the imported panel measures exactly **230 × 200 mm**. Do not scale it.
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
2. Check the overall dimensions, corner holes, and every zip-tie slot.
3. Install the four feet.
4. Position components using `panel-placement-map.png`; version 1 has no VALVE1 or chamber position.
5. Loosely install the zip ties, confirm cable and tube clearance, then tighten and trim them.
6. Continue with version 1
   [Step 02 — Connect electronics and tubing](https://github.com/The-Hybrid-Atelier/RheoBoard/blob/v1/BuildYourOwn/README.md#step-02-connect-electronics-and-tubing).

## Modify or regenerate the design

Edit [`generate_panel_vector.py`](generate_panel_vector.py), then regenerate both vector formats:

```bash
python3 generate_panel_vector.py
```

Do not hand-edit only one export; `panel.svg` and `panel.dxf` must remain synchronized.
