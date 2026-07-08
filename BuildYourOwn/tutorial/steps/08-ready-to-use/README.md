# Step 08: Ready to use

- **Time:** ~10 min to confirm every control path works
- **Difficulty:** easy

First measurement workflow, controls overview, and troubleshooting pointers — what the builder
should be able to do once every prior step is complete. By the end of this step you should have
triggered a REP from all three control paths at least once.

## What you'll need for this step

- **Parts:** none beyond the completed, powered-on build (steps 01–07 done, firmware running,
  BLE advertising as confirmed in step 06).
- **Design files:** none.
- **Tools:** RheoData (BLE) and/or a serial monitor (USB) for the workflow below. Full OSC/serial
  API: [`../../../software/rheometer-firmware/README.md`](../../../software/rheometer-firmware/README.md).

## Instructions

### 1. Trigger your first REP, three ways

A **REP** is one retract → extrude pulse (baseline → retract/PUMP1 → extrude/PUMP2 → relax,
1500 ms total — see step 07 for tuning). Confirm each trigger path works at least once:

| Path | How |
|---|---|
| **BLE (RheoData)** | Send OSC `rheo/rep` |
| **Onboard button** | Press the ESP32's boot button (GPIO 0) once |
| **Qwiic Button** | Single click |
| **USB serial (bench)** | Type `REP` + Enter in Serial Monitor @ 115200 |

The onboard LED (GPIO 13) lights for the duration of the REP — a quick visual confirmation
without needing Serial Monitor open.

### 2. Controls overview

| Control | Action |
|---|---|
| `rheo/rep` (BLE) / `REP` (serial) / onboard button / Qwiic single-click | Trigger one REP |
| `rheo/rep/triad` (BLE) | Three REPs back-to-back, `rheo/rep/interval` ms apart (default 500 ms) |
| `rheo/stop` (BLE) / `STOP` (serial) | Abort the current REP or triad early |
| `rheo/purge` (BLE) | Push the line clear with the pressure pump, then stop |
| Qwiic double-click | Toggle **latched suck** (continuous vacuum until toggled off) |
| Qwiic hold | **Momentary blow** (pressure) while held, stops on release |
| `PUMP1 <pct>` / `PUMP2 <pct>` (serial) | Run one pump continuously at `<pct>`% — bench debug only, streams `#S,<ms>,<Pa>` lines |
| `rheo/api` (BLE) | Prints the full supported OSC command list back over BLE/Serial |

Full parameter reference (REP timing, sampling rate, etc.): step 07 and
[`../../../software/rheometer-firmware/README.md`](../../../software/rheometer-firmware/README.md).

### 3. Read a measurement

- **Via RheoData:** connect to device `2P1VX`, trigger `rheo/rep`, and the pressure trace
  (`/rheo/sense/air` samples between `/db/start` and `/db/save`) should appear in whatever
  RheoData's capture/plot view is — see RheoMap/RheoData's own docs for that side once they
  exist (currently TBD in [`../../../product-specs.md`](../../../product-specs.md)).
- **Via USB serial (bench, no RheoData needed):** Serial Monitor @ 115200, type `REP`. You should
  see `#REP_START`, a stream of `#S,<ms>,<Pa>` samples, `#PH,<ms>,<phase>` phase markers, then
  `#REP_END`.

### 4. Sign off

Run the full [`../../../VERIFICATION.md`](../../../VERIFICATION.md) checklist and complete the
human sign-off block at the bottom — this is the final gate before calling the build "done."

## Media

`../../../images/teaser.jpg` has an assembled-rig photo. Still wanted: a photo/video of the rig
mid-REP (LED lit) to show it actually running, not just assembled.

## Tips / common mistakes

- If nothing happens on `rheo/rep` but the onboard button works, check the BLE connection in
  RheoData first — the firmware side is very likely fine.
- The onboard button and Qwiic Button single-click do the *exact same thing* (`rheo.startRep()`)
  — there's no behavioral difference, just convenience depending on what's within reach.
- A REP already in progress ignores a second trigger (BLE, serial, and both buttons all check
  `rheo.recording` first) — this is expected, not a bug; wait for `#REP_END` / the LED to go out.
- Link a troubleshooting FAQ here once real field experience/failure modes accumulate.

## Check before moving on

- [ ] REP successfully triggered from BLE, onboard button, Qwiic Button, and USB serial (all
      four paths)
- [ ] A pressure trace was observed end-to-end (either in RheoData or via serial `#S` lines)
- [ ] Builder can complete a basic measurement workflow without hand-holding
- [ ] `VERIFICATION.md` human sign-off completed
