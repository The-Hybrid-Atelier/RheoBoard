# Specifications

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

Figures on this page are already written in the [bill of materials](../BuildYourOwn/hardware/README.md), the [build guide](../BuildYourOwn/README.md), the [laser-cut panel notes](../BuildYourOwn/laser-cut/README.md), and the [firmware reference](../BuildYourOwn/software/README.md).

## System

From the build guide:

This simple rheometer is a pneumatic retraction-extrusion system with 2 air pumps + 1 valve on a laser-cut acrylic panel, driven by an ESP32 over BLE and sensed by a Qwiic MicroPressure sensor.

The sensing routine runs a REP (retraction-extrusion pulse) on command and streams a pressure trace.

Basic soldering is not required if the boards already have headers and the actuator leads are prepared.

## Panel

From the laser-cut panel notes:

- Material: 3 mm acrylic
- Finished size: 230 × 200 mm
- Mounting: zip ties through the cut slots; four feet at the corner holes

The bill of materials lists the same panel as acrylic, laser-cut, 230 × 200 × 3 mm, with about 18 small zip ties (~2.5 mm wide) and 4 rubber/plastic feet.

Rev C placement, from the laser-cut panel notes:

- Both pumps are rotated 90° with VALVE2 centered between them.
- The two-pump L298N is on the left, the valve L298N is on the right, MPRLS is directly below VALVE2, and the seesaw, ESP32, and Button are behind the drivers.
- PWR, VALVE1, and the former chamber/bulkhead hole are removed.
- This new arrangement has not yet been physically test-fit.

The build guide places L298N #1 (both pumps) on the left, L298N #2 (VALVE2) on the right, and the Button beside the ESP32. It states that Rev C has no PWR, VALVE1, or chamber mounting position.

Physical envelopes used for Rev C placement, from the laser-cut panel notes:

- Pumps: Adafruit 4699 / ZR370-02PM, **58.2 × Ø27.0 mm nominal**; the placement outline uses the tolerance-max 58.3 × 27.2 mm body, rotated 90°
- VALVE2: Adafruit 4663 / FA0520E, **36.02 × 14.5 mm** top-view envelope
- ATtiny1616 seesaw: **25.5 × 17.8 mm**
- ESP32 Thing Plus WRL-15663: **64.77 × 22.86 mm**
- SparkFun MPRLS and Qwiic Button: **25.4 × 25.4 mm** each
- L298N modules: retained placeholder outlines because generic module dimensions vary; test-fit the exact boards before cutting

The pump, valve, and MPRLS envelopes do not reserve pneumatic tube bend radius.

## Bill of materials

Quantities and ratings below are the bill of materials. Part sources and datasheets stay in that file.

| Part | Qty | Rating or note already in the bill of materials |
|---|---|---|
| SparkFun ESP32 Thing Plus (micro-USB, WRL-15663) | 1 | MCU; plain ESP32-WROOM-32D/E (not S2/S3); powers + programs over micro-USB; Qwiic port for sensor chain |
| SparkFun Qwiic MicroPressure (MPRLS) | 1 | I2C `0x18`; Qwiic |
| SparkFun Qwiic Button, red (BOB-15932) | 1 | I2C `0x6F`; Qwiic |
| Adafruit ATtiny1616 Breakout with seesaw, STEMMA QT/Qwiic (PID 5690) | 1 | I2C `0x49`; 3.3 V Qwiic logic; PWM/GPIO output |
| L298N dual H-bridge module | 2 | #1 drives two pumps; #2 drives one valve |
| Air pump / vacuum motor (Adafruit 4699, ZR370-02PM) | 2 | ~4.5 V / ~500 mA each; 2.5 LPM; 58.2 × Ø27.0 mm nominal |
| 6 V air valve (Adafruit 4663, FA0520E) | 1 | 3-port flip selector |
| DC power adapter 12 V | 1 | External supply for both L298N motor rails; ≥ 2 A recommended; share GND with ESP32 |
| Silicone tubing 3 mm ID | 1 | Pneumatic plumbing |
| Qwiic cables | 3 | Four Qwiic boards daisy-chained on one I2C bus |
| micro-USB cable | 1 | Flash `2P1V_Adafruit.ino`; also powers the ESP32 during upload/bench use |

From the bill of materials notes: pumps are rated ~4.5–5 V and the valve ~6 V. Adafruit's ~50% pump duty-cycle recommendation describes intermittent run time, not a 50% PWM ceiling; firmware may use brief higher-PWM pulses, but the pumps should not run continuously.

## Firmware

From the firmware reference. The sketch and BLE device name are `2P1V_Adafruit`. Serial baud is 115200.

I2C addresses:

- Qwiic Button: `0x6F`
- MicroPressure (MPRLS): `0x18`
- ATtiny1616 seesaw: `0x49`

| Seesaw pin | Signal | Drives | Pin type |
|---|---|---|---|
| `0` | `PUMP1_EN` | L298N #1 `ENA` — vacuum pump | PWM |
| `1` | `PUMP2_EN` | L298N #1 `ENB` — pressure pump | PWM |
| `4` | `VALVE1_EN` | Reserved for L298N #2 `ENA`; physically NC | digital |
| `5` | `VALVE2_EN` | L298N #2 `ENB` — the only valve driven | digital |

Qwiic button, from the firmware reference: 1-click = REP, 2-click = latched vacuum, hold = momentary pressure.

A REP runs `BASELINE → RETRACT → EXTRUDE → RELAX` in a fixed 1500 ms window (`REP_TIME`).

| OSC address | Default |
|---|---|
| `rheo/sense/rate` | 10 ms, clamped to 10–1000 ms |
| `rheo/rep/pull/power` | 54 (%) |
| `rheo/rep/pull/time` | 315 ms, clamped to 1–5000 ms |
| `rheo/rep/push/power` | 100 (%) |
| `rheo/rep/push/time` | 345 ms, clamped to 1–5000 ms |
| `rheo/rep/push/ramp/start` | 40 (%) |
| `rheo/rep/push/ramp/time` | 300 ms; `0` disables the ramp |
| `rheo/rep/baseline/time` | 420 ms |
| `rheo/rep/interval` | 500 ms |

The extrude drive ramps from `push/ramp/start` to `push/power` over `push/ramp/time`, then holds at `push/power` for the remainder of `push/time`.

OSC address descriptions, serial commands, and the phase diagram are in the firmware reference.

## Sensor full-scale range and accuracy

TODO

## Assembled mass

TODO

## Parts cost

TODO

## Tube lengths

TODO
