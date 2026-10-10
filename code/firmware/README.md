# Firmware

**Entry point:** `2P1V_Adafruit/2P1V_Adafruit.ino`. Arduino requires the sketch folder name to match the `.ino` file. The sketch file and BLE device name are `2P1V_Adafruit`.

This firmware is shared by the DIY build, the PCB, and the portable version. Install steps are on the [Code](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Code) wiki page. RheoData notes are in `../software/README.md`. The download, pairing steps, the connected indicator, and how to start and name a REP are not written yet. That gap stays in the software notes.

Firmware is MIT-licensed. The license notice and full text are in [`../../LICENSE`](../../LICENSE).

## Firmware pin map

The firmware expects these I2C addresses:

- Qwiic Button: `0x6F`
- MicroPressure (MPRLS): `0x18`
- ATtiny1616 seesaw: `0x49`

| Seesaw pin | Signal | Drives | Pin type |
|---|---|---|---|
| `0` | `PUMP1_EN` | L298N #1 `ENA` — vacuum pump | PWM |
| `1` | `PUMP2_EN` | L298N #1 `ENB` — pressure pump | PWM |
| `4` | `VALVE1_EN` | Reserved for L298N #2 `ENA`; physically NC | digital |
| `5` | `VALVE2_EN` | L298N #2 `ENB` — the only valve driven | digital |

These constants are defined in `2P1V_Adafruit/PneumaticSystem.h`. Physical connections are in `BuildYourOwn/hardware/electronic-wiring/` and on the Assembly instructions page.

## Control API

| OSC address | Description |
|---|---|
| `rheo/rep` | Trigger one REP. Ignored if a REP is already running. |
| `rheo/rep/triad` | Trigger three REPs, `rheo/rep/interval` ms apart. |
| `rheo/stop` | Abort the current REP and cancel its triad. |
| `rheo/purge` | Push the line clear with the pressure pump, then stop. |
| `rheo/api` | Print supported OSC messages and send each back over BLE. |

### Sampling

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/sense/rate` | `[<ms>]` | 10 | Pressure-sampling interval, clamped to 10–1000 ms. Omit the value to read it. |

All parameter addresses below accept an optional value. Omit it to read the current setting.

### REP parameters

| OSC address | Argument | Default | Description |
|---|---|---|---|
| `rheo/rep/pull/power` | `[<0–100>]` | 54 | Retract pump drive level (%). |
| `rheo/rep/pull/time` | `[<ms>]` | 315 | Retract pump run duration, clamped to 1–5000 ms. |
| `rheo/rep/push/power` | `[<0–100>]` | 100 | Extrude ramp ceiling (%). |
| `rheo/rep/push/time` | `[<ms>]` | 345 | Extrude pump run duration, clamped to 1–5000 ms. |
| `rheo/rep/push/ramp/start` | `[<0–100>]` | 40 | Extrude drive level at pump-on. |
| `rheo/rep/push/ramp/time` | `[<ms>]` | 300 | Linear ramp duration; `0` disables the ramp. |
| `rheo/rep/baseline/time` | `[<ms>]` | 420 | Ambient-sampling window before retract. |
| `rheo/rep/interval` | `[<ms>]` | 500 | Gap between REPs in a triad. |

The extrude drive ramps from `push/ramp/start` to `push/power` over `push/ramp/time`, then holds
at `push/power` for the remainder of `push/time`.

### Data path

| OSC address | Description |
|---|---|
| `/rheo/sense/air` | Pressure sample in Pa, streamed while recording. |
| `/db/start` | Signals the start of a REP recording. |
| `/db/save` | Signals the end of a REP recording. |

## REP timing

A REP runs `BASELINE → RETRACT → EXTRUDE → RELAX` in a fixed 1500 ms window (`REP_TIME` in
`2P1V_Adafruit/RheoSystem.h`). The relax tail is the time remaining after baseline, valve-settle,
pull, and push.

```text
0 ms        baseline       settle + retract       settle + extrude       1500 ms
|──────── pumps off ───────|──── vacuum ────|──────── pressure ────────|─ relax ─|
```

## Bench serial commands

Available over USB when `SERIAL_STREAM` is enabled:

| Command | Description |
|---|---|
| `REP` | Trigger one REP. |
| `STOP` | Abort a REP or pump test. |
| `PUMP1 <pct>` | Run the vacuum pump continuously and stream pressure samples. |
| `PUMP2 <pct>` | Run the pressure pump continuously and stream pressure samples. |

Serial output uses `#REP_START`, `#REP_END`, `#PH,<ms>,<phase>`, `#S,<ms>,<Pa>`,
`#TEST_START`, and `#TEST_END`.
