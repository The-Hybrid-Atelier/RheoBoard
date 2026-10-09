# Firmware

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

The ESP32 sketch is [`2P1V_Adafruit.ino`](../firmware/2P1V_Adafruit/2P1V_Adafruit.ino).
Install steps are in the [Toolchain section](../firmware/README.md#toolchain) of the
firmware reference, [`firmware/README.md`](../firmware/README.md). That firmware, and the RheoData
notes in [`software/README.md`](../software/README.md), apply to the DIY build, the PCB, and the
portable version.

From that README: the entry point is `2P1V_Adafruit/2P1V_Adafruit.ino`, this design's 2 pumps, 1
valve bench firmware for RheoData (SparkFun ESP32 Thing Plus, micro-USB, WRL-15663 + 2× L298N +
Qwiic MicroPressure + Qwiic Button + Adafruit ATtiny1616 seesaw breakout). The sketch file and
BLE device name in code are `2P1V_Adafruit`. USB uploads the sketch. After upload, USB may be
disconnected. Portable power is a single-cell LiPo on the board's JST connector (schematic
V_BATT, 4.2 V maximum). TODO: the maintainer said 3–5 V; the datasheet maximum is 4.2 V. Confirm.
