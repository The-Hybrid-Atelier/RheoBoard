#!/usr/bin/env python3
"""Render the RheoBoardOTS electrical wiring diagram.

SparkFun ESP32 Thing Plus. Wire.begin() uses SDA GPIO 23 and SCL GPIO 22.
Those header pins are the same I2C peripheral as the Qwiic connector.

Qwiic cable (connector only): Qwiic Button 0x6F and MPRLS 0x18.
PCA9685 product 815 has no Qwiic socket. Four header jumpers:
    ESP32 GND → PCA9685 GND
    ESP32 3V3 → PCA9685 VCC   (logic only; never 12 V)
    ESP32 SDA (GPIO 23) → PCA9685 SDA
    ESP32 SCL (GPIO 22) → PCA9685 SCL
Address 0x40. V+ and OE unconnected.

PCA9685 channels, not ESP32 GPIOs. On this L298N, INA is ENA
(OUT1/OUT2) and INB is ENB (OUT3/OUT4):
    8  → RIGHT L298N ENA (INA)   FA0520E valve, full-on / full-off
    9  → RIGHT L298N ENB (INB)   right pump (PUMP_2, extrude / pressure) PWM
    10 → LEFT  L298N ENA (INA)   left pump (PUMP_1, retract / vacuum) PWM
    11 → not connected; firmware parks it full-off
LEFT L298N ENB (INB) is wired to GND. Remove that jumper. Do not
drive it from channel 11.

One 12 V brick feeds both L298N VS terminals, common ground with the ESP32
and the PCA9685. Product 815 has 220 ohm resistors in series with its outputs.
V+ and OE stay open.

Usage:
    python wiring_diagram.py                  # -> wiring-diagram.png
    python wiring_diagram.py --outdir figs     # -> figs/wiring-diagram.png
"""
import argparse
from pathlib import Path

HERE = Path(__file__).resolve().parent


def _render(out_path: Path) -> None:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    from matplotlib.patches import FancyBboxPatch

    QW = "#6d28d9"
    HDR = "#0f766e"
    HDR_3V3 = "#15803d"
    CTRL = "#c2410c"
    DRIVE = "#0369a1"
    POWER = "#b91c1c"
    GROUND = "#1f2937"
    WARN = "#9f1239"
    ESP_C, ESP_E = "#dbeafe", "#1d4ed8"
    QW_C, QW_E = "#f3e8ff", "#6d28d9"
    PWM_C, PWM_E = "#fef3c7", "#a16207"
    L298_C, L298_E = "#dcfce7", "#166534"
    ACT_C, ACT_E = "#ffedd5", "#c2410c"
    PWR_C, PWR_E = "#fee2e2", "#b91c1c"
    UNUSED_C, UNUSED_E = "#f3f4f6", "#6b7280"
    NOTE_C, NOTE_E = "#f0fdfa", "#0f766e"
    MAP_C, MAP_E = "#eef2ff", "#3730a3"

    fig, ax = plt.subplots(figsize=(17.4, 13.2))
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis("off")
    fig.subplots_adjust(left=0.01, right=0.99, top=0.99, bottom=0.01)

    def box(x, y, w, h, fc, ec, title, sub="", tsize=9.2, ssize=6.5, dashed=False):
        ax.add_patch(
            FancyBboxPatch(
                (x, y), w, h, boxstyle="round,pad=0.25,rounding_size=1.2",
                linewidth=1.5, edgecolor=ec, facecolor=fc, zorder=3,
                linestyle=(0, (2.2, 1.6)) if dashed else "solid",
            )
        )
        ax.text(
            x + w / 2, y + h - 0.55, title, ha="center", va="top",
            fontsize=tsize, fontweight="bold", color=ec, zorder=4,
        )
        if sub:
            ax.text(
                x + w / 2, y + h - 2.35, sub, ha="center", va="top",
                fontsize=ssize, color="#1f2937", zorder=4, linespacing=1.35,
            )

    def wire(pts, color, lw=2.4, dashed=False):
        xs = [p[0] for p in pts]
        ys = [p[1] for p in pts]
        ax.plot(
            xs, ys, color=color, lw=lw, solid_capstyle="round",
            solid_joinstyle="round", zorder=2,
            linestyle=(0, (2.4, 1.5)) if dashed else "solid",
        )

    def node(x, y, color):
        ax.plot([x], [y], marker="o", ms=6.5, mfc=color, mec="white", mew=0.9, zorder=5)

    ax.text(
        50, 99.0, "RheoBoardOTS — ESP32 Thing Plus + PCA9685 + 2× L298N + FA0520E",
        ha="center", va="top", fontsize=14, fontweight="bold",
    )
    ax.text(
        50, 96.8,
        "Header jumpers to the PCA9685 (product 815 has no Qwiic socket).  "
        "Qwiic cable only to the button and MPRLS.  Same I2C peripheral: SDA GPIO 23, SCL GPIO 22.",
        ha="center", va="top", fontsize=8.0, color="#374151",
    )

    ax.add_patch(FancyBboxPatch(
        (2, 90.6), 96, 4.6, boxstyle="round,pad=0.2,rounding_size=0.8",
        linewidth=0.8, edgecolor="#d1d5db", facecolor="#f9fafb", zorder=1,
    ))
    legend = [
        (4.0, QW, "Qwiic cable"),
        (22.5, HDR, "Header SDA / SCL"),
        (40.5, HDR_3V3, "Header 3V3 → VCC"),
        (58.0, GROUND, "Common GND"),
        (74.0, CTRL, "PCA channel → ENA/ENB"),
        (92.0, POWER, "12V VS"),
    ]
    for x, color, label in legend:
        ax.plot([x, x + 3.2], [92.9, 92.9], color=color, lw=3.2, solid_capstyle="round", zorder=2)
        ax.text(x + 3.6, 92.9, label, ha="left", va="center", fontsize=7.0, color="#111827")

    # ---- ESP32 and Qwiic-only peripherals -----------------------------------
    box(
        2, 75.2, 30, 13.4, ESP_C, ESP_E, "ESP32 Thing Plus",
        "Qwiic connector → button + MPRLS\n"
        "Header jumpers → PCA9685\n"
        "GND,  3V3,  SDA GPIO 23\n"
        "SCL GPIO 22   ·   Wire.begin()",
        tsize=10, ssize=6.6,
    )
    box(
        38, 77.2, 26, 11.4, QW_C, QW_E, "Qwiic Button",
        "manual REP / suck / blow\naddr 0x6F\non the Qwiic cable",
        tsize=9.4, ssize=6.6,
    )
    box(
        70, 77.2, 26, 11.4, QW_C, QW_E, "MicroPressure",
        "MPRLS pressure stream\naddr 0x18\non the Qwiic cable",
        tsize=9.4, ssize=6.6,
    )
    qwiic_y = 88.6
    wire([(32, qwiic_y), (96, qwiic_y)], QW, lw=3.2)
    for bx in (32, 51, 83, 96):
        node(bx, qwiic_y, QW)
    ax.text(
        64, 89.55, "Qwiic cable — connector only",
        ha="center", va="bottom", fontsize=7.2, color=QW, fontweight="bold",
    )

    # ---- PCA9685, reached by header jumpers --------------------------------
    box(
        34, 55.6, 32, 13.2, PWM_C, PWM_E, "PCA9685  (815)",
        "addr 0x40 · no Qwiic socket\n"
        "VCC = header 3V3 · V+ open\n"
        "OE open · 220 ohm series · 1000 Hz\n"
        "ch 8–10 wired · ch 11 open",
        tsize=9.4, ssize=6.4,
    )
    box(
        2, 55.6, 30, 13.2, NOTE_C, NOTE_E, "Same I2C peripheral",
        "Header SDA/SCL = GPIO 23/22,\n"
        "the Qwiic connector's bus.\n"
        "Button and MPRLS stay on\n"
        "the Qwiic cable.",
        tsize=8.6, ssize=6.3,
    )
    box(
        68, 55.6, 30, 13.2, MAP_C, MAP_E, "Channel → enable → pair",
        "8   RIGHT ENA/INA  OUT1/2 valve\n"
        "9   RIGHT ENB/INB  OUT3/4 right pump\n"
        "10  LEFT  ENA/INA  OUT1/2 left pump\n"
        "11  not connected · parked off",
        tsize=8.4, ssize=6.3,
    )

    header = [
        ("GND", 8, 40, GROUND, 73.4),
        ("3V3", 14, 48, HDR_3V3, 72.2),
        ("SDA", 20, 56, HDR, 71.0),
        ("SCL", 26, 64, HDR, 69.8),
    ]
    esp_bottom = 75.2
    pca_top = 55.6 + 13.2
    for label, src_x, dst_x, color, lane_y in header:
        wire([(src_x, esp_bottom), (src_x, lane_y), (dst_x, lane_y), (dst_x, pca_top)], color, lw=2.0)
        node(src_x, esp_bottom, color)
        node(dst_x, pca_top, color)
        ax.text(dst_x, pca_top + 0.25, label, ha="center", va="bottom", fontsize=5.6,
                color=color, fontweight="bold")
    ax.text(
        50, 74.35, "header jumpers — GND,  3V3,  SDA GPIO 23,  SCL GPIO 22",
        ha="center", va="center", fontsize=7.0, color=HDR, fontweight="bold",
    )

    # ---- Drivers and 12 V brick ---------------------------------------------
    l298_y, l298_h = 31.4, 16.6
    l298_top = l298_y + l298_h
    box(
        2, l298_y, 20, l298_h, PWR_C, PWR_E, "12V wall brick",
        "one supply\n"
        "+  →  both VS\n"
        "−  →  common GND\n"
        "ESP32, PCA9685,\n"
        "both L298N",
        tsize=8.6, ssize=6.2,
    )
    box(
        24, l298_y, 36, l298_h, L298_C, L298_E, "LEFT L298N",
        "one pump · remove ENA and ENB jumpers\n"
        "ENA (INA) ch 10 → OUT1/OUT2 left pump\n"
        "ENB (INB) → GND · not channel 11\n"
        "5V OUT → IN1 · GND → IN2\n"
        "IN3 + IN4 → GND · OUT3/OUT4 unused",
        tsize=9.0, ssize=6.0,
    )
    box(
        64, l298_y, 34, l298_h, L298_C, L298_E, "RIGHT L298N",
        "valve + right pump · remove both jumpers\n"
        "ENA (INA) ch 8 → OUT1/OUT2 valve\n"
        "ENB (INB) ch 9 → OUT3/OUT4 right pump\n"
        "5V OUT → IN1 · GND → IN2 (valve)\n"
        "5V OUT → IN3 · GND → IN4 (pump)",
        tsize=9.0, ssize=6.0,
    )

    plus_y = l298_y + 3.15
    ground_y = l298_y + 1.35
    wire([(22, plus_y), (24, plus_y)], POWER, lw=2.6)
    wire([(60, plus_y), (64, plus_y)], POWER, lw=2.6)
    wire([(22, ground_y), (24, ground_y)], GROUND, lw=2.6)
    wire([(60, ground_y), (64, ground_y)], GROUND, lw=2.6)
    for edge in (24, 64):
        node(edge, plus_y, POWER)
        node(edge, ground_y, GROUND)
    ax.text(22.9, plus_y + 0.85, "+12V", ha="center", va="bottom", fontsize=5.5,
            color=POWER, fontweight="bold")
    ax.text(61.9, plus_y + 0.85, "+12V", ha="center", va="bottom", fontsize=5.5,
            color=POWER, fontweight="bold")
    ax.text(22.9, ground_y - 0.15, "GND", ha="center", va="top", fontsize=5.5,
            color=GROUND, fontweight="bold")
    ax.text(61.9, ground_y - 0.15, "GND", ha="center", va="top", fontsize=5.5,
            color=GROUND, fontweight="bold")

    # Header GND continues to the brick ground.
    wire([(8, esp_bottom), (1.15, esp_bottom), (1.15, ground_y), (2, ground_y)], GROUND, lw=1.5, dashed=True)
    ax.text(2.4, 51.6, "common GND", ha="left", va="center", fontsize=6.0,
            color=GROUND, fontweight="bold")

    channels = {
        "10": (56, 36, 53.8, False, "ch 10\nENA"),
        "8":  (40, 74, 51.8, False, "ch 8\nENA"),
        "9":  (48, 90, 50.2, False, "ch 9\nENB"),
    }
    pca_bottom = 55.6
    for _label, (src_x, dst_x, lane_y, dashed, caption) in channels.items():
        color = UNUSED_E if dashed else CTRL
        wire([(src_x, pca_bottom), (src_x, lane_y), (dst_x, lane_y), (dst_x, l298_top)],
             color, lw=2.1, dashed=dashed)
        node(src_x, pca_bottom, color)
        node(dst_x, l298_top, color)
        ax.text(dst_x + 1.3, l298_top - 0.15, caption, ha="left", va="top",
                fontsize=5.3, color=color, fontweight="bold", linespacing=0.95)
    # Channel 11 is parked full-off in firmware and is not wired to either L298N.
    wire([(62, pca_bottom), (62, 53.15)], UNUSED_E, lw=2.1, dashed=True)
    node(62, pca_bottom, UNUSED_E)
    ax.text(63.1, 53.15, "ch 11\nnot connected", ha="left", va="top",
            fontsize=5.3, color=UNUSED_E, fontweight="bold", linespacing=0.95)

    # ---- Actuators -----------------------------------------------------------
    act_y, act_h = 15.6, 12.2
    actuators = [
        (28.5, 15, "LEFT PUMP", "PUMP_1 retract / vacuum\nchannel 10 · LEFT ENA\nOUT1 / OUT2", False),
        (44.5, 14, "UNUSED", "LEFT OUT3 / OUT4\nENB (INB) tied to GND\nchannel 11 not connected", True),
        (66.5, 14, "FA0520E", "channel 8 · RIGHT ENA\nOUT1 / OUT2\npowered = blow (plastic)\nreleased = suck (metal)", False),
        (82.5, 14.5, "RIGHT PUMP", "PUMP_2 extrude / pressure\nchannel 9 · RIGHT ENB\nOUT3 / OUT4", False),
    ]
    for x, w, title, sub, dashed in actuators:
        fc, ec = (UNUSED_C, UNUSED_E) if dashed else (ACT_C, ACT_E)
        box(x, act_y, w, act_h, fc, ec, title, sub, tsize=8.0, ssize=5.8, dashed=dashed)

    act_top = act_y + act_h
    # (L298N pin, actuator pin, label, unused)
    output_wires = [
        ((32, l298_y), 31.5, "OUT1", False),
        ((37, l298_y), 36.5, "OUT2", False),
        ((48, l298_y), 48.5, "OUT3", True),
        ((53, l298_y), 53.5, "OUT4", True),
        ((70, l298_y), 70.5, "OUT1", False),
        ((75, l298_y), 75.5, "OUT2", False),
        ((86, l298_y), 86.5, "OUT3", False),
        ((91, l298_y), 91.5, "OUT4", False),
    ]
    for (ox, oy), act_x, label, dashed in output_wires:
        color = UNUSED_E if dashed else DRIVE
        wire([(ox, oy), (ox, 29.6), (act_x, 29.6), (act_x, act_top)], color, lw=1.7, dashed=dashed)
        node(ox, oy, color)
        node(act_x, act_top, color)
        ax.text(ox, oy - 0.55, label, ha="center", va="top", fontsize=5.0,
                color=color, fontweight="bold")

    ax.add_patch(FancyBboxPatch(
        (4, 1.0), 92, 13.2, boxstyle="round,pad=0.25,rounding_size=0.8",
        linewidth=1.3, edgecolor=WARN, facecolor="#fff1f2", zorder=3,
    ))
    ax.text(
        50, 13.4, "PCA9685 outputs and the valve",
        ha="center", va="top", fontsize=8.2, color=WARN, fontweight="bold", zorder=4,
    )
    ax.text(
        50, 7.3,
        "Product 815 has 220 ohm resistors in series with its outputs.\n"
        "Channel 8 full-on is the plastic pole (pressure). Full-off is the metal pole (vacuum).\n"
        "A finished REP leaves channel 8 full-on until the sketch releases it.\n"
        "Remove the ENA and ENB caps. LEFT ENB is GND. Channel 11 is not connected; firmware parks it full-off.\n"
        "V+ and OE stay open. Never put 12 V on the PCA9685. Never tie L298N 5V OUT to 3.3 V.",
        ha="center", va="center", fontsize=7.0, color="#3f1220", zorder=4, linespacing=1.4,
    )

    out_path.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(out_path, dpi=160)
    plt.close(fig)
    print(f"wrote {out_path}")


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--outdir", default=None, help="output directory (default: this script's directory)")
    args = p.parse_args()
    out_dir = Path(args.outdir).expanduser().resolve() if args.outdir else HERE
    _render(out_dir / "wiring-diagram.png")


if __name__ == "__main__":
    main()
