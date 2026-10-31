#!/usr/bin/env python3
"""Render the RheoBoardOTS electrical wiring diagram.

Three stacked sections, in the same infographic style as the earlier
published figure: Qwiic chain, control and loads, then power. This script
is the source of wiring-diagram.png. It does not draw the ATtiny1616
seesaw, and it does not draw the sparse one-sheet schematic.

SparkFun ESP32 Thing Plus. Wire.begin() uses SDA GPIO 23 and SCL GPIO 22.
Those header pins are the same I2C peripheral as the Qwiic connector.

Two Qwiic cables join the ESP32, Qwiic Button 0x6F and MicroPressure 0x18.
A Qwiic-to-jumper cable connects the sensor's spare port to PCA9685 0x40:
    Qwiic black GND → PCA9685 GND
    Qwiic red 3.3 V → PCA9685 VCC (logic only; never 12 V)
    Qwiic blue SDA → PCA9685 SDA (ESP32 GPIO 23 bus)
    Qwiic yellow SCL → PCA9685 SCL (ESP32 GPIO 22 bus)
Adafruit product 815 has no Qwiic socket. Leave address jumpers unset.
V+ and OE stay open. Product 815 has 220 ohm resistors in series with
its outputs.

PCA9685 channels, not ESP32 GPIOs. On this L298N, ENA enables OUT1/OUT2
and ENB enables OUT3/OUT4:
    8  → right ENA   valve, full-on / full-off, OUT1/OUT2
    9  → right ENB   PUMP2 pressure, OUT3/OUT4
    10 → left ENA    PUMP1 vacuum, OUT1/OUT2
    11 → not wired
Left ENB, IN3, and IN4 go to that module's GND. Left OUT3/OUT4 stay open.

Usage (from this folder):
    python3 wiring_diagram.py
    python3 wiring_diagram.py --outdir figs
"""
import argparse
from pathlib import Path

HERE = Path(__file__).resolve().parent

# 1 unit = 0.1 inch. figsize matches the limits so circles stay round.
W, H = 168.0, 156.0
FIG_W, FIG_H = 16.8, 15.6
DPI = 112

NAVY = "#10233F"
INK = "#1F2933"
MUTED = "#5C6A78"
SECTION_F = "#E5F6F7"
SECTION_E = "#6E989E"
BADGE = "#0E8A93"

ESP_F, ESP_E = "#D7EBFA", "#1D4F91"
QW_F, QW_E = "#F3E8FF", "#6D3A9B"
QW = "#8E44AD"
PCA_F, PCA_E = "#FFF3D6", "#C47D12"
TEAL = "#0F766E"
VCC = "#1B7A3A"
GND = "#2C333A"
CTRL = "#E07A12"
LOAD = "#2B86D6"
L298_F, L298_E = "#E3F6E6", "#1E7A34"
ACT_F, ACT_E = "#FFE4C4", "#E08A20"
PWR_F, PWR_E = "#FDE2E2", "#B42318"
POWER = "#C4312E"
SLATE_F, SLATE_E = "#F3F6F8", "#3E4C59"
BOARD_F, BOARD_E = "#FFF8EE", "#8A6232"
RULE_F, RULE_E = "#E7F1FB", "#1D4F91"
WARN_F, WARN_E = "#FDECEC", "#B42318"


def _use_font():
    import matplotlib.pyplot as plt
    from matplotlib import font_manager

    # Arial has the arrow and middle-dot used in the labels, plus a real bold.
    names = {f.name for f in font_manager.fontManager.ttflist}
    if "Arial" in names:
        plt.rcParams["font.family"] = "Arial"
    else:
        plt.rcParams["font.family"] = "DejaVu Sans"


def _render(out_path: Path, export_sections: bool = False) -> None:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.patches import Circle, FancyArrowPatch, FancyBboxPatch, Rectangle

    _use_font()
    fig, ax = plt.subplots(figsize=(FIG_W, FIG_H))
    fig.patch.set_facecolor("white")
    ax.set_xlim(0, W)
    ax.set_ylim(0, H)
    ax.set_aspect("equal")
    ax.axis("off")
    fig.subplots_adjust(left=0.0, right=1.0, top=1.0, bottom=0.0)

    def rounded(x, y, w, h, fc, ec, lw=2.2, radius=1.35, z=3, ls="solid"):
        ax.add_patch(FancyBboxPatch(
            (x, y), w, h,
            boxstyle=f"round,pad=0.02,rounding_size={radius}",
            linewidth=lw, edgecolor=ec, facecolor=fc, zorder=z, linestyle=ls,
        ))

    def card(x, y, w, h, fc, ec, lines, title_color=None, body=13.2, title=16.5,
             align="center", lw=2.3):
        """lines[0] is the bold title. Later lines are body copy."""
        rounded(x, y, w, h, fc, ec, lw=lw, radius=1.15, z=3)
        color0 = ec if title_color is None else title_color
        n = len(lines)
        pad = 1.35
        usable = max(h - 2 * pad, 1)
        step = usable / max(n - 1, 1) if n > 1 else 0
        step = min(step, 2.55)
        block = step * (n - 1)
        y0 = y + h / 2 + block / 2
        ha = "center" if align == "center" else "left"
        tx = x + w / 2 if align == "center" else x + 2.0
        for i, line in enumerate(lines):
            ax.text(
                tx, y0 - i * step, line,
                ha=ha, va="center", zorder=4,
                fontsize=title if i == 0 else body,
                fontweight="bold" if i == 0 else "regular",
                color=color0 if i == 0 else INK,
            )

    def wire(pts, color, lw=4.0, dashed=False):
        xs = [p[0] for p in pts]
        ys = [p[1] for p in pts]
        ax.plot(
            xs, ys, color=color, lw=lw, solid_capstyle="round",
            solid_joinstyle="round", zorder=2,
            linestyle=(0, (1.6, 1.5)) if dashed else "solid",
        )

    def node(x, y, color, ms=11):
        ax.plot([x], [y], marker="o", ms=ms, mfc=color, mec="white", mew=1.15, zorder=5)

    def tag(x, y, text, color, size=12.2, ha="center", va="center", weight="bold"):
        ax.text(x, y, text, ha=ha, va=va, fontsize=size, color=color,
                fontweight=weight, zorder=6)

    section_labels = []

    def section(x, y, w, h, number, title):
        rounded(x, y, w, h, SECTION_F, SECTION_E, lw=2.4, radius=1.8, z=0)
        cy = y + h - 4.6
        ax.add_patch(Circle((x + 5.4, cy), 2.65, fc=BADGE, ec="white", lw=1.5, zorder=5))
        badge_text = ax.text(x + 5.4, cy, str(number), ha="center", va="center", color="white",
                fontsize=18, fontweight="bold", zorder=6)
        title_text = ax.text(x + 9.6, cy, title, ha="left", va="center", color=NAVY,
                fontsize=24, fontweight="bold", zorder=4)
        section_labels.append((badge_text, title_text))
        return cy

    # ---- Title ----------------------------------------------------------
    ax.text(W / 2, 151.3, "RheoBoardOTS  ·  Electrical wiring",
            ha="center", va="center", fontsize=46, fontweight="bold", color=NAVY, zorder=4)
    ax.text(W / 2, 145.7, "Two pumps  ·  One valve  ·  USB-powered ESP32",
            ha="center", va="center", fontsize=18, color=MUTED, zorder=4)

    # ---- 1. Qwiic chain + PCA9685 breakout cable -----------------------
    s1x, s1y, s1w, s1h = 2.6, 100.2, 162.8, 42.6
    section(s1x, s1y, s1w, s1h, 1, "Qwiic chain")

    row_y, row_h = 123.6, 12.4
    row_cy = row_y + row_h / 2

    # USB plug into the ESP32. The label sits under the plug so it stays
    # clear of the section title.
    tag(12.2, row_cy - 3.35, "USB power", NAVY, size=12.5)
    ax.add_patch(FancyBboxPatch(
        (8.2, row_cy - 1.85), 6.4, 3.7,
        boxstyle="round,pad=0.02,rounding_size=0.45",
        fc="#1F2933", ec="#1F2933", lw=0.6, zorder=4,
    ))
    ax.add_patch(Rectangle((14.2, row_cy - 0.85), 1.7, 1.7, fc="#E5E7EB",
                           ec="#1F2933", lw=0.7, zorder=4))
    ax.add_patch(FancyArrowPatch(
        (16.4, row_cy), (24.6, row_cy),
        arrowstyle="-|>", mutation_scale=16, lw=2.4, color=NAVY,
        shrinkA=0, shrinkB=0, zorder=2,
    ))

    card(26.2, row_y, 30.5, row_h, ESP_F, ESP_E, [
        "ESP32 Thing Plus",
        "Qwiic connector",
        "3.3 V I2C bus",
    ], body=13.0, title=16.2)
    card(72.4, row_y, 32.0, row_h, QW_F, QW_E, [
        "Qwiic Button",
        "manual REP",
        "I2C 0x6F",
    ], body=13.0, title=16.2)
    card(120.2, row_y, 40.0, row_h, QW_F, QW_E, [
        "MicroPressure",
        "pressure sensor",
        "I2C 0x18",
    ], body=13.0, title=16.2)

    wire([(56.7, row_cy), (72.4, row_cy)], QW, lw=4.4)
    wire([(104.4, row_cy), (120.2, row_cy)], QW, lw=4.4)
    node(56.7, row_cy, QW)
    node(72.4, row_cy, QW)
    node(104.4, row_cy, QW)
    node(120.2, row_cy, QW)
    tag(64.55, row_cy + 1.55, "Qwiic cable", QW, size=12.4)
    tag(112.3, row_cy + 1.55, "Qwiic cable", QW, size=12.4)

    tag(84.0, 121.15,
        "2 Qwiic cables  +  1 Qwiic-to-jumper cable  ·  same 3.3 V I2C bus",
        INK, size=12.6, weight="regular")

    # The spare MicroPressure Qwiic port feeds the PCA9685 breakout cable.
    spine_x = 140.2
    taps = [
        (115.6, "Red  3.3 V → VCC", "#c4312e"),
        (112.2, "Black  GND", GND),
        (108.8, "Blue  SDA", "#1689c4"),
        (105.4, "Yellow  SCL", "#987700"),
    ]
    pca_x, pca_y, pca_w, pca_h = 26.2, 103.0, 67.5, 14.6
    spine = "#64748B"
    wire([(spine_x, row_y), (spine_x, taps[-1][0])], spine, lw=2.6)
    node(spine_x, row_y, spine, ms=10)
    for lane_y, label, color in taps:
        edge_x = pca_x + pca_w
        wire([(spine_x, lane_y), (edge_x, lane_y)], color, lw=3.5)
        node(spine_x, lane_y, color, ms=9)
        node(edge_x, lane_y, color, ms=9)
        tag((spine_x + edge_x) / 2, lane_y + 1.15, label, color, size=11.6)

    card(pca_x, pca_y, pca_w, pca_h, PCA_F, PCA_E, [
        "PCA9685  ·  product 815",
        "I2C 0x40  ·  no address jumpers",
        "V+ open  ·  OE open",
        "220 ohm resistors in series with outputs",
        "Qwiic-to-jumper cable from sensor",
    ], body=12.4, title=15.5)

    # ---- 2. Control and loads -------------------------------------------
    s2x, s2y, s2w, s2h = 2.6, 38.6, 162.8, 59.2
    section(s2x, s2y, s2w, s2h, 2, "Control and load connections")

    pca2_x, pca2_y, pca2_w, pca2_h = 6.2, 51.6, 34.0, 36.6
    l_x, l_w = 64.5, 46.5
    load_x, load_w, load_h = 126.8, 34.5, 8.6
    left_y, left_h = 76.4, 12.8
    right_y, right_h = 50.6, 16.2
    left_cy = left_y + left_h / 2
    # Enable pins sit on the load centers so the output wires stay straight.
    # Channel 11 uses the open band between these two cards.
    valve_cy = 63.8
    pump2_cy = 53.4
    pump1_cy = left_cy

    card(pca2_x, pca2_y, pca2_w, pca2_h, PCA_F, PCA_E, [
        "PCA9685 outputs",
        "same board  ·  0x40",
        "220 ohm resistors",
        "in series with",
        "each output",
    ], body=13.0, title=15.5)

    card(l_x, left_y, l_w, left_h, L298_F, L298_E, [
        "LEFT L298N  ·  PUMP1",
        "ENB, IN3, IN4 → this module's GND",
        "5 V out → IN1   ·   GND → IN2",
        "OUT3 / OUT4 open",
    ], body=12.6, title=14.8, align="left")
    card(l_x, right_y, l_w, right_h, L298_F, L298_E, [
        "RIGHT L298N",
        "valve + PUMP2",
        "5 V out → IN1 and IN3",
        "GND → IN2 and IN4",
    ], body=12.8, title=14.8, align="left")

    card(load_x, pump1_cy - load_h / 2, load_w, load_h, ACT_F, ACT_E, [
        "PUMP1",
        "Vacuum",
        "OUT1 / OUT2",
    ], body=12.8, title=15.5)
    card(load_x, valve_cy - load_h / 2, load_w, load_h, ACT_F, ACT_E, [
        "Valve",
        "OUT1 / OUT2",
        "full-on / full-off",
    ], body=12.6, title=15.5)
    card(load_x, pump2_cy - load_h / 2, load_w, load_h, ACT_F, ACT_E, [
        "PUMP2",
        "Pressure",
        "OUT3 / OUT4",
    ], body=12.8, title=15.5)

    def drive(y, src_right, dst_left, label, color, out_label=None, dst_right=None, load_left=None):
        wire([(src_right, y), (dst_left, y)], color, lw=3.6)
        node(src_right, y, color, ms=10)
        node(dst_left, y, color, ms=10)
        tag((src_right + dst_left) / 2, y + 1.4, label, color, size=11.4)
        if out_label and dst_right is not None and load_left is not None:
            wire([(dst_right, y), (load_left, y)], LOAD, lw=3.6)
            node(dst_right, y, LOAD, ms=10)
            node(load_left, y, LOAD, ms=10)
            tag((dst_right + load_left) / 2, y + 1.4, out_label, LOAD, size=11.4)

    pca_right = pca2_x + pca2_w
    l_right = l_x + l_w
    drive(pump1_cy, pca_right, l_x, "ch 10 · ENA", CTRL, "OUT1/OUT2", l_right, load_x)
    drive(valve_cy, pca_right, l_x, "ch 8 · ENA", CTRL, "OUT1/OUT2", l_right, load_x)
    drive(pump2_cy, pca_right, l_x, "ch 9 · ENB", CTRL, "OUT3/OUT4", l_right, load_x)

    # Channel 11 stops in the gap between the two drivers. It does not land on one.
    ch11_y = (left_y + right_y + right_h) / 2
    wire([(pca_right, ch11_y), (pca_right + 5.2, ch11_y)], "#9AA3AD", lw=3.2, dashed=True)
    node(pca_right, ch11_y, "#9AA3AD", ms=10)
    tag(pca_right + 6.2, ch11_y + 1.25, "ch 11 not wired", "#6B7280", size=11.4, ha="left")

    rounded(6.2, 39.6, 155.6, 7.2, "white", "#C5D6D8", lw=1.3, radius=0.9, z=3)
    tag(W / 2, 44.7,
        "Both modules: 5V-EN jumper installed, ENA and ENB jumper caps removed.",
        NAVY, size=13.2)
    tag(W / 2, 41.85,
        "The two 5 V outputs stay separate from each other and from the ESP32.  Do not tie either 5 V output to 3.3 V.",
        PWR_E, size=12.6)

    # ---- 3. Power -------------------------------------------------------
    s3x, s3y, s3w, s3h = 2.6, 6.5, 162.8, 30.2
    section(s3x, s3y, s3w, s3h, 3, "Power and jumper settings")

    pow_y, pow_h = 14.6, 14.2
    card(6.2, pow_y, 24.5, pow_h, PWR_F, PWR_E, [
        "12 V adapter",
        "center positive",
        "sleeve negative",
    ], body=12.6, title=14.8)
    card(42.5, pow_y, 26.5, pow_h, SLATE_F, SLATE_E, [
        "PRT-10288",
        "female barrel jack",
        "+ and − screws",
    ], body=12.2, title=14.8)
    card(80.5, pow_y, 24.5, pow_h, BOARD_F, BOARD_E, [
        "Breadboard",
        "+ to motor screws",
        "− to common ground",
    ], body=12.0, title=14.8)

    def pair(x1, x2, y_mid, label_plus, label_minus):
        yp, yn = y_mid + 1.45, y_mid - 1.45
        wire([(x1, yp), (x2, yp)], POWER, lw=3.6)
        wire([(x1, yn), (x2, yn)], GND, lw=3.6)
        node(x1, yp, POWER, ms=9)
        node(x2, yp, POWER, ms=9)
        node(x1, yn, GND, ms=9)
        node(x2, yn, GND, ms=9)
        tag((x1 + x2) / 2, yp + 1.25, label_plus, POWER, size=11.0)
        tag((x1 + x2) / 2, yn - 1.25, label_minus, GND, size=11.0)

    mid = pow_y + pow_h / 2
    pair(30.7, 42.5, mid, "center +", "sleeve −")
    pair(69.0, 80.5, mid, "+12 V", "GND")

    # One column of terminals. Red bus feeds the motor screws.
    # Gray bus is the sleeve-negative common ground.
    pills = [
        (26.55, "Left motor +", L298_F, L298_E, "plus"),
        (23.85, "Right motor +", L298_F, L298_E, "plus"),
        (20.55, "Left GND", "#ECEFF1", GND, "gnd"),
        (17.85, "Right GND", "#ECEFF1", GND, "gnd"),
        (15.15, "ESP32 GND", ESP_F, ESP_E, "gnd"),
        (12.45, "PCA9685 GND", PCA_F, PCA_E, "gnd"),
    ]
    pill_x, pill_w, pill_h = 112.2, 22.5, 2.35
    plus_ys = []
    gnd_ys = []
    for cy, text, fc, ec, kind in pills:
        rounded(pill_x, cy - pill_h / 2, pill_w, pill_h, fc, ec, lw=1.6, radius=0.55, z=3)
        tag(pill_x + pill_w / 2, cy, text, ec, size=11.2)
        (plus_ys if kind == "plus" else gnd_ys).append(cy)

    bb_right = 80.5 + 24.5
    red_bus, gnd_bus = 107.6, 109.6
    red_y, gnd_y = mid + 1.45, mid - 1.45
    wire([(bb_right, red_y), (red_bus, red_y)], POWER, lw=3.4)
    wire([(red_bus, min(plus_ys + [red_y])), (red_bus, max(plus_ys + [red_y]))], POWER, lw=3.4)
    node(bb_right, red_y, POWER, ms=9)
    for y in plus_ys:
        wire([(red_bus, y), (pill_x, y)], POWER, lw=3.2)
        node(red_bus, y, POWER, ms=8)
        node(pill_x, y, POWER, ms=8)
    wire([(bb_right, gnd_y), (gnd_bus, gnd_y)], GND, lw=3.4)
    wire([(gnd_bus, min(gnd_ys + [gnd_y])), (gnd_bus, max(gnd_ys + [gnd_y]))], GND, lw=3.4)
    node(bb_right, gnd_y, GND, ms=9)
    for y in gnd_ys:
        wire([(gnd_bus, y), (pill_x, y)], GND, lw=3.2)
        node(gnd_bus, y, GND, ms=8)
        node(pill_x, y, GND, ms=8)

    # Rules sit in the power section, as on the earlier figure.
    rules_x, rules_w = 136.6, 26.2
    rounded(rules_x, 22.15, rules_w, 6.55, RULE_F, RULE_E, lw=1.8, radius=0.8, z=3)
    tag(rules_x + rules_w / 2, 26.85, "On BOTH modules", RULE_E, size=11.6)
    tag(rules_x + rules_w / 2, 24.85, "5V-EN jumper: installed", INK, size=10.6, weight="regular")
    tag(rules_x + rules_w / 2, 23.15, "ENA + ENB caps: removed", INK, size=10.6, weight="regular")

    rounded(rules_x, 14.7, rules_w, 6.9, WARN_F, WARN_E, lw=1.8, radius=0.8, z=3)
    tag(rules_x + rules_w / 2, 19.55, "Keep the two +5 V", WARN_E, size=10.8)
    tag(rules_x + rules_w / 2, 17.85, "outputs separate.", WARN_E, size=10.8)
    tag(rules_x + rules_w / 2, 16.15, "Do not tie either to 3.3 V.", WARN_E, size=10.4)

    tag(56.0, 11.55,
        "Qwiic 3.3 V and PCA9685 VCC do not power the motors.",
        NAVY, size=12.6)

    # ---- Footer ---------------------------------------------------------
    ax.text(
        W / 2, 3.15,
        "Wire with USB disconnected, battery disconnected, and the adapter unplugged from the wall.  "
        "Connections are by label.  Positions are illustrative.",
        ha="center", va="center", fontsize=13.5, color=NAVY, zorder=4,
    )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(out_path, dpi=DPI, facecolor="white")
    if export_sections:
        from matplotlib.transforms import Bbox

        # Export the same native figure sections for the assembly steps.
        # Coordinates are in the figure's drawing units, not bitmap pixels.
        for (badge_text, title_text), number, title in zip(
                section_labels, (2, 3, 4),
                ("Control boards", "Drivers and loads", "Power wiring checks")):
            badge_text.set_text(str(number))
            title_text.set_text(title)
        sections = [
            ("wiring-control-boards.png", s1x, s1y, s1w, s1h),
            ("wiring-drivers.png", s2x, s2y, s2w, s2h),
            ("wiring-power-check.png", s3x, s3y, s3w, s3h),
        ]
        for name, x, y, width, height in sections:
            bounds = Bbox.from_extents(
                (x - 0.5) * FIG_W / W, (y - 0.5) * FIG_H / H,
                (x + width + 0.5) * FIG_W / W,
                (y + height + 0.5) * FIG_H / H,
            )
            fig.savefig(out_path.parent / name, dpi=DPI, facecolor="white",
                        bbox_inches=bounds, pad_inches=0)
            print(f"wrote {out_path.parent / name}")
    plt.close(fig)
    try:
        from PIL import Image
        image = Image.open(out_path).convert("RGB")
        image.save(out_path, optimize=True)
        print(f"wrote {out_path} {image.size[0]}x{image.size[1]}")
    except ImportError:
        print(f"wrote {out_path}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--outdir", default=None, help="output directory (default: this script's directory)")
    parser.add_argument("--sections", action="store_true", help="also export focused assembly figures")
    args = parser.parse_args()
    out_dir = Path(args.outdir).expanduser().resolve() if args.outdir else HERE
    _render(out_dir / "wiring-diagram.png", export_sections=args.sections)


if __name__ == "__main__":
    main()
