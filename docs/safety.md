# Safety

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

Hazards below are the [Open Know-How health and safety notice](../okh-RheoBoard.yml) and the [build guide](../BuildYourOwn/README.md). The pump duty-cycle sentence is the one already in the [bill of materials](../BuildYourOwn/hardware/README.md).

## 12 V adapter

From the health and safety notice: the build involves a mains-powered 12 V DC adapter. Risk: electric shock from mishandled mains wiring.

The bill of materials lists that adapter as the external supply for both L298N motor rails, ≥ 2 A recommended, sharing GND with the ESP32.

From the build guide:

- Leave micro-USB and the 12 V adapter disconnected while connecting electronics and tubing.
- Leave the 12 V adapter unplugged until the Step 04 ground and motor-power checks.
- Power the ESP32 via micro-USB for upload and for the Step 04 test. Never back-feed 12 V into it.
- After upload, USB may be disconnected.
- With the firmware at safe idle, plug in the 12 V adapter while watching for unexpected pump/valve movement, excessive current draw, or heat. Disconnect immediately if any appears.

Everyday wall power for the DIY build and for the PCB is that 12 V plug.

## Portable battery

Portable power for the SparkFun ESP32 Thing Plus is a single-cell LiPo on its JST connector. The vendored schematic says V_BATT should be a single-cell LiPo, 4.2 V maximum. The graphical datasheet labels that connector "JST Connector for single cell LiPo" and says VBAT is direct to the battery and the charger.

TODO: the maintainer said 3–5 V for this battery. The datasheet maximum is 4.2 V. Confirm before using another voltage.

## Soldering

From the health and safety notice: the build involves soldered wiring. Risk: burns from soldering.

From the build guide: basic soldering is not required if the boards already have headers and the actuator leads are prepared. A soldering iron and solder are used only if headers or wire leads are not already fitted.

## Pneumatic pressure

From the health and safety notice: the build involves generated pneumatic pressure/vacuum. Pressurized-air hazards: do not point tubing/nozzles at eyes; keep pressures modest.

## ESP32 5 V pin

From the health and safety notice: never route pump/valve current through the ESP32's 5 V pin.

From the build guide: no pump or valve is powered from the ESP32.

## Pumps on the 12 V rail

From the health and safety notice: air pumps are ~4.5 V parts on a 12 V rail — limit duty cycle to avoid overheating.

From the bill of materials: pumps are rated ~4.5–5 V and the valve ~6 V. Adafruit's ~50% pump duty-cycle recommendation describes intermittent run time, not a 50% PWM ceiling; firmware may use brief higher-PWM pulses, but the pumps should not run continuously.
