# Tube wiring

**This simple rheometer** — pneumatic connections for 2 pumps, 1 switched-port valve, the
pressure sensor, and the chamber/nozzle.

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

<a href="tube-connection.png"><img src="tube-connection.png" alt="Pneumatic tube connection diagram" width="600"></a>

Tubing: **3 mm ID silicone** ([Adafruit 4664](https://www.adafruit.com/product/4664) or
equivalent).

## Components

| Label | Part | Port usage |
|---|---|---|
| PUMP1 | [Adafruit 4699](https://www.adafruit.com/product/4699) ZR370-02PM (vacuum) | **Side port** (inlet) → VALVE2 metal pole; **tubing port** (outlet) → atmosphere |
| PUMP2 | Adafruit 4699 ZR370-02PM (pressure) | **Tubing port** (outlet) → VALVE2 plastic pole; **side port** (inlet) → atmosphere |
| VALVE2 | [Adafruit 4663](https://www.adafruit.com/product/4663) FA0520E | Metal pole (OFF) = vacuum path; plastic pole (ON) = pressure path; common → shared line |
| Sensor | [SparkFun Qwiic MicroPressure](https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html) (MPRLS) | Teed into shared line |
| Output | Chamber / nozzle | Bottom of shared line — "the line we sense" |

### Pump note (4699)

The ZR370-02PM always draws air in through the **side port** and pushes it out the **tubing port**.
Reversing motor wires does **not** flip flow direction — retract vs extrude is set by **which port
is plumbed to the valve** and which is open to atmosphere, not by L298N direction wiring.

## Valve logic (VALVE2)

| VALVE2 state | seesaw pin 5 | Path |
|---|---|---|
| OFF (suck / retract) | LOW | Common → metal pole → PUMP1 vacuum |
| ON (blow / extrude) | HIGH | Common → plastic pole → PUMP2 pressure |

Idle pump not selected is disconnected through the valve — no plug or seal needed.

## Flow during a REP

The tube paths change with the firmware phases:

1. **Baseline** — pumps off, ambient pressure sampling.
2. **Retract** — VALVE2 OFF, PUMP1 on (vacuum).
3. **Extrude** — VALVE2 ON, PUMP2 on (pressure, with optional ramp).
4. **Relax** — both pumps stop.

Timing, controls, and parameter defaults are documented in
[`../../software/README.md`](../../software/README.md).

## Diagram source

`tube-connection.png` is a photo/pictogram-style diagram and has no editable generator script yet.
