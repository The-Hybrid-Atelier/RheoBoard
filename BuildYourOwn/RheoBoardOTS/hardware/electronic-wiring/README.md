# Electronic wiring

This folder is the electronic wiring diagram for RheoBoardOTS - DIY: two pumps, one valve, and the Qwiic chain. Step 02 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page is the connection sequence.

![Electrical wiring diagram](wiring-diagram.png)

The published file is `wiring-diagram.png`. On 2026-10-12 it was replaced with the three-section RheoBoardOTS figure. The source of that PNG was a maintainer-supplied image, not `generate_wiring_diagram.py`.

`generate_wiring_diagram.py` still draws the earlier figure. Running it writes `wiring-diagram.png` and would replace the current image with that earlier figure. `matplotlib` is required to run the script. `Pillow` is optional and shrinks the PNG the script writes. Keep the script. Do not run it to recreate the current diagram. When the wiring changes, update the published PNG and the pin defines in `code/firmware/2P1V_Adafruit/PneumaticSystem.h` together. Changing the script does not update the published PNG.

Hookup-wire type and gauge are not stated. The wall-adapter connection to the L298N inputs is in [4. Connect the power wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#connect-the-power-wiring), and the continuity pairs and expected result are in [5. Check continuity](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#check-continuity). `generate_wiring_diagram.py` draws the earlier figure described above. That script does not draw the PRT-10288. A leak check and a photo of the finished wiring are not in this folder yet.

License: CERN-OHL-W-2.0 — see [`LICENSE`](../../../../LICENSE).
