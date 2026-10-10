# Tube wiring

This folder is the pneumatic port table for RheoBoardOTS - DIY: two pumps, one switched-port valve, the pressure sensor, and the chamber or nozzle. Step 02 of the [Assembly instructions](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Assembly-instructions) page summarizes these ports.

License: CERN-OHL-W-2.0 — see [`../../../LICENSE`](../../../LICENSE).

![Pneumatic tube connection diagram](tube-connection.png)

Tubing is Adafruit 4661, 1 m silicone, 3 mm ID, 5 mm OD, for air only. The length of each run is not stated. A tool to cut the silicone tubing is not named in the repository.

## Components

| Label | Part | Port usage |
|---|---|---|
| PUMP1 | Adafruit 4699 ZR370-02PM (vacuum) | **Side port** (inlet) → VALVE2 metal pole; **tubing port** (outlet) → atmosphere |
| PUMP2 | Adafruit 4699 ZR370-02PM (pressure) | **Tubing port** (outlet) → VALVE2 plastic pole; **side port** (inlet) → atmosphere |
| VALVE2 | Adafruit 4663 FA0520E | Metal pole (OFF) = vacuum path; plastic pole (ON) = pressure path; common → shared line |
| Sensor | SparkFun Qwiic MicroPressure (MPRLS) | Teed into shared line |
| Output | Chamber / nozzle | Bottom of shared line — "the line we sense" |

It is not confirmed whether the chamber or nozzle is the printed sensing tube in `BuildYourOwn/cad/connector/`. How the sensing tube, the small connector, and the FTLLB220-6005 fitting join the shared line, which tee to use, and the length of each tube run are not written yet.

### Pump note (4699)

The ZR370-02PM always draws air in through the **side port** and pushes it out the **tubing port**. Reversing motor wires does **not** flip flow direction — retract vs extrude is set by **which port is plumbed to the valve** and which is open to atmosphere, not by L298N direction wiring.

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

Timing, controls, and parameter defaults are in `code/firmware/README.md`.

## Diagram source

`tube-connection.png` is a photo-style diagram and has no editable generator script yet.
