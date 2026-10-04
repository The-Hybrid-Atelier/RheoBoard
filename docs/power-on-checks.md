# Power-on checks

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

From [Step 04 — Power-on test](../BuildYourOwn/README.md#step-04-power-on-test) in the build guide:

1. Place the panel on a stable surface; inspect tubing and leave slack in both power cables.
2. Leave the 12 V adapter unplugged. Confirm ESP32 GND, both L298N GNDs, and the adapter (−) are tied together; confirm adapter (+) reaches both L298N motor power inputs.
3. Power the ESP32 via micro-USB. Never back-feed 12 V into it.
4. Open Serial Monitor at 115200 and confirm the MPRLS, Qwiic Button, and seesaw board are found (no "not found on Qwiic bus" or HAL-init error).
5. With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/valve movement, excessive current draw, or heat. Disconnect immediately if any appears.
6. Confirm the MPRLS reads near ambient, then connect RheoData to `2P1V_Adafruit`.
