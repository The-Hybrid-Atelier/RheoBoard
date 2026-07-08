# 2P1VX Firmware API

2 pumps, 1 valve bench rig (SparkFun ESP32 Thing Plus + L298N).  
All parameters are runtime-settable over BLE without reflashing.

---

## Control

| OSC address | Description |
|---|---|
| `rheo/rep` | Trigger one REP (retract → extrude pulse). Ignored if a REP is already running. |
| `rheo/rep/triad` | Trigger three REPs back-to-back, `rheo/rep/interval` ms apart. Ignored if a REP/triad is already running. |
| `rheo/stop` | Abort the current REP early. Also cancels an in-progress triad (later REPs in the sequence are not run). |
| `rheo/purge` | Push the line clear with the pressure pump, then stop. |
| `rheo/api` | Print all supported OSC messages to Serial and send each back over BLE. |

---

## Sampling

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/sense/rate` | `[<ms>]` | 10 | Pressure sampling interval. Clamped to 10–1000 ms. Omit value to read current. |

---

All parameter addresses below accept an optional value. **Omit the value to read the current setting** — the device replies with a `sendSampleOSC` of the same address containing the current value.

## REP sensing routine parameters

All addresses below are scoped under `rheo/rep/` so that future sensing routines can have their own namespaces without collision.

### Pull (retract — PUMP1, vacuum)

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/pull/power` | `[<0–100>]` | 54 | Retract pump drive level (%). Clamped to 0–100. |
| `rheo/rep/pull/time` | `[<ms>]` | 315 | Retract pump run duration. Clamped to 1–5000 ms. |

### Push (extrude — PUMP2, pressure)

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/push/power` | `[<0–100>]` | 100 | Extrude ramp ceiling (%). Clamped to 0–100. |
| `rheo/rep/push/time` | `[<ms>]` | 345 | Extrude pump run duration. Clamped to 1–5000 ms. |
| `rheo/rep/push/ramp/start` | `[<0–100>]` | 40 | Extrude ramp start % (drive level at pump-on). Clamped to 0–100. |
| `rheo/rep/push/ramp/time` | `[<ms>]` | 300 | Duration over which drive climbs from `ramp/start` to `power`. 0 = instant (no ramp). Clamped to 0–5000 ms. |

The extrude drive follows a linear ramp from `ramp/start` → `power` over `ramp/time` ms, then holds `power` for the remainder of `push/time`.

### Baseline

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/baseline/time` | `[<ms>]` | 420 | Idle ambient-sampling window before retract begins. Clamped to 0–5000 ms. |

### Triad

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/interval` | `[<ms>]` | 500 | Gap between REPs when running `rheo/rep/triad` (from one REP's auto-stop to the next REP's start). Clamped to 0–5000 ms. |

---

## Data path (device → bridge)

These are sent by the firmware; the bridge listens for them.

| OSC address | Description |
|---|---|
| `/rheo/sense/air` | Single pressure sample (Pa). Streamed while recording. |
| `/db/start` | Signals start of a REP recording; bridge arms the buffer. |
| `/db/save` | Signals end of a REP recording; bridge flushes buffer to DB. |

---

## REP timing

A full REP window runs `BASELINE → RETRACT → EXTRUDE → RELAX`. Total window is fixed at 1500 ms (`REP_TIME` in `params.h`); only the phase durations are runtime-tunable.

```
0 ms        rep/baseline/time      +VALVE_SETTLE+rep/pull/time  +VALVE_SETTLE+rep/push/time  1500 ms
|────── baseline (pumps off) ──────|──── retract ────|──────────── extrude ────────|── relax ──|
```

The relax tail is implicit: `REP_TIME − (baseline + settle + rep/pull/time + settle + rep/push/time)`.

---

## Bench serial commands (USB, `SERIAL_STREAM` only)

| Command | Description |
|---|---|
| `REP` | Trigger one REP (same as `rheo/rep`). |
| `STOP` | Abort REP or pump test. |
| `PUMP1 <pct>` | Run PUMP1 (vacuum) continuously at `<pct>`%; streams `#S` pressure lines. |
| `PUMP2 <pct>` | Run PUMP2 (pressure) continuously at `<pct>`%; streams `#S` pressure lines. |

Serial output uses `#`-prefixed lines: `#REP_START`, `#REP_END`, `#PH,<ms>,<phase>`, `#S,<ms>,<Pa>`, `#TEST_START`, `#TEST_END`.

---

## Qwiic button gestures

| Gesture | Action |
|---|---|
| Single click | Trigger one REP |
| Double click | Toggle latched vacuum (SUCK) |
| Hold | Momentary pressure (BLOW) while held |
