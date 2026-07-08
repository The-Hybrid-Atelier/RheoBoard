#!/usr/bin/env python3
"""Generate wiring-diagram.png — the electrical schematic for this design.

SPDX-License-Identifier: CERN-OHL-W-2.0
Copyright (c) 2026 Charlie Vuong -- see ../../../LICENSE-HARDWARE.txt

This is the editable source for the wiring diagram (OSHWA requires design files in a
"preferred format for making changes", not just a rendered raster export). Regenerate the PNG
with:

    python3 generate_wiring_diagram.py

Wiring facts here must stay in sync with:
  - README.md (this folder) -- prose description of the same connections
  - ../../software/rheometer-firmware/PneumaticSystem.h -- GPIO pin #defines (source of truth)

If you change a connection, update all three together.
"""

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from matplotlib.lines import Line2D

# ---- Palette -----------------------------------------------------------------
C_POWER = "#d98c1a"      # +12V supply rail
C_GND = "#1a1a1a"        # common ground
C_CTRL = "#2f6fb3"       # GPIO -> EN control lines
C_LOAD = "#8c1c1c"       # motor/valve load output
C_I2C = "#7a4fa3"        # Qwiic (I2C)
C_UNUSED = "#9a9a9a"     # unused / not populated

FILL_SUPPLY = "#f6e2c0"
FILL_MCU = "#cfe0f3"
FILL_I2C = "#e3d6f0"
FILL_DRIVER = "#f3d6d6"
FILL_LOAD = "#d9ecd4"
FILL_UNPOP = "#e8e8e8"

FIG_W, FIG_H = 11.5, 8.2

fig, ax = plt.subplots(figsize=(FIG_W, FIG_H), dpi=220)
ax.set_xlim(0, 11.5)
ax.set_ylim(0, 8.2)
ax.set_aspect("equal")
ax.axis("off")

# ---- Sheet border + grid reference (cosmetic, standard schematic convention) --
border = Rectangle((0.25, 0.25), 11.0, 7.7, fill=False, lw=1.1, edgecolor="#333333")
ax.add_patch(border)
for i, x in enumerate(range(1, 9)):
    gx = 0.25 + x * (11.0 / 8.5)
    ax.text(gx, 7.97, str(i + 1), ha="center", va="center", fontsize=6, color="#888888")
    ax.text(gx, 0.33, str(i + 1), ha="center", va="center", fontsize=6, color="#888888")
for i, letter in enumerate("ABCDEF"):
    gy = 7.85 - i * (7.4 / 5.5)
    ax.text(0.33, gy, letter, ha="center", va="center", fontsize=6, color="#888888")
    ax.text(11.15, gy, letter, ha="center", va="center", fontsize=6, color="#888888")

# ---- Title block (top of sheet) ----------------------------------------------
ax.text(5.75, 7.75, "RheoBoard DIY — Electrical Schematic", ha="center", va="center",
        fontsize=15, fontweight="bold")
ax.text(5.75, 7.45,
        "ESP32 Thing Plus + 2× L298N + 2× Adafruit 4700 pumps + 1× Adafruit 4663 valve + "
        "Qwiic MicroPressure + Qwiic Button",
        ha="center", va="center", fontsize=8, style="italic", color="#333333")

# ---- Legend (single row, sits in the gap above the component boxes) ---------
legend_items = [
    (C_POWER, "-", "+12V supply"),
    (C_GND, "-", "GND (common)"),
    (C_CTRL, "-", "GPIO → EN (control)"),
    (C_LOAD, "-", "load / motor output"),
    (C_I2C, "-", "Qwiic (I2C)"),
    (C_UNUSED, "--", "unused / not populated"),
]
lx = 0.6
ly = 7.12
item_w = 1.78
for i, (color, style, label) in enumerate(legend_items):
    x0 = lx + i * item_w
    ls = "--" if style == "--" else "-"
    ax.add_line(Line2D([x0, x0 + 0.35], [ly, ly], color=color, lw=2, linestyle=ls))
    ax.text(x0 + 0.45, ly, label, va="center", fontsize=6.8)

# ---- Component box helper ------------------------------------------------
def box(x, y, w, h, title, subtitle, fill, pins=None, dashed=False):
    """Draw a component box. pins: list of (side, frac, label, color) for pin labels."""
    edge = "#666666" if dashed else "#333333"
    rect = Rectangle((x, y), w, h, facecolor=fill, edgecolor=edge, lw=1.3,
                      linestyle="--" if dashed else "-")
    ax.add_patch(rect)
    ax.text(x + w / 2, y + h - 0.28, title, ha="center", va="center",
            fontsize=9.5, fontweight="bold", color="#222222" if not dashed else "#777777")
    if subtitle:
        ax.text(x + w / 2, y + h - 0.55, subtitle, ha="center", va="center",
                fontsize=7, style="italic", color="#555555" if not dashed else "#888888")
    coords = {}
    if pins:
        for side, frac, label, color in pins:
            if side == "left":
                px, py = x, y + frac * h
                ha = "right"
                lx_ = px - 0.08
            elif side == "right":
                px, py = x + w, y + frac * h
                ha = "left"
                lx_ = px + 0.08
            elif side == "bottom":
                px, py = x + frac * w, y
                ha = "center"
                lx_ = px
            else:  # top
                px, py = x + frac * w, y + h
                ha = "center"
                lx_ = px
            v_off = -0.13 if side == "bottom" else (0.13 if side == "top" else 0.0)
            va = "top" if side == "bottom" else ("bottom" if side == "top" else "center")
            ax.text(lx_, py + v_off, label, ha=ha, va=va, fontsize=6.3, color=color)
            coords[label] = (px, py)
    return coords


def wire(p1, p2, color, dashed=False, waypoints=None):
    pts = [p1] + (waypoints or []) + [p2]
    xs = [p[0] for p in pts]
    ys = [p[1] for p in pts]
    ax.plot(xs, ys, color=color, lw=1.6, linestyle="--" if dashed else "-",
            solid_capstyle="round", zorder=2)


def junction(p, color=C_GND):
    ax.add_patch(plt.Circle(p, 0.045, color=color, zorder=3))


# ---- Boxes -----------------------------------------------------------------
supply = box(0.7, 6.0, 2.1, 0.85, "12V DC SUPPLY", "external adapter", FILL_SUPPLY,
             pins=[("right", 0.72, "+12V", C_POWER), ("right", 0.28, "GND", C_GND)])

esp32 = box(0.7, 4.55, 2.1, 1.35, "ESP32 THING PLUS", "SparkFun, micro-USB, WRL-15663", FILL_MCU,
            pins=[
                ("right", 0.92, "GPIO32", C_CTRL),
                ("right", 0.76, "GPIO33", C_CTRL),
                ("right", 0.60, "GPIO14", C_CTRL),
                ("right", 0.44, "GPIO15*", C_UNUSED),
                ("right", 0.28, "GND", C_GND),
                ("left", 0.15, "Qwiic", C_I2C),
            ])
ax.text(0.7 + 2.1 - 0.05, 4.55 + 0.10, "*reserved, not populated", ha="right", va="center",
        fontsize=5.4, style="italic", color=C_UNUSED)

mprls = box(0.7, 3.75, 2.1, 0.65, "QWIIC MICROPRESSURE", "MPRLS, I2C 0x18", FILL_I2C,
            pins=[("left", 0.5, "Qwiic", C_I2C)])
button = box(0.7, 3.00, 2.1, 0.60, "QWIIC BUTTON", "I2C, default 0x6F", FILL_I2C,
             pins=[("left", 0.5, "Qwiic", C_I2C)])

l298n1 = box(3.7, 4.75, 2.4, 2.15, "L298N #1 — PUMPS", "dual H-bridge driver", FILL_DRIVER,
             pins=[
                 ("top", 0.5, "+12V", C_POWER),
                 ("left", 0.88, "ENA", C_CTRL),
                 ("left", 0.72, "ENB", C_CTRL),
                 ("bottom", 0.3, "IN1/3→+5V", "#555555"),
                 ("bottom", 0.7, "IN2/4→GND", "#555555"),
                 ("left", 0.10, "GND", C_GND),
                 ("right", 0.80, "OUT1/2", C_LOAD),
                 ("right", 0.35, "OUT3/4", C_LOAD),
             ])

l298n2 = box(3.7, 2.30, 2.4, 2.15, "L298N #2 — VALVE", "dual H-bridge driver", FILL_DRIVER,
             pins=[
                 ("top", 0.5, "+12V", C_POWER),
                 ("left", 0.88, "ENA", C_CTRL),
                 ("left", 0.72, "ENB", C_UNUSED),
                 ("bottom", 0.3, "IN1/3→+5V", "#555555"),
                 ("bottom", 0.7, "IN2/4→GND", "#555555"),
                 ("left", 0.10, "GND", C_GND),
                 ("right", 0.80, "OUT1/2", C_LOAD),
                 ("right", 0.35, "OUT3/4", C_UNUSED),
             ])

pump1 = box(7.6, 6.05, 2.3, 0.85, "PUMP1 — VACUUM", "Adafruit 4700", FILL_LOAD,
            pins=[("left", 0.5, "+/−", C_LOAD)])
pump2 = box(7.6, 4.95, 2.3, 0.85, "PUMP2 — PRESSURE", "Adafruit 4700", FILL_LOAD,
            pins=[("left", 0.5, "+/−", C_LOAD)])
valve2 = box(7.6, 3.85, 2.3, 0.85, "VALVE2 — flip sel.", "Adafruit 4663", FILL_LOAD,
             pins=[("left", 0.5, "+/−", C_LOAD)])
valve1 = box(7.6, 2.75, 2.3, 0.85, "2ND VALVE CH.", "not populated", FILL_UNPOP, dashed=True,
             pins=[("left", 0.5, "+/−", C_UNUSED)])

# ---- Wiring: +12V supply bus -------------------------------------------------
p12 = supply["+12V"]
bus12_x = 3.15
wire(p12, (bus12_x, p12[1]), C_POWER)
junction((bus12_x, p12[1]), C_POWER)
wire((bus12_x, p12[1]), (bus12_x, l298n1["+12V"][1] + 0.02),
     C_POWER, waypoints=[(bus12_x, l298n1["+12V"][1] + 0.02)])
wire((bus12_x, l298n1["+12V"][1]), l298n1["+12V"], C_POWER)
wire((bus12_x, p12[1]), (bus12_x, l298n2["+12V"][1]), C_POWER)
wire((bus12_x, l298n2["+12V"][1]), l298n2["+12V"], C_POWER)
junction((bus12_x, l298n2["+12V"][1]), C_POWER)

# ---- Wiring: common GND bus (supply, ESP32, both L298N) --------------------
# One vertical bus at gnd_bus_x; every GND pin T's into it via a short horizontal stub.
gnd_bus_x = 2.95
g_supply = supply["GND"]
g_esp = esp32["GND"]
g_l298n1 = l298n1["GND"]
g_l298n2 = l298n2["GND"]
bus_top_y = g_supply[1]
bus_bot_y = g_l298n2[1]
wire((gnd_bus_x, bus_top_y), (gnd_bus_x, bus_bot_y), C_GND)
for pin in (g_supply, g_esp, g_l298n1, g_l298n2):
    wire(pin, (gnd_bus_x, pin[1]), C_GND)
    junction((gnd_bus_x, pin[1]), C_GND)

# ---- Wiring: GPIO control lines --------------------------------------------
wire(esp32["GPIO32"], l298n1["ENA"], C_CTRL)
wire(esp32["GPIO33"], (l298n1["ENA"][0] - 0.25, esp32["GPIO33"][1]), C_CTRL,
     waypoints=[(l298n1["ENA"][0] - 0.25, esp32["GPIO33"][1])])
wire((l298n1["ENA"][0] - 0.25, esp32["GPIO33"][1]), l298n1["ENB"], C_CTRL)
wire(esp32["GPIO14"], l298n2["ENA"], C_CTRL)
gpio15 = esp32["GPIO15*"]
wire(gpio15, (l298n2["ENA"][0] - 0.25, gpio15[1]), C_UNUSED, dashed=True,
     waypoints=[(l298n2["ENA"][0] - 0.25, gpio15[1])])
wire((l298n2["ENA"][0] - 0.25, gpio15[1]), l298n2["ENB"], C_UNUSED, dashed=True)

# ---- Wiring: Qwiic I2C chain (routed left of the boxes, daisy-chained) -----
i2c_bus_x = 0.42
q_esp = esp32["Qwiic"]
q_mprls = mprls["Qwiic"]
q_button = button["Qwiic"]
wire(q_esp, (i2c_bus_x, q_esp[1]), C_I2C)
wire((i2c_bus_x, q_esp[1]), (i2c_bus_x, q_button[1]), C_I2C)
for pin in (q_esp, q_mprls, q_button):
    wire(pin, (i2c_bus_x, pin[1]), C_I2C)
junction((i2c_bus_x, q_mprls[1]), C_I2C)

# ---- Wiring: motor/valve loads ---------------------------------------------
wire(l298n1["OUT1/2"], pump1["+/−"], C_LOAD)
wire(l298n1["OUT3/4"], pump2["+/−"], C_LOAD)
wire(l298n2["OUT1/2"], valve2["+/−"], C_LOAD)
wire(l298n2["OUT3/4"], valve1["+/−"], C_UNUSED, dashed=True)

# ---- Notes -------------------------------------------------------------------
notes = [
    "NOTES:",
    "1. 10 kΩ pull-downs on GPIO 14/15/32/33 hold enable lines LOW at boot/reset, before firmware runs.",
    "2. ESP32 GND, both L298N GND terminals, and the 12V supply (−) are all tied together — one common reference for GPIO/EN logic and the motor rail.",
    "3. Pneumatic plumbing (tubing, valve ports, chamber) is a separate diagram — see wiring/tube-connection.png and wiring/pneumatic-plumbing.md.",
    "4. Firmware GPIO source of truth: software/rheometer-firmware/PneumaticSystem.h.",
]
ny = 2.15
for i, line in enumerate(notes):
    fw = "bold" if i == 0 else "normal"
    ax.text(0.7, ny - i * 0.24, line, fontsize=6.6, fontweight=fw, color="#222222")

# ---- Title block (bottom right) --------------------------------------------
tb_x, tb_y, tb_w, tb_h = 7.6, 0.55, 3.15, 1.35
ax.add_patch(Rectangle((tb_x, tb_y), tb_w, tb_h, fill=False, edgecolor="#333333", lw=1.1))
ax.plot([tb_x, tb_x + tb_w], [tb_y + 0.85, tb_y + 0.85], color="#333333", lw=0.8)
ax.plot([tb_x, tb_x + tb_w], [tb_y + 0.42, tb_y + 0.42], color="#333333", lw=0.8)
ax.plot([tb_x + tb_w / 3, tb_x + tb_w / 3], [tb_y, tb_y + 0.42], color="#333333", lw=0.8)
ax.plot([tb_x + 2 * tb_w / 3, tb_x + 2 * tb_w / 3], [tb_y, tb_y + 0.42], color="#333333", lw=0.8)
ax.text(tb_x + tb_w / 2, tb_y + 1.10, "RHEOBOARD DIY — THIS DESIGN", ha="center", va="center",
        fontsize=8, fontweight="bold")
ax.text(tb_x + tb_w / 2, tb_y + 0.63, "ELECTRICAL SCHEMATIC", ha="center", va="center", fontsize=7)
ax.text(tb_x + tb_w / 6, tb_y + 0.21, "SCALE: NTS", ha="center", va="center", fontsize=6)
ax.text(tb_x + tb_w / 2, tb_y + 0.21, "REV. 1", ha="center", va="center", fontsize=6)
ax.text(tb_x + 5 * tb_w / 6, tb_y + 0.21, "SHEET 1/1", ha="center", va="center", fontsize=6)

plt.tight_layout()
OUT = "wiring-diagram.png"
plt.savefig(OUT, dpi=220, bbox_inches="tight", facecolor="white")

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
