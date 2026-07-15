#!/usr/bin/env python3
"""Render the electrical wiring diagram for this design.

SPDX-License-Identifier: CERN-OHL-W-2.0
Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE

An Adafruit ATtiny1616 Breakout (seesaw firmware, STEMMA QT / Qwiic) sits between the ESP32
Thing Plus and the two L298N drivers. The ESP32's single Qwiic (I2C) bus daisy-chains to three
boards (Button, MicroPressure, seesaw board); the seesaw board's GPIO/PWM pins then drive the
three connected L298N signals that earlier direct-GPIO builds wired straight to the ESP32
(pins 32/33/14). Pin 4 is reserved for a future VALVE1 channel but physically NC. Unlike a
plain digital I2C GPIO expander (no PWM), seesaw exposes real 8-bit PWM over I2C, so pump
drive stays fully proportional -- see README.md.

This is the editable source for the wiring diagram (OSHWA requires design files in a
"preferred format for making changes", not just a rendered raster export). Regenerate the PNG
with:

    python3 generate_wiring_diagram.py

Wiring facts here must stay in sync with:
  - README.md (this folder) -- prose description of the same connections
  - ../../software/rheometer-firmware/PneumaticSystem.h -- seesaw pin #defines (source of truth)

If you change a connection, update all three together.
"""

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import FancyBboxPatch

I2C = "#8e44ad"        # Qwiic/I2C bus wiring (purple)
CTRL = "#e07b00"       # seesaw board -> L298N control lines (orange, PWM-capable)
PNEUM = "#3aa0ff"      # L298N -> actuator power leads (blue)
POWER = "#c58b00"      # external +12 V motor supply
GND = "#333333"        # common ground
ESP_C, ESP_E = "#dbeafe", "#1565c0"     # ESP32 Thing Plus
QW_C, QW_E = "#f3e8ff", "#6a1b9a"       # Qwiic peripherals (Button/MPRLS)
SS_C, SS_E = "#fff3cd", "#b8860b"       # ATtiny1616 seesaw board
L298_C, L298_E = "#d8f0d8", "#2e7d32"   # L298N drivers
ACT_C, ACT_E = "#ffe6c2", "#e07b00"     # pumps/valve (actuators)
UNUSED_C, UNUSED_E = "#f0f0f0", "#aaaaaa"  # unused/wired-but-idle
TABLE_C, TABLE_E = "#eef2ff", "#4338ca"    # pin-map reference table

fig, ax = plt.subplots(figsize=(14.5, 10))
ax.set_xlim(0, 116)
ax.set_ylim(0, 100)
ax.axis("off")


def box(x, y, w, h, fc, ec, title, sub="", tsize=10, ssize=7.8, dashed=False):
    ax.add_patch(
        FancyBboxPatch(
            (x, y), w, h, boxstyle="round,pad=0.4,rounding_size=2",
            linewidth=1.6, edgecolor=ec, facecolor=fc, zorder=3,
            linestyle="dashed" if dashed else "solid",
        )
    )
    ax.text(x + w / 2, y + h - h * 0.28, title, ha="center", va="top",
             fontsize=tsize, fontweight="bold", color=ec, zorder=4)
    if sub:
        ax.text(x + w / 2, y + h * 0.32, sub, ha="center", va="center",
                 fontsize=ssize, color="#333333", zorder=4, linespacing=1.5)


def l298_box(x, y, w, title, motor_a_rows, motor_b_rows):
    """Draw one L298N module with every screw/header terminal accounted for."""
    h = 22
    ax.add_patch(
        FancyBboxPatch(
            (x, y), w, h, boxstyle="round,pad=0.4,rounding_size=2",
            linewidth=1.6, edgecolor=L298_E, facecolor=L298_C, zorder=3,
        )
    )
    ax.text(x + w / 2, y + h - 1.2, title, ha="center", va="top",
            fontsize=9.5, fontweight="bold", color=L298_E, zorder=4)
    ax.text(x + w / 2, y + h - 3.5, "5V-EN jumper ON  \u00b7  ENA/ENB jumper caps OFF",
            ha="center", va="top", fontsize=6.4, fontweight="bold",
            color=L298_E, zorder=4)
    ax.text(x + w * 0.25, y + h - 5.2, "MOTOR A", ha="center", va="top",
            fontsize=6.8, fontweight="bold", color="#555555", zorder=4)
    ax.text(x + w * 0.75, y + h - 5.2, "MOTOR B", ha="center", va="top",
            fontsize=6.8, fontweight="bold", color="#555555", zorder=4)
    ax.plot([x + w / 2, x + w / 2], [y + 5.2, y + h - 5.2],
            color="#92b892", lw=0.8, zorder=4)
    for i, (motor_a, motor_b) in enumerate(zip(motor_a_rows, motor_b_rows)):
        row_y = y + h - 7.3 - i * 2.15
        ax.text(x + 1.2, row_y, motor_a, ha="left", va="top",
                fontsize=6.1, color="#222222", fontfamily="monospace", zorder=4)
        ax.text(x + w / 2 + 1.0, row_y, motor_b, ha="left", va="top",
                fontsize=6.1, color="#222222", fontfamily="monospace", zorder=4)
    ax.plot([x + 1.2, x + w - 1.2], [y + 4.6, y + 4.6],
            color="#92b892", lw=0.8, zorder=4)
    ax.text(x + w / 2, y + 2.35,
            "12V: external in  \u00b7  +5V: local regulator output  \u00b7  GND: common",
            ha="center", va="center", fontsize=6.1, color="#222222",
            fontfamily="monospace", zorder=4)


def wire(pts, color, lw=3.0, dashed=False):
    xs = [p[0] for p in pts]
    ys = [p[1] for p in pts]
    ax.plot(xs, ys, color=color, lw=lw, solid_capstyle="round",
             solid_joinstyle="round", zorder=2,
             linestyle="dashed" if dashed else "solid")


def node(x, y, color):
    ax.plot([x], [y], marker="o", ms=7, mfc=color, mec="white", mew=1.1, zorder=5)


def ground_symbol(x, y_top, scale=1.0):
    """Draw the standard three-bar ground symbol below a connection point."""
    y_bar = y_top - 2.0 * scale
    ax.plot([x, x], [y_top, y_bar], color=GND, lw=2.2,
            solid_capstyle="round", zorder=2)
    for offset, width in ((0.0, 3.2), (0.75, 2.1), (1.5, 1.0)):
        y = y_bar - offset * scale
        half_width = width * scale / 2
        ax.plot([x - half_width, x + half_width], [y, y], color=GND, lw=2.2,
                solid_capstyle="round", zorder=2)


# ---- Title ------------------------------------------------------------ #
ax.text(58, 99, "RheoBoard DIY — Electrical Schematic",
         ha="center", va="top", fontsize=14.5, fontweight="bold")

# Keep the design summary and every legend label inside one box. The source
# diagram placed the summary just above a shorter legend box, which allowed the
# text to overlap the border in tightly cropped/rendered previews.
ax.add_patch(FancyBboxPatch((3, 89.8), 110, 6.2,
                             boxstyle="round,pad=0.3,rounding_size=1.5",
                             linewidth=1.0, edgecolor="#999999", facecolor="#f6f6f6", zorder=1))
ax.text(58, 94.4,
        "ESP32 Thing Plus + Adafruit ATtiny1616 (seesaw) + 2\u00d7 L298N  \u00b7  "
        "seesaw pins 0/1/5 replace ESP32 GPIO 32/33/14; pin 4 reserved/NC  \u00b7  real PWM preserved",
        ha="center", va="center", fontsize=8.2, color="#555555")

# ---- Legend ------------------------------------------------------------ #
legend_y = 91.7
legend_items = [
    (6, I2C, "Qwiic / I\u00b2C"),
    (27, CTRL, "seesaw \u2192 EN"),
    (49, PNEUM, "L298N \u2192 load"),
    (72, POWER, "+12 V motor"),
]
for x, color, label in legend_items:
    ax.plot([x, x + 4], [legend_y, legend_y], color=color, lw=3.2,
            solid_capstyle="round", zorder=2)
    ax.text(x + 5, legend_y, label, ha="left", va="center", fontsize=7.8)
ground_symbol(96, 93.2, scale=0.55)
ax.text(99, legend_y, "common GND", ha="left", va="center", fontsize=7.8)

# ---- Row 1: ESP32 (left) + pin-map reference table (right) ------------ #
esp_x, esp_y, esp_w, esp_h = 17, 76, 30, 12
box(esp_x, esp_y, esp_w, esp_h, ESP_C, ESP_E, "ESP32 Thing Plus",
    "1\u00d7 Qwiic connector\n(SDA / SCL / 3V3 / GND)", tsize=10.5, ssize=8)
esp_bottom = (esp_x + esp_w / 2, esp_y)

tbl_x, tbl_y, tbl_w, tbl_h = 53, 76, 61, 12
ax.add_patch(FancyBboxPatch((tbl_x, tbl_y), tbl_w, tbl_h,
                             boxstyle="round,pad=0.4,rounding_size=2",
                             linewidth=1.4, edgecolor=TABLE_E, facecolor=TABLE_C, zorder=3))
ax.text(tbl_x + tbl_w / 2, tbl_y + tbl_h - 1.6, "seesaw pin map (was ESP32 pin \u2192 now here)",
         ha="center", va="top", fontsize=9, fontweight="bold", color=TABLE_E, zorder=4)
rows = [
    "0 (PWM) \u2192 L298N#1 ENA = PUMP1_EN   (was pin 32)",
    "1 (PWM) \u2192 L298N#1 ENB = PUMP2_EN   (was pin 33)",
    "4 (GPIO) \u2192 NC (reserved VALVE1_EN; no physical wire)",
    "5 (GPIO) \u2192 L298N#2 ENB = VALVE2_EN  (was pin 14, driven)",
]
for i, row in enumerate(rows):
    ax.text(tbl_x + 2.5, tbl_y + tbl_h - 4.2 - i * 1.9, row, ha="left", va="top",
             fontsize=7.6, color="#222222", fontfamily="monospace", zorder=4)

# ---- Row 2: Qwiic bus (I2C) -- daisy chain to 3 peripherals ----------- #
bus_y = 70
wire([esp_bottom, (esp_bottom[0], bus_y)], I2C, lw=3.5)
wire([(28, bus_y), (98, bus_y)], I2C, lw=3.5)  # horizontal bus
for bx in (28, 63, 98):
    node(bx, bus_y, I2C)

per_y, per_h = 54, 14
box(17, per_y, 22, per_h, QW_C, QW_E, "Qwiic Button", "manual REP\nsuck / blow\naddr 0x6F", tsize=9.5, ssize=7.3)
box(52, per_y, 22, per_h, SS_C, SS_E, "ATtiny1616",
    "seesaw \u00b7 addr 0x49\npowered by Qwiic 3.3V\nVin: NC", tsize=9.5, ssize=6.8)
box(87, per_y, 22, per_h, QW_C, QW_E, "MicroPressure", "MPRLS\naddr 0x18", tsize=9.5, ssize=7.5)
wire([(28, bus_y), (28, per_y + per_h)], I2C, lw=3)
wire([(63, bus_y), (63, per_y + per_h)], I2C, lw=3)
wire([(98, bus_y), (98, per_y + per_h)], I2C, lw=3)
ax.text(78, bus_y + 1.4, "Qwiic bus (I\u00b2C, daisy-chained)", ha="center", va="bottom",
         fontsize=8, color=I2C, style="italic")

# The Qwiic cable supplies both 3.3 V and common GND to the seesaw board.
# Vin is therefore left unconnected; show the board's GND pin explicitly.
wire([(52, 61), (49, 61), (49, 59.5)], GND, lw=2.2)
node(52, 61, GND)
ground_symbol(49, 59.5, scale=0.8)
ax.text(48.1, 61.8, "GND", ha="right", va="center",
        fontsize=6.8, color=GND, fontweight="bold")

# ---- seesaw pins 0/1/5 -> L298N; pin 4 reserved/NC --------------------- #
ss_pins_x = {"0": 56, "1": 60, "4": 66, "5": 70}
for label, px in ss_pins_x.items():
    pin_color = UNUSED_E if label == "4" else SS_E
    node(px, per_y, pin_color)
    pin_label = "4 NC" if label == "4" else label
    ax.text(px, per_y - 1.2, pin_label, ha="center", va="top", fontsize=7,
             color=pin_color, fontweight="bold")

l298_y, l298_h = 26, 22
l298_1_x, l298_1_w = 25, 40   # Pumps
l298_2_x, l298_2_w = 68, 40   # Valve
l298_box(
    l298_1_x, l298_y, l298_1_w, "L298N #1 (Pumps)",
    [
        "ENA: seesaw 0 PWM",
        "IN1: 5V",
        "IN2: GND",
        "OUT1/2: PUMP1",
    ],
    [
        "ENB: seesaw 1 PWM",
        "IN3: 5V",
        "IN4: GND",
        "OUT3/4: PUMP2",
    ],
)
l298_box(
    l298_2_x, l298_y, l298_2_w, "L298N #2 (Valve)",
    [
        "ENA: NC",
        "IN1: NC",
        "IN2: NC",
        "OUT1/2: NC",
    ],
    [
        "ENB: seesaw 5 DIG",
        "IN3: 5V",
        "IN4: GND",
        "OUT3/4: VALVE2",
    ],
)

ena1_x, enb1_x = 34.5, 55.5
ena2_x, enb2_x = 77.5, 98.5
top_y, bot_y = l298_y + l298_h, l298_y

wire([(ss_pins_x["0"], per_y), (ena1_x, top_y)], CTRL, lw=2.4)
wire([(ss_pins_x["1"], per_y), (enb1_x, top_y)], CTRL, lw=2.4)
wire([(ss_pins_x["5"], per_y), (enb2_x, top_y)], CTRL, lw=2.4)
for x in (ena1_x, enb1_x, enb2_x):
    node(x, top_y, CTRL)

# ---- External motor power + common ground ------------------------------ #
# The Qwiic cable powers only the logic boards. Both L298N motor rails use
# the external 12 V adapter. Repeated ground symbols identify one common net
# shared by the adapter, ESP32, and both L298N modules.
box(2, 30, 13, 15, "#fff1cc", POWER, "12 V DC", "external supply\n\u2265 2 A", tsize=9, ssize=7.2)
power_y = 42.0
node(15, power_y, POWER)
wire([(15, power_y), (l298_1_x, power_y)], POWER, lw=2.6)
node(l298_1_x, power_y, POWER)
wire([(l298_1_x + l298_1_w, power_y), (l298_2_x, power_y)], POWER, lw=2.6)
node(l298_1_x + l298_1_w, power_y, POWER)
node(l298_2_x, power_y, POWER)
ax.text(21.5, power_y + 1.1, "+12 V", ha="center", va="bottom",
        fontsize=7.2, color=POWER, fontweight="bold")

# Standard ground symbols: each one denotes this same common-GND net.
ground_symbol(8.5, 30, scale=1.0)            # 12 V adapter negative
ground_symbol(27.5, l298_y, scale=1.0)       # L298N #1 GND
ground_symbol(70.5, l298_y, scale=1.0)       # L298N #2 GND
wire([(esp_x, 82), (14, 82), (14, 80)], GND, lw=2.2)
node(esp_x, 82, GND)
ground_symbol(14, 80, scale=1.0)             # ESP32 GND

# ---- L298N -> actuators ------------------------------------------------ #
act_y, act_h = 7, 12
box(27, act_y, 15, act_h, ACT_C, ACT_E, "PUMP1", "vacuum\nOUT1/2", tsize=9, ssize=7.3)
box(48, act_y, 15, act_h, ACT_C, ACT_E, "PUMP2", "pressure\nOUT3/4", tsize=9, ssize=7.3)
box(69, act_y, 15, act_h, UNUSED_C, UNUSED_E, "VALVE1", "not fitted\nchannel A = NC", tsize=9, ssize=7.0, dashed=True)
box(90, act_y, 15, act_h, ACT_C, ACT_E, "VALVE2", "flip selector\nOUT3/4", tsize=9, ssize=7.3)

wire([(34.5, bot_y), (34.5, act_y + act_h)], PNEUM, lw=2.6)
wire([(55.5, bot_y), (55.5, act_y + act_h)], PNEUM, lw=2.6)
wire([(98.5, bot_y), (97.5, act_y + act_h)], PNEUM, lw=2.6)

# ---- Footer note -------------------------------------------------------- #
ax.text(58, 1.8,
        "Pumps keep full proportional PWM through the seesaw board's real PWM pins (0/1) \u2014 "
        "REP retract/extrude stay ramped/percentage drive. The external 12 V supply powers both "
        "L298N motor rails; all grounds share one common reference.",
        ha="center", va="center", fontsize=8.6, color="#222222")

fig.tight_layout()
OUT = "wiring-diagram.png"
fig.savefig(OUT, dpi=170, bbox_inches="tight")

# Quantize: this is a flat-color technical diagram, not a photo, so a small palette
# (PIL FASTOCTREE, not MEDIANCUT -- MEDIANCUT chokes on RGBA) cuts file size ~4x with no
# visible quality loss. Keeps the PNG small enough to embed inline in markdown/README.
try:
    from PIL import Image
    im = Image.open(OUT).convert("RGBA")
    im.quantize(colors=64, method=Image.Quantize.FASTOCTREE).save(OUT, optimize=True)
except ImportError:
    print("(Pillow not installed -- skipped palette quantization; PNG will be larger than usual)")

print(f"Wrote {OUT}")
