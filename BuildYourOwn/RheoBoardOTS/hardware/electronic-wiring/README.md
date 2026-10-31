# Electronic wiring

This folder contains the electronic wiring diagrams for RheoBoardOTS - DIY. Steps 02–04 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page cover the connection sequence and checks.

**Component wiring**

<img src="rheoboardots-fritzing.png" alt="RheoBoardOTS component wiring with the ESP32, PCA9685, two L298N modules, pumps, valve, and power distribution" width="900">

[Full-size image](rheoboardots-fritzing.png) · [Editable SVG](rheoboardots-fritzing.svg)

**Step details**

[Driver connections and jumper settings](assembly-driver-detail.png) · [Continuity probe example](assembly-continuity-detail.png)

The driver visual accompanies Assembly Step 03. The continuity example is an optional meter reference; Step 04 uses a plain wiring checklist. `python3 assembly_details.py` regenerates the PNG and editable SVG files using matplotlib. They show connections by label; use the component wiring picture for the physical layout.

**Connection reference**

![Electrical wiring diagram](wiring-diagram.png)

The connection reference is `wiring-diagram.png`. `wiring_diagram.py` generates that three-section figure. `matplotlib` is required. Run `python3 wiring_diagram.py` from this folder. It writes `wiring-diagram.png`. Add `--sections` to also export `wiring-control-boards.png`, `wiring-drivers.png`, and `wiring-power-check.png` as optional technical references for assembly.

The figure shows the ESP32 Thing Plus, the PCA9685 (Adafruit product 815) on four header jumpers, two L298N modules, the pumps, and the valve. Two Qwiic cables join the ESP32, the Qwiic Button, and the MicroPressure sensor. The PCA9685 is not on that chain.

PCA9685 VCC comes from ESP32 3V3. GND is on the common ground. SDA is GPIO 23. SCL is GPIO 22. The address is `0x40`. There are no address jumpers. V+ and OE stay open. Product 815 has 220 ohm resistors in series with its outputs.

Channel 10 drives the left L298N ENA: PUMP1 on OUT1/OUT2. Channel 9 drives the right L298N ENB: PUMP2 on OUT3/OUT4. Channel 8 drives the right L298N ENA: the valve on OUT1/OUT2. Channel 11 is not wired. A finished REP leaves channel 8 full-on until the sketch releases it.

The channel defines are in [`PneumaticSystem.h`](../../../../code/firmware/RheoBoardOTS/PneumaticSystem.h).

Hookup-wire type and gauge are not stated. The wall-adapter connection to the L298N inputs is in [Connect the power wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#connect-the-power-wiring), and the pre-power checklist is in [Check wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#check-wiring-before-power-on). A leak check and a photo of the finished wiring are not in this folder yet.
