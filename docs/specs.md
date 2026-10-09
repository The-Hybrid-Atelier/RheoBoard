# Specifications

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

Figures on this page are already written in the [bill of materials](../BuildYourOwn/hardware/README.md), the [build guide](../BuildYourOwn/README.md), the [print notes](../BuildYourOwn/cad/encloser/README.md), and the [firmware reference](../firmware/README.md).

## System

From the build guide:

This simple rheometer is a pneumatic retraction-extrusion system with 2 air pumps + 1 valve on a 3D-printed panel inside a 3D-printed enclosure, driven by an ESP32 over BLE and sensed by a Qwiic MicroPressure sensor.

The sensing routine runs a REP (retraction-extrusion pulse) on command and streams a pressure trace.

Basic soldering is not required if the boards already have headers and the actuator leads are prepared.

## Panel and enclosure

From the [print notes](../BuildYourOwn/cad/encloser/README.md):

- Material: PLA, printed on a Bambu Lab printer with the normal profile
- Parts: base panel about 170 × 170 × 4 mm with 4 mm holes, pump holders about 42.5 × 45 × 81 mm,
  a small bracket, the enclosure body about 195 mm wide, and a lid about 195 × 195 × 29 mm
  (sizes approximate, from the STEP geometry)
- Placement, from the renders: the two pumps stand upright in printed holders, with the valve
  between them and the L298N drivers behind the pumps

Fasteners are 4 mm zip ties. A count is not stated. TODO: overall assembled size, and placement of
the MPRLS, seesaw, ESP32, and Button. Those board positions stay open until the enclosure CAD is
updated. The files stay in [`BuildYourOwn/cad/encloser/`](../BuildYourOwn/cad/encloser/).

Component sizes, from the bill of materials and the component datasheets:

- Pumps: Adafruit 4699 / ZR370-02PM, **58.2 × Ø27.0 mm nominal**
- VALVE2: Adafruit 4663 / FA0520E, **36.02 × 14.5 mm** top-view envelope
- ATtiny1616 seesaw: **25.5 × 17.8 mm**
- ESP32 Thing Plus WRL-15663: **64.77 × 22.86 mm**
- SparkFun MPRLS and Qwiic Button: **25.4 × 25.4 mm** each
- L298N modules: generic module dimensions vary; check your exact boards

## Bill of materials

Quantities, sources, and datasheets are in the [parts list](../BuildYourOwn/hardware/README.md). That file is the one parts list (parts to buy and parts to 3D print).

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
