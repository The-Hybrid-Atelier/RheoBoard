# Electronic wiring

This folder is the electronic wiring diagram for RheoBoardOTS - DIY. Step 02 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page is the connection sequence.

![Electrical wiring diagram](wiring-diagram.png)

The published file is `wiring-diagram.png`. `wiring_diagram.py` generates that PNG. `matplotlib` is required to run the script. Run it from this folder. It writes `wiring-diagram.png`.

The figure shows the ESP32 Thing Plus, the PCA9685 (Adafruit product 815) on four header jumpers, two L298N modules, the pumps, and the valve. Two Qwiic cables join the ESP32, the Qwiic Button, and the MicroPressure sensor. The PCA9685 is not on that chain.

PCA9685 VCC comes from ESP32 3V3. GND is on the common ground. SDA is GPIO 23. SCL is GPIO 22. The address is `0x40`. There are no address jumpers. V+ and OE stay open. Product 815 has 220 ohm resistors in series with its outputs.

Channel 10 drives the left L298N ENA: PUMP1 on OUT1/OUT2. Channel 9 drives the right L298N ENB: PUMP2 on OUT3/OUT4. Channel 8 drives the right L298N ENA: the valve on OUT1/OUT2. Channel 11 is not wired. A finished REP leaves channel 8 full-on until the sketch releases it.

The channel defines are in [`PneumaticSystem.h`](../../../../code/firmware/RheoBoardOTS/PneumaticSystem.h).

Hookup-wire type and gauge are not stated. The wall-adapter connection to the L298N inputs is in [4. Connect the power wiring](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#connect-the-power-wiring), and the continuity pairs and expected result are in [5. Check continuity](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions#check-continuity). A leak check and a photo of the finished wiring are not in this folder yet.

License: CERN-OHL-W-2.0 — see [`LICENSE`](../../../../LICENSE).
