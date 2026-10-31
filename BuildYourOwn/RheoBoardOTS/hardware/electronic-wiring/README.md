# Electronic wiring

This folder contains the electronic wiring diagrams for RheoBoardOTS - DIY. Steps 02–04 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page cover the connection sequence and checks.

**Component wiring**

Current panel connections. PCA9685 GND is supplied through Qwiic; both L298N GND terminals return to adapter negative through the breadboard.

<img src="rheoboardots-fritzing.png?v=3b3a40abf3ba" alt="RheoBoardOTS component wiring with the ESP32, PCA9685, two L298N modules, pumps, valve, and power distribution" width="900">

[Full-size image](https://raw.githubusercontent.com/The-Hybrid-Atelier/RheoBoard/main/BuildYourOwn/RheoBoardOTS/hardware/electronic-wiring/rheoboardots-fritzing.png?v=3b3a40abf3ba) · [Editable SVG](rheoboardots-fritzing.svg)

**Step details**

[Driver connections and jumper settings](assembly-driver-detail.png) · [First power-on](assembly-first-power-on.png) · [Optional continuity probe example](assembly-continuity-detail.png)

The photo-based driver and power visuals accompany Assembly Steps 03 and 07. `python3 assembly_photo_figures.py` regenerates their PNG and editable SVG files using matplotlib. Supplier photos identify the jumper caps, PWM pins, USB socket and motor-power terminals; use the component wiring picture for the complete wiring. Embedded photos retain their owners’ terms; sources are listed in [Images](../../images/README.md).

The continuity example is an optional meter reference; Step 04 uses a plain wiring checklist. `python3 assembly_details.py` regenerates that reference.

**Connection reference**

![Electrical wiring diagram](https://raw.githubusercontent.com/The-Hybrid-Atelier/RheoBoard/main/BuildYourOwn/RheoBoardOTS/hardware/electronic-wiring/wiring-diagram.png?v=2a2f03b8c41f)

The connection reference is `wiring-diagram.png`. `wiring_diagram.py` generates that three-section figure. `matplotlib` is required. Run `python3 wiring_diagram.py` from this folder. It writes `wiring-diagram.png`. Add `--sections` to also export `wiring-control-boards.png`, `wiring-drivers.png`, and `wiring-power-check.png` as optional technical references for assembly.

The figure shows the ESP32 Thing Plus, PCA9685 (Adafruit product 815), two L298N modules, pumps, and valve. Two Qwiic cables connect ESP32 → Qwiic Button → MicroPressure. A Qwiic-to-jumper cable extends the bus from the sensor’s spare Qwiic port to the PCA9685.

Connect the breakout cable’s black wire to GND, red to VCC (3.3 V), blue to SDA, and yellow to SCL. This is the ESP32 I2C bus on SDA GPIO 23 and SCL GPIO 22. The address is `0x40`. There are no address jumpers. V+ and OE stay open. Product 815 has 220 ohm resistors in series with its outputs.

Channel 10 drives the left L298N ENA: PUMP1 on OUT1/OUT2. Channel 9 drives the right L298N ENB: PUMP2 on OUT3/OUT4. Channel 8 drives the right L298N ENA: the valve on OUT1/OUT2. Channel 11 is not wired. A finished REP leaves channel 8 full-on until the sketch releases it.

The channel defines are in [`PneumaticSystem.h`](../../../../code/firmware/RheoBoardOTS/PneumaticSystem.h).

The wall-adapter connection to the L298N inputs is in [Connect the power wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#connect-the-power-wiring), and the pre-power checklist is in [Check wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#check-wiring-before-power-on).
