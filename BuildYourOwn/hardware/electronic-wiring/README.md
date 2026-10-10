# Electronic wiring

This folder is the electronic wiring diagram for RheoBoardOTS - DIY: two pumps, one valve, and the Qwiic chain. Step 02 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page is the connection sequence.

![Electrical wiring diagram](wiring-diagram.png)

The diagram file is `wiring-diagram.png`. It is a labeled schematic with component blocks, named pins, orthogonal wires, junction dots, and a net-color legend. Edit `generate_wiring_diagram.py` and run it to regenerate that image. `matplotlib` is required. `Pillow` is optional and shrinks the PNG. When the wiring changes, update that image and the pin defines in `code/firmware/2P1V_Adafruit/PneumaticSystem.h` together.

Hookup-wire type and gauge are not stated. How the 12 V adapter connects to the L298N inputs, whether by a connector or bare leads, is not stated. Which connections to continuity-check, and the expected result for each, are not stated. A leak check and a photo of the finished wiring are not in this folder yet.

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).
