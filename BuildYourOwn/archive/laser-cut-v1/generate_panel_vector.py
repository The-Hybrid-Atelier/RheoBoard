#!/usr/bin/env python3
"""Generate panel.svg / panel.dxf -- vector cut geometry for the panel.

SPDX-License-Identifier: CERN-OHL-W-2.0
Copyright (c) 2026 Charlie Vuong -- see ../../LICENSE

WHAT THIS IS: source geometry for the panel, expressed in real millimeter coordinates
and exported as SVG, DXF, and proportional PNG references. The geometry is maintained
directly in the tables below.

GEOMETRY ORIGIN: The earlier geometry was a trace of panel-cut-lines.png. The tables below are an explicit
component-driven relayout: both pumps are rotated 90 degrees with VALVE2 centered between them;
the two-pump L298N is left and the valve L298N is right; MPRLS sits directly below VALVE2;
the seesaw, ESP32, and Button form a rear row; and the former chamber/bulkhead hole is removed.

Usage:
    python3 generate_panel_vector.py            # write panel.svg + panel.dxf from the geometry
                                                   table baked into this script
    python3 generate_panel_vector.py --rasters  # also rebuild both PNG references from source;
                                                   requires Pillow
    python3 generate_panel_vector.py --retrace  # re-run pixel extraction against
                                                   panel-cut-lines.png and print a fresh
                                                   geometry table for review (requires
                                                   numpy/scipy/Pillow; does not overwrite the
                                                   table below automatically)
"""

import sys

PANEL_W_MM = 230.0
PANEL_H_MM = 200.0
PANEL_CORNER_R_MM = 3.0

# ---- Traced geometry (mm, origin = panel top-left, +x right, +y down) -------
# Most retained values came from a --retrace run against panel-cut-lines.png. Version 1
# removals, rotations, and relocated modules are explicit design changes; see the module docstring.

# (x, y, diameter) -- 4 corner mounting holes (feet), ~10mm inset from each edge
CORNER_HOLES_MM = [
    (10.05, 10.06, 5.95),
    (PANEL_W_MM - 10.05, 10.06, 5.95),
    (10.05, 189.94, 5.95),
    (PANEL_W_MM - 10.05, 189.94, 5.95),
]

# Version 1 does not mount the chamber or a chamber bulkhead on this panel.
CHAMBER_HOLE_MM = None

# Zip-tie slots: (x, y, w, h) mm, rounded rectangles. Version 1 is organized by component:
# both pumps are rotated 90 degrees with VALVE2 between them; the pump driver is left and the
# valve driver is right; MPRLS sits below the valve; the remaining modules form a rear row.
SLOT_COLUMNS_MM = {
    "pump1": [
        (30.00, 29.00, 5.60, 10.40), (70.00, 29.00, 5.60, 10.40),
        (30.00, 61.00, 5.60, 10.40), (70.00, 61.00, 5.60, 10.40),
    ],
    "valve2": [
        (109.00, 32.50, 9.60, 5.40), (131.00, 32.50, 9.60, 5.40),
        (109.00, 57.50, 9.60, 5.40), (131.00, 57.50, 9.60, 5.40),
    ],
    "pump2": [
        (170.00, 29.00, 5.60, 10.40), (210.00, 29.00, 5.60, 10.40),
        (170.00, 61.00, 5.60, 10.40), (210.00, 61.00, 5.60, 10.40),
    ],
    "l298n1": [
        (52.00, 95.00, 9.60, 5.60), (78.00, 95.00, 9.60, 5.60),
        (52.00, 135.00, 9.60, 5.60), (78.00, 135.00, 9.60, 5.60),
    ],
    "l298n2": [
        (177.00, 95.00, 9.60, 5.60), (203.00, 95.00, 9.60, 5.60),
        (177.00, 135.00, 9.60, 5.60), (203.00, 135.00, 9.60, 5.60),
    ],
    "seesaw": [
        (73.00, 158.00, 9.35, 5.40), (97.00, 158.00, 9.35, 5.40),
        (73.00, 182.00, 9.35, 5.40), (97.00, 182.00, 9.35, 5.40),
    ],
    "mprls": [
        (104.00, 80.00, 5.40, 9.35), (136.00, 80.00, 5.40, 9.35),
        (104.00, 100.00, 5.40, 9.35), (136.00, 100.00, 5.40, 9.35),
    ],
    "button": [
        (179.00, 160.00, 5.40, 9.35), (211.00, 160.00, 5.40, 9.35),
        (179.00, 180.00, 5.40, 9.35), (211.00, 180.00, 5.40, 9.35),
    ],
    "esp32": [
        (122.00, 152.00, 9.35, 5.40), (158.00, 152.00, 9.35, 5.40),
        (122.00, 188.00, 9.35, 5.40), (158.00, 188.00, 9.35, 5.40),
    ],
}

# Footprint used by the placement-map overlay (mm). Centered in the column-F slot rectangle.
SEESAW_FOOTPRINT_MM = {
    "label": "6 SEESAW",
    "center": (85.00, 170.00),
    "size": (25.50, 17.80),  # Adafruit PID 5690 physical board outline
    "slots": "seesaw",
}

# Physical top-view envelopes used by the placement map. Dimensions are manufacturer nominal
# values in millimeters; slot spacing adds clearance outside each envelope. L298N modules are
# intentionally excluded because generic module dimensions vary and the user requested that
# their existing placeholder footprints remain unchanged.
PHYSICAL_ENVELOPES_MM = {
    "pump": (27.20, 58.30),       # Adafruit 4699 / ZR370-02PM tolerance-max body (W × H)
    "valve": (36.32, 14.80),      # Adafruit 4663 / FA0520E tolerance-max body
    "seesaw": (25.50, 17.80),     # Adafruit PID 5690
    "mprls": (25.40, 25.40),      # SparkFun SEN-16476, 1 × 1 inch
    "button": (25.40, 25.40),     # SparkFun BOB-15932, 1 × 1 inch
    "esp32": (64.77, 22.86),      # SparkFun WRL-15663, 2.55 × 0.9 inch
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
    if CHAMBER_HOLE_MM:
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
    if CHAMBER_HOLE_MM:
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


def _draw_slot_ellipse(draw, cx, cy, w, h, fill, outline=None, width=1):
    """Draw a filled ellipse representing one zip-tie slot (axis-aligned)."""
    box = [cx - w / 2, cy - h / 2, cx + w / 2, cy + h / 2]
    draw.ellipse(box, fill=fill, outline=outline, width=width)


def update_rasters():
    """Refresh panel-cut-lines.png and panel-placement-map.png for the version 1 layout.

    Both PNG files are rendered from scratch from the source geometry, so removed features cannot
    linger and repeated runs are idempotent.
    """
    from PIL import Image, ImageDraw, ImageFont

    try:
        font_tiny = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 12)
        font_body = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 14)
        font_label = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 16)
        font_bold = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 16, index=1)
        font_title = ImageFont.truetype("/System/Library/Fonts/Helvetica.ttc", 19, index=1)
    except OSError:
        font_tiny = font_body = font_label = font_bold = font_title = ImageFont.load_default()

    # ---- panel-cut-lines.png ------------------------------------------------ #
    cut_height = 706
    cut_width = round(cut_height * PANEL_W_MM / PANEL_H_MM)
    cut = Image.new("RGB", (cut_width, cut_height), "white")
    draw_c = ImageDraw.Draw(cut)
    sx_c, sy_c = cut.width / PANEL_W_MM, cut.height / PANEL_H_MM

    def cut_box(cx, cy, w, h):
        return [
            (cx - w / 2) * sx_c,
            (cy - h / 2) * sy_c,
            (cx + w / 2) * sx_c,
            (cy + h / 2) * sy_c,
        ]

    draw_c.rounded_rectangle(
        [1, 1, cut.width - 2, cut.height - 2],
        radius=PANEL_CORNER_R_MM * sx_c,
        outline=(30, 55, 85),
        width=2,
    )
    for x, y, d in CORNER_HOLES_MM:
        draw_c.ellipse(cut_box(x, y, d, d), outline=(220, 40, 40), width=2)
    if CHAMBER_HOLE_MM:
        x, y, d = CHAMBER_HOLE_MM
        draw_c.ellipse(cut_box(x, y, d, d), outline=(220, 40, 40), width=2)
    for slots in SLOT_COLUMNS_MM.values():
        for x, y, w, h in slots:
            draw_c.rounded_rectangle(
                cut_box(x, y, w, h),
                radius=min(w * sx_c, h * sy_c) / 2.2,
                outline=(220, 40, 40),
                width=2,
            )
    cut.quantize(colors=32).save("panel-cut-lines.png", optimize=True)
    print("Updated panel-cut-lines.png (clean version 1 geometry)")

    # ---- panel-placement-map.png ------------------------------------------- #
    place = Image.new("RGB", (1120, 700), (248, 250, 252))
    draw_p = ImageDraw.Draw(place)
    ox, oy, scale = 30.0, 45.0, 3.05

    def mm_to_place(x_mm, y_mm):
        return ox + x_mm * scale, oy + y_mm * scale

    panel_box = [ox, oy, ox + PANEL_W_MM * scale, oy + PANEL_H_MM * scale]
    draw_p.rounded_rectangle(
        panel_box,
        radius=PANEL_CORNER_R_MM * scale,
        fill=(255, 255, 255),
        outline=(35, 55, 80),
        width=3,
    )
    for x, y, d in CORNER_HOLES_MM:
        px, py = mm_to_place(x, y)
        r = d * scale / 2
        draw_p.ellipse([px - r, py - r, px + r, py + r], outline=(220, 40, 40), width=2)

    components = [
        ("1", "PUMP1", (50, 45), PHYSICAL_ENVELOPES_MM["pump"], "pump1", (211, 245, 220), (55, 145, 85)),
        ("2", "VALVE2", (120, 45), PHYSICAL_ENVELOPES_MM["valve"], "valve2", (214, 231, 255), (55, 100, 190)),
        ("3", "PUMP2", (190, 45), PHYSICAL_ENVELOPES_MM["pump"], "pump2", (211, 245, 220), (55, 145, 85)),
        ("4", "L298N #1", (65, 115), (42, 30), "l298n1", (255, 239, 190), (170, 105, 25)),
        ("5", "L298N #2", (190, 115), (42, 30), "l298n2", (255, 239, 190), (170, 105, 25)),
        ("6", "SEESAW", (85, 170), PHYSICAL_ENVELOPES_MM["seesaw"], "seesaw", (255, 231, 165), (155, 105, 20)),
        ("7", "MPRLS", (120, 90), PHYSICAL_ENVELOPES_MM["mprls"], "mprls", (235, 220, 255), (110, 55, 165)),
        ("8", "BUTTON", (195, 170), PHYSICAL_ENVELOPES_MM["button"], "button", (235, 220, 255), (110, 55, 165)),
        ("9", "ESP32", (140, 170), PHYSICAL_ENVELOPES_MM["esp32"], "esp32", (211, 245, 220), (55, 145, 85)),
    ]

    def centered_text(center, text, font, fill):
        bbox = draw_p.textbbox((0, 0), text, font=font)
        width = bbox[2] - bbox[0]
        height = bbox[3] - bbox[1]
        draw_p.text((center[0] - width / 2, center[1] - height / 2), text, font=font, fill=fill)

    for number, label, center_mm, size_mm, group, fill, outline in components:
        cx, cy = mm_to_place(*center_mm)
        width, height = size_mm[0] * scale, size_mm[1] * scale
        draw_p.rounded_rectangle(
            [cx - width / 2, cy - height / 2, cx + width / 2, cy + height / 2],
            radius=6,
            fill=fill,
            outline=outline,
            width=2,
        )
        centered_text((cx, cy), label, font_label if len(label) < 8 else font_body, outline)

        if number:
            nx, ny = cx - width / 2 - 11, cy - height / 2 - 8
            draw_p.ellipse([nx - 9, ny - 9, nx + 9, ny + 9], fill=(20, 30, 45))
            centered_text((nx, ny - 1), number, font_tiny, "white")

        slots = SLOT_COLUMNS_MM[group]
        if len(slots) == 4:
            pairs = (
                ((0, 1), (2, 3))
                if group in ("pump1", "pump2", "mprls", "button")
                else ((0, 2), (1, 3))
            )
            for left_index, right_index in pairs:
                start = mm_to_place(slots[left_index][0], slots[left_index][1])
                end = mm_to_place(slots[right_index][0], slots[right_index][1])
                for step in range(0, 8, 2):
                    t0, t1 = step / 8, (step + 1) / 8
                    draw_p.line(
                        [
                            (start[0] + (end[0] - start[0]) * t0,
                             start[1] + (end[1] - start[1]) * t0),
                            (start[0] + (end[0] - start[0]) * t1,
                             start[1] + (end[1] - start[1]) * t1),
                        ],
                        fill=(205, 45, 45),
                        width=2,
                    )
        elif len(slots) == 2:
            start = mm_to_place(slots[0][0], slots[0][1])
            end = mm_to_place(slots[1][0], slots[1][1])
            draw_p.line([start, end], fill=(205, 45, 45), width=2)

        for x, y, w, h in slots:
            px, py = mm_to_place(x, y)
            draw_p.rounded_rectangle(
                [
                    px - w * scale / 2,
                    py - h * scale / 2,
                    px + w * scale / 2,
                    py + h * scale / 2,
                ],
                radius=min(w, h) * scale / 2.2,
                fill=(255, 240, 240),
                outline=(220, 40, 40),
                width=2,
            )

    legend_x = 760
    draw_p.text((legend_x, 48), "VERSION 1 PLACEMENT", font=font_title, fill=(20, 30, 45))
    draw_p.text((legend_x, 76), "Top view — front at top", font=font_body, fill=(80, 90, 105))
    legend_lines = [
        "1  PUMP1 — rotated 90°",
        "2  VALVE2 — between both pumps",
        "3  PUMP2 — rotated 90°",
        "4  L298N #1 — controls both pumps",
        "5  L298N #2 — controls VALVE2",
        "6  SEESAW — rear row",
        "7  MPRLS — directly below VALVE2",
        "8  Qwiic Button — beside ESP32",
        "9  ESP32 — rear row",
        "",
        "PHYSICAL ENVELOPES",
        "Pump: 58.3 × 27.2 mm tolerance-max",
        "Valve: 36.32 × 14.8 mm tolerance-max",
        "Seesaw: 25.5 × 17.8 mm",
        "ESP32: 64.77 × 22.86 mm",
        "MPRLS / Button: 25.4 × 25.4 mm",
        "",
        "No VALVE1 mounting position.",
        "No chamber or bulkhead hole.",
        "",
        "Red outlines are laser-cut slots.",
        "Red dashes show zip-tie paths.",
        "Keep L298N heatsinks unobstructed.",
        "Non-L298N outlines use physical dimensions.",
        "L298N outlines remain placeholders.",
        "Placement only; wire per electrical schematic.",
    ]
    for index, text in enumerate(legend_lines):
        draw_p.text((legend_x, 112 + index * 21), text, font=font_body, fill=(35, 45, 60))

    place.quantize(colors=256).save("panel-placement-map.png", optimize=True)
    print("Updated panel-placement-map.png (requested version 1 component order)")


if __name__ == "__main__":
    if "--retrace" in sys.argv:
        retrace()
        sys.exit(0)
    with open("panel.svg", "w") as f:
        f.write(build_svg())
    with open("panel.dxf", "w") as f:
        f.write(build_dxf())
    print("Wrote panel.svg and panel.dxf")
    if "--rasters" in sys.argv:
        update_rasters()
