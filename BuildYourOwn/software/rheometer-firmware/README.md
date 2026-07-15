# Firmware API

This simple rheometer's firmware — 2 pumps, 1 valve bench rig (SparkFun ESP32 Thing Plus + L298N),
with an **Adafruit ATtiny1616 Breakout (seesaw, STEMMA QT / Qwiic)** inserted between the ESP32
and the two L298N drivers. The sketch file is named `2P1V_Adafruit.ino` and the compiled firmware
advertises itself over BLE as device `2P1V_Adafruit` — look for that name when connecting from
RheoData. All parameters are runtime-settable over BLE without reflashing.

---

## Hardware / wiring

The ESP32 Thing Plus's single Qwiic (I2C) bus is daisy-chained to three boards:

1. **SparkFun Qwiic Button** — manual REP / suck-toggle / blow gestures, address `0x6F`
2. **SparkFun MicroPressure (MPRLS)** — REP pressure sensing, address `0x18`
3. **Adafruit ATtiny1616 Breakout (seesaw)** — pump/valve control, address `0x49` (factory default)

No address jumpers to change — `0x49`/`0x18`/`0x6F` don't collide, so this is a single-board,
zero-configuration addition to the Qwiic chain.

The seesaw board supplies three connected control signals that earlier builds sourced from the
ESP32: L298N #1 `ENA`/`ENB` and L298N #2 `ENB`. **These remain discrete point-to-point wires**:
the ESP32 only needs its one Qwiic cable, but seesaw pins `0`, `1`, and `5` run directly to the
L298N boards. Qwiic does not carry these signals. Seesaw pin `4` remains reserved in firmware
for a possible VALVE1 channel but is physically NC in this single-valve build.

| Seesaw pin | Signal | Drives | Pin type |
|---|---|---|---|
| `0` | `PUMP1_EN` | L298N #1 `ENA` — vacuum pump | PWM |
| `1` | `PUMP2_EN` | L298N #1 `ENB` — pressure pump | PWM |
| `4` | `VALVE1_EN` | Reserved for L298N #2 `ENA`; physically NC in this build | digital |
| `5` | `VALVE2_EN` | L298N #2 `ENB` — the only valve driven (flip selector) | digital |

L298N #2 uses only Motor B: `ENB` → seesaw pin `5`, `IN3` → +5V, `IN4` → GND, and
`OUT3/OUT4` → VALVE2. Its unused Motor A terminals (`ENA`, `IN1`, `IN2`, `OUT1/OUT2`) are NC.
On both L298N modules, keep the separate `5V-EN` regulator jumper ON and remove the ENA/ENB
jumper caps. Use each module's own +5 V output for its direction inputs; do not parallel the
two +5 V outputs or apply external 5 V while `5V-EN` is installed.

### ⚠️ Qwiic is signal-only — L298N motor power still needs its own wires

The Qwiic cable only carries I2C commands and 3.3V logic power for the seesaw board's own
microcontroller. It does **not** carry the current that drives the pumps/valve — that's still the
L298N boards' job, wired to the 12V supply same as always. The L298N boards' motor-power wiring
and the actuator leads out are unaffected by this build; only the *signal* path (`ENA`/`ENB`)
moved off native ESP32 pins.

**Why this build keeps proportional control:** the Adafruit ATtiny1616 breakout is not a plain
digital expander — it's a real microcontroller running Adafruit's "seesaw" firmware, which
exposes **real 8-bit PWM over I2C** (`Adafruit_seesaw::analogWrite`, `setPWMFreq`) on 5 of its 12
GPIO pins. That's functionally the same drive-%/duration/ramp control that direct ESP32 LEDC PWM
gave earlier builds, just issued over I2C instead of a native PWM pin. So `RheoSystem.h`/
`RheoSystem.cpp` and every REP percentage/ramp parameter are unchanged — only the low-level
primitives in `PneumaticSystem.h`/`.cpp` (`pumpSetPct`, `pumpSet`, `valveBlow`, `valveSuck`,
`halBegin`) changed, to call `ss.analogWrite()` / `ss.digitalWrite()` instead of `ledcWrite()` /
`digitalWrite()`.

**Arduino library:** requires the
[Adafruit seesaw Library](https://github.com/adafruit/Adafruit_Seesaw)
(`Adafruit_seesaw.h`, class `Adafruit_seesaw`) — install via Library
Manager: search "Adafruit seesaw Library".

**Power note:** the seesaw board's own logic is powered from the Qwiic 3.3 V and GND conductors,
so its separate `Vin` header pin is NC and its `GND` pin is on the system's common-ground net.
This logic supply is electrically separate from the L298N motor-supply rail — same isolation the
ESP32 pins always had from the 12V rail. No voltage derating is needed; this build keeps the existing 12V L298N supply
exactly as-is.

See
[`../../hardware/electronic-wiring/wiring-diagram.png`](../../hardware/electronic-wiring/wiring-diagram.png)
for the full connection diagram.

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

All addresses below are scoped under `rheo/rep/` so that future sensing routines can have their own namespaces without collision. Full proportional power/ramp control, not just timing.

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

A full REP window runs `BASELINE → RETRACT → EXTRUDE → RELAX`. Total window is fixed at 1500 ms (`REP_TIME` in `RheoSystem.h`); only the phase durations are runtime-tunable.

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
