#!/usr/bin/env python3
"""Generate panel.svg / panel.dxf -- vector cut geometry for the laser-cut platform.

SPDX-License-Identifier: CERN-OHL-W-2.0
Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE-HARDWARE.txt

WHAT THIS IS: a vector trace of panel-cut-lines.png (the pre-existing raster design
reference provided alongside this build), converted to real mm coordinates and proper vector
primitives (circles, rounded-rectangle slots) instead of a flat image. panel-cut-lines.png's
1024x706px canvas maps 1:1 onto the panel's 290x200mm bounds (matching aspect ratio, no
margin), so pixel position converts directly to mm position -- see `retrace()` below for the
extraction method (connected-component analysis on non-white pixels).

WHAT THIS IS NOT: independently verified against physical parts, and not a fine-grained,
slot-by-slot part attribution. Every hole/slot below is positioned exactly where
panel-cut-lines.png draws it (this part is objective, reproducible via --retrace). But WHICH
named part each slot belongs to is only established at coarse column/row granularity by eye
against panel-placement-map.png -- it has not been re-derived slot-by-slot with certainty.
Corner holes and the chamber bulkhead hole are high-confidence (unambiguous, isolated shapes,
cross-checked against panel-placement-map.png's CHAMBER marker position). The ~50 zip-tie slots
are grouped into rough columns matching the placement map's left-to-right layout, not labeled
per-part -- cross-reference ../laser-cut/panel-placement-map.png visually if you need to know
exactly which slot belongs to which component.

**Do not cut material from this file without test-fitting real parts first** -- see
../VERIFICATION.md and AGENTS.md -> "What the agent can and can't verify."

Usage:
    python3 generate_panel_vector.py            # write panel.svg + panel.dxf from the traced
                                                   geometry table baked into this script
    python3 generate_panel_vector.py --retrace   # re-run pixel extraction against
                                                   panel-cut-lines.png and print a fresh
                                                   geometry table for review (requires
                                                   numpy/scipy/Pillow; does not overwrite the
                                                   table below automatically)
"""

import sys

PANEL_W_MM = 290.0
PANEL_H_MM = 200.0
PANEL_CORNER_R_MM = 3.0

# ---- Traced geometry (mm, origin = panel top-left, +x right, +y down) -------
# All values below came directly out of a --retrace run against panel-cut-lines.png; nothing
# here is invented or estimated by hand. See module docstring for what is/isn't verified.

# (x, y, diameter) -- 4 corner mounting holes (feet), ~10mm inset from each edge
CORNER_HOLES_MM = [
    (10.05, 10.06, 5.95),
    (279.95, 10.06, 5.95),
    (10.05, 189.94, 5.95),
    (279.95, 189.94, 5.95),
]

# (x, y, diameter) -- panel-mount bulkhead fitting for the chamber connection (labeled "Ø10" in
# BOM/README -- that's the fitting's own spec, not necessarily this panel-hole diameter; confirm
# against the actual fitting's datasheet before cutting)
CHAMBER_HOLE_MM = (261.96, 60.06, 12.46)

# Zip-tie slots: (x, y, w, h) mm, rounded-rectangle. Grouped into rough columns matching
# panel-placement-map.png's left-to-right layout -- NOT a verified per-part breakdown, see
# module docstring.
SLOT_COLUMNS_MM = {
    "column A (x~34-66mm -- pumps + L298N #1, per placement map)": [
        (34.13, 19.55, 10.48, 5.67), (66.13, 19.55, 10.48, 5.67),
        (33.98, 52.55, 10.20, 5.38), (65.99, 52.55, 10.20, 5.38),
        (33.98, 71.53, 10.20, 5.38), (65.99, 71.53, 10.20, 5.38),
        (33.98, 104.53, 10.76, 5.67), (65.99, 104.53, 10.76, 5.67),
        (37.10, 123.51, 9.63, 5.67), (62.87, 123.51, 9.63, 5.67),
        (37.10, 172.52, 9.63, 5.67), (62.87, 172.52, 9.63, 5.67),
    ],
    "column B (x~106-134mm -- valves + L298N #2, per placement map)": [
        (109.03, 23.65, 9.63, 5.38), (130.98, 23.80, 9.35, 5.67),
        (106.20, 48.30, 3.96, 5.38), (112.01, 48.30, 3.68, 5.38),
        (128.15, 48.30, 3.68, 5.38), (133.67, 48.30, 3.96, 5.38),
        (109.03, 75.78, 9.63, 5.38), (130.98, 75.78, 9.35, 5.38),
        (109.03, 100.28, 9.63, 5.67), (130.98, 100.28, 9.35, 5.67),
        (115.97, 110.06, 4.81, 7.65), (124.04, 109.92, 5.10, 7.37),
        (107.19, 123.37, 9.35, 5.38), (132.82, 123.37, 9.63, 5.38),
        (107.19, 172.52, 9.35, 5.67), (132.82, 172.52, 9.63, 5.67),
    ],
    "column C (x~46-55mm -- narrow paired slot, same shape as the likely T-connector-node "
    "pairs at y~48/110/154mm elsewhere on this panel)": [
        (46.02, 109.92, 4.81, 7.37), (54.66, 110.06, 3.96, 7.65),
    ],
    "column D (x~122-260mm -- MPRLS/BUTTON/ESP32 area, per placement map)": [
        (122.06, 53.97, 8.50, 5.38), (195.98, 53.97, 8.50, 5.38),
        (122.06, 65.86, 8.50, 5.38), (195.98, 66.01, 8.50, 5.67),
        (180.26, 76.91, 5.38, 9.35), (211.69, 76.91, 5.38, 9.35),
        (227.27, 76.91, 5.38, 9.35), (258.71, 77.05, 5.38, 9.07),
        (180.26, 91.08, 5.38, 9.35), (211.69, 91.08, 5.38, 9.35),
        (227.27, 91.22, 5.38, 9.07), (258.71, 91.08, 5.38, 9.35),
        (188.47, 129.60, 9.35, 5.38), (223.59, 129.60, 9.35, 5.38),
        (161.00, 154.11, 4.81, 7.37), (169.07, 153.97, 5.10, 7.08),
        (188.47, 158.36, 9.35, 5.67), (223.31, 158.36, 9.35, 5.67),
    ],
    "column E (x~249-267mm -- PWR terminal block area, per placement map)": [
        (248.94, 171.95, 5.66, 8.50), (267.06, 171.95, 5.66, 8.50),
    ],
}


def build_svg():
    parts = []
    parts.append(
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{PANEL_W_MM}mm" height="{PANEL_H_MM}mm" '
        f'viewBox="0 0 {PANEL_W_MM} {PANEL_H_MM}">'
    )
    parts.append(
        f'<rect x="0" y="0" width="{PANEL_W_MM}" height="{PANEL_H_MM}" rx="{PANEL_CORNER_R_MM}" '
        f'fill="none" stroke="black" stroke-width="0.2"/>'
    )
    for x, y, d in CORNER_HOLES_MM:
        parts.append(f'<circle cx="{x}" cy="{y}" r="{d/2}" fill="none" stroke="red" stroke-width="0.2"/>')
    cx, cy, cd = CHAMBER_HOLE_MM
    parts.append(f'<circle cx="{cx}" cy="{cy}" r="{cd/2}" fill="none" stroke="red" stroke-width="0.2"/>')
    for label, slots in SLOT_COLUMNS_MM.items():
        parts.append(f'<g data-column="{label}">')
        for x, y, w, h in slots:
            rx = min(w, h) / 2.2
            parts.append(
                f'<rect x="{x - w/2:.2f}" y="{y - h/2:.2f}" width="{w:.2f}" height="{h:.2f}" '
                f'rx="{rx:.2f}" fill="none" stroke="red" stroke-width="0.2"/>'
            )
        parts.append('</g>')
    parts.append('</svg>')
    return "\n".join(parts)


def build_dxf():
    # Minimal hand-written ASCII DXF (R12-compatible: LWPOLYLINE + CIRCLE only). No external
    # dependency (e.g. ezdxf) needed -- keeps this script dependency-free besides the optional
    # --retrace path (numpy/scipy/Pillow, only needed to re-run the trace from the PNG).
    lines = ["0", "SECTION", "2", "ENTITIES"]

    def circle(x, y, d):
        lines.extend(["0", "CIRCLE", "8", "CUT", "10", f"{x}", "20", f"{-y}", "30", "0.0",
                      "40", f"{d/2}"])

    def rounded_rect(x, y, w, h, rx):
        # 8-point polyline approximation (straight corner cuts, not true arcs) -- adequate for
        # a zip-tie slot; swap in real arc segments in CAD later if sharper corners matter.
        x0, y0, x1, y1 = x - w/2, y - h/2, x + w/2, y + h/2
        pts = [(x0+rx, y0), (x1-rx, y0), (x1, y0+rx), (x1, y1-rx),
               (x1-rx, y1), (x0+rx, y1), (x0, y1-rx), (x0, y0+rx)]
        lines.extend(["0", "LWPOLYLINE", "8", "CUT", "90", str(len(pts)), "70", "1"])
        for px, py in pts:
            lines.extend(["10", f"{px}", "20", f"{-py}"])

    rounded_rect(PANEL_W_MM / 2, PANEL_H_MM / 2, PANEL_W_MM, PANEL_H_MM, PANEL_CORNER_R_MM)
    for x, y, d in CORNER_HOLES_MM:
        circle(x, y, d)
    circle(*CHAMBER_HOLE_MM)
    for slots in SLOT_COLUMNS_MM.values():
        for x, y, w, h in slots:
            rounded_rect(x, y, w, h, min(w, h) / 2.2)

    lines.extend(["0", "ENDSEC", "0", "EOF"])
    return "\n".join(lines)


def retrace():
    """Re-run pixel extraction against panel-cut-lines.png; prints a fresh table for manual
    review. Does NOT auto-update SLOT_COLUMNS_MM above -- re-grouping pixel blobs into named
    columns/parts needs a human (or a fresh agent pass with the placement map open) to check,
    not a blind overwrite."""
    import numpy as np
    from PIL import Image
    from scipy import ndimage

    im = Image.open("panel-cut-lines.png").convert("RGBA")
    arr = np.array(im)
    rgb = arr[:, :, :3].astype(int)
    alpha = arr[:, :, 3]
    is_white = np.all(rgb > 240, axis=-1)
    mask = (~is_white) & (alpha > 5)
    mask_d = ndimage.binary_dilation(mask, structure=np.ones((5, 5)), iterations=2)
    labeled, n = ndimage.label(mask_d)
    objs = ndimage.find_objects(labeled)
    sx, sy = PANEL_W_MM / im.width, PANEL_H_MM / im.height
    print(f"# {n} raw components, image {im.size}px, scale ({sx:.4f}, {sy:.4f}) mm/px")
    kept = 0
    for i, sl in enumerate(objs):
        ys, xs = sl
        w, h = xs.stop - xs.start, ys.stop - ys.start
        npix = (labeled[sl] == i + 1).sum()
        if npix < 180:
            continue  # drops the 4 tiny corner registration marks (~4x4mm, not real features)
        kept += 1
        cx, cy = (xs.start + xs.stop) / 2, (ys.start + ys.stop) / 2
        print(f"  ({cx*sx:.2f}, {cy*sy:.2f})  size=({w*sx:.2f}, {h*sy:.2f})mm  npix={npix}")
    print(f"# {kept} kept after filtering tiny marks")


if __name__ == "__main__":
    if "--retrace" in sys.argv:
        retrace()
        sys.exit(0)
    with open("panel.svg", "w") as f:
        f.write(build_svg())
    with open("panel.dxf", "w") as f:
        f.write(build_dxf())
    print("Wrote panel.svg and panel.dxf")
