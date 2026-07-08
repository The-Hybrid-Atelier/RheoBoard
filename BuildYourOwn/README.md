# Build Your Own Rheometer — Step-by-Step Guide

An Instructables-style guide that takes someone from zero to a working DIY rheometer build, in
one document. Project overview, features, and full reference material (BOM, wiring, firmware API)
live in the [repo root README](../README.md); this file is the actual build guide it links out to.

_Content written for all 8 steps; none human-verified yet, and no photos/videos exist for any
step — see the per-step status below._

<img src="images/teaser.jpg" alt="This simple rheometer — assembled bench prototype" width="480">

## Overview

You'll build **this simple rheometer**: a benchtop pneumatic "pull-push" measurement head — 2 air
pumps + 1 valve zip-tied to a laser-cut acrylic panel, driven by an ESP32 over BLE, sensed by a
Qwiic MicroPressure sensor. The end result is a device that runs a **REP** (retract → extrude
pulse) on command — from BLE (RheoData), the onboard/Qwiic button, or USB serial — and streams a
pressure trace for each one.

- **Estimated build time:** not yet measured end-to-end by a human build. Summing the per-step
  time estimates below gives **~4–7 hours hands-on** for a first-timer, dominated by Step 03's
  soldering — plus laser-cut turnaround time on top if that's outsourced rather than done
  in-house (Step 02). Treat this as a placeholder until someone times a real build — see
  [`PROGRESS.md`](PROGRESS.md) to log the correction.
- **Difficulty / prerequisite skills:** comfortable with a soldering iron (pull-down resistors,
  screw terminals), basic Arduino IDE usage (installing boards/libraries, flashing firmware), and
  reading a wiring diagram. No custom PCB work, no CAD skills required (the panel is cut from a
  provided file) — but laser cutter access (or a cut-to-order service) is required for Step 02.

## Before you start

- **Materials:** see [`hardware/BOM.md`](hardware/BOM.md).
- **Design files:** see [`laser-cut/`](laser-cut/) for the platform,
  [`hardware/wiring/`](hardware/wiring/) for circuit diagrams.
- **Software:** see [`software/`](software/) for firmware source; flash procedure is Step 04.
- **Tools:**
  - Laser cutter access, or a cut-to-order service that accepts `.svg`/`.dxf` (Step 02 — note the
    vector file itself is still TBD, see [`laser-cut/README.md`](laser-cut/README.md))
  - Small zip-tie/flush cutters (Step 02)
  - Soldering iron + solder, wire strippers, small flathead/Phillips screwdriver for L298N screw
    terminals, multimeter (Step 03, Step 06)
  - Computer with a free USB port, data-capable micro-USB cable (Steps 04, 06)
  - A phone/tablet or computer running **RheoData** for BLE control (Steps 06–08)

## Steps

This list is the source of truth for step status. A step is only `[x]` once it's written **and**
a human has verified it against [`VERIFICATION.md`](VERIFICATION.md)/by building it (see
`AGENTS.md` → "What the agent can and can't verify") — "written" and "verified" are different
things, don't collapse them. **No step has photos or video yet regardless of text status.**

- [ ] [Step 01 — Kit contents and tools](#step-01-kit-contents-and-tools) — written, not verified; no media
- [ ] [Step 02 — Assemble the platform](#step-02-assemble-the-platform) — written, not verified; blocked on laser-cut vector file; no media
- [ ] [Step 03 — Wire the electronics](#step-03-wire-the-electronics) — written, not verified; no media
- [ ] [Step 04 — Install firmware](#step-04-install-firmware) — written, not verified; no media
- [ ] [Step 05 — Mount and set up](#step-05-mount-and-set-up) — written, not verified; blocked on RheoMap's sample/fixture geometry spec; no media
- [ ] [Step 06 — Power and data connections](#step-06-power-and-data-connections) — written, not verified; no media
- [ ] [Step 07 — Calibrate](#step-07-calibrate) — written, not verified; no media
- [ ] [Step 08 — Ready to use](#step-08-ready-to-use) — written, not verified; no media

---

## Step 01: Kit contents and tools

- **Time:** ~15–20 min
- **Difficulty:** easy

**Parts:** full list in [`hardware/BOM.md`](hardware/BOM.md) — verify everything arrived before
starting assembly. **Tools:** see "Before you start" above for the full tool list — this step is
just confirming you have them, not using any of them yet.

**Instructions:**

1. Unpack and inventory every item against [`hardware/BOM.md`](hardware/BOM.md).
2. Note anything missing or substituted — record it in `PROGRESS.md` if it's a design change,
   don't silently substitute.
3. Gather tools listed above under "Before you start."

**Media:** add a photo of laid-out kit contents to [`images/`](images/) once available.

**Tips / common mistakes:** don't start wiring until the BOM inventory is complete — missing a
part mid-build is costly.

**Check before moving on:**

- [ ] Every `hardware/BOM.md` row accounted for (or explicitly marked "not in this revision")
- [ ] Tools gathered

---

## Step 02: Assemble the platform

- **Time:** ~30–45 min hands-on (excludes laser-cutting turnaround, which can be days if
  outsourced to a cut-to-order service)
- **Difficulty:** requires laser cutter access (or a cut-to-order service); assembly itself is
  hand tools only (zip ties, no screws)

**Parts:** acrylic panel, zip ties (~20), Ø10 bulkhead fitting, 4× rubber/plastic feet — see
[`hardware/BOM.md`](hardware/BOM.md). **Design files:** [`laser-cut/`](laser-cut/) — cut the panel
first if not pre-cut. **Note:** as of this writing only a raster design reference exists there
(placement map + cut-geometry preview); the laser-ready vector file (`.svg`/`.dxf`) still needs to
be produced from `laser-cut/panel-cut-lines.png` before this step can actually be cut. **Tools:**
small zip-tie cutters/flush cutters, laser cutter (or cut-to-order service).

**Instructions:**

1. Cut the panel per [`laser-cut/panel-cut-lines.png`](laser-cut/panel-cut-lines.png)
   (290 × 200 × 3 mm acrylic) — once the vector file exists.
2. Attach the 4 corner feet.
3. Install the Ø10 bulkhead fitting at the CHAMBER position.
4. Place and zip-tie each component per the numbered map in
   [`laser-cut/panel-placement-map.png`](laser-cut/panel-placement-map.png) and the table in
   [`laser-cut/README.md`](laser-cut/README.md#component-placement--zip-tie-map): PUMP1/PUMP2
   lying flat, VALVE2 (leave the VALVE1 slot empty — unpopulated in this build), MPRLS + Button
   next to the ESP32, both L298N boards clear of their heatsinks, ESP32, and the power terminal
   block.
5. Don't wire anything yet — this step is mechanical placement only. Electrical wiring is
   [Step 03](#step-03-wire-the-electronics).

**Media:** pictographic or photo sequence strongly recommended (one photo per sub-step).
`laser-cut/panel-placement-map.png` doubles as the primary pictographic reference here.

**Tips / common mistakes:**

- Leave the VALVE1 zip-tie slot empty — it's a reserved position for a future 2-valve (2P2V)
  variant, not part of this build. See `laser-cut/README.md` for why.
- Zip-tie snug but not so tight it deforms the pump/valve housings.

**Check before moving on:**

- [ ] Platform is rigid and square — no wobble that would affect measurements
- [ ] All mechanical parts from this step's BOM rows are installed
- [ ] Component positions match `laser-cut/panel-placement-map.png` (right components, right
      orientation, VALVE1 slot left empty)

---

## Step 03: Wire the electronics

- **Time:** ~2–4 h (breadboard/perfboard, first build)
- **Difficulty:** moderate (soldering pull-downs; L298N screw terminals)

**Parts:** electronics rows in [`hardware/BOM.md`](hardware/BOM.md). **Design files:**
[`hardware/wiring/2P1V-wiring-diagram.png`](hardware/wiring/2P1V-wiring-diagram.png) — this
diagram covers electrical wiring only; pneumatic plumbing is a separate diagram (see below).
**Tools:** soldering iron, wire strippers, multimeter, small screwdriver (L298N terminals).

**Instructions:**

1. **Common GND:** tie ESP32 GND, both L298N GND pins, and 12 V adapter (−) together.
2. **Pull-downs:** solder 10 kΩ from GPIO **14, 15, 32, 33** each to GND on the ESP32.
3. **12 V motor power:** connect adapter (+) to both L298N motor power inputs (+12V / VCC pins per
   module label). Do **not** power pumps or valve from the ESP32 5 V pin.
4. **L298N #1 (pumps):** remove ENA and ENB jumpers. Loop IN1→+5V, IN3→+5V; IN2→GND, IN4→GND.
   Connect ENA→GPIO 32, ENB→GPIO 33. OUT1/OUT2→PUMP1 (4700), OUT3/OUT4→PUMP2 (4700).
5. **L298N #2 (valve):** same direction wiring on the active channel. ENA→GPIO 14 (VALVE2).
   OUT1/OUT2→VALVE2 (4663). ENB/GPIO 15/OUT3/OUT4 may stay unwired.
6. **Qwiic chain:** ESP32 Thing Plus Qwiic port → [MicroPressure sensor](https://www.sparkfun.com/sparkfun-qwiic-micropressure-sensor.html)
   (I2C `0x18`) → [Qwiic Button](https://www.sparkfun.com/sparkfun-qwiic-button.html) (I2C `0x6F`,
   default). Qwiic devices daisy-chain — plug a second Qwiic cable from the sensor's spare port to
   the button.
7. Continuity-check new connections **before** applying 12 V (see [`VERIFICATION.md`](VERIFICATION.md)).

GPIO map must match [`software/rheometer-firmware/PneumaticSystem.h`](software/rheometer-firmware/PneumaticSystem.h).

**Pneumatic plumbing (same step or next):** after electrical wiring, plumb per
[`hardware/wiring/2P1V-tube-connection.png`](hardware/wiring/2P1V-tube-connection.png) and
[`hardware/wiring/pneumatic-plumbing.md`](hardware/wiring/pneumatic-plumbing.md).

**4700 port orientation matters:** PUMP1 side port → valve metal pole (vacuum); PUMP2 tubing port →
valve plastic pole (pressure). Motor polarity does not flip air direction.

**Media:** diagram reference + photo of actual wiring from the same angle as the diagram.

**Tips / common mistakes:**

- L298N ENA/ENB jumpers must be **OFF** when using PWM from the ESP32.
- Pumps are ~4.5 V parts on a 12 V rail — use firmware PWM defaults; avoid 100% duty for long runs
  (Adafruit recommends ~50% duty for the 4700).
- Any deviation from the published diagram must be documented (update `hardware/wiring/` or note
  in `PROGRESS.md`).

**Check before moving on:**

- [ ] Wiring matches `hardware/wiring/2P1V-wiring-diagram.png`
- [ ] Pneumatic plumbing matches `hardware/wiring/2P1V-tube-connection.png` (4700 port orientation)
- [ ] Continuity/short check passed (12 V not applied yet)

---

## Step 04: Install firmware

- **Time:** ~30–60 min (first-time ESP32 toolchain setup)
- **Difficulty:** moderate (requires computer + USB)

Flash the BLE firmware for this design. Source:
[`software/rheometer-firmware/2P1VX.ino`](software/rheometer-firmware/2P1VX.ino).

**Parts:** SparkFun ESP32 Thing Plus (wired but motor supplies can stay off for upload).
**Design files:** none — see [`software/README.md`](software/README.md). **Tools:** computer,
**micro-USB** cable (data-capable, not charge-only), Arduino IDE 2.x.

**Instructions:**

1. Install [Arduino IDE](https://www.arduino.cc/en/software) 2.x.
2. **Boards Manager:** add ESP32 package URL from
   [`hardware/references/README.md`](hardware/references/README.md) (Espressif `esp32` core).
   Install **esp32 by Espressif Systems**.
3. **Libraries** (Library Manager): SparkFun Qwiic Button, SparkFun MicroPressure, OSC (by Adrian
   Freed).
4. **ThingPlusBLEOSC:** not on Library Manager — clone it into your Arduino `libraries/` folder:

   ```bash
   cd ~/Documents/Arduino/libraries
   git clone https://github.com/cearto/ThingPlusBLEOSC.git
   ```

   Restart the Arduino IDE afterward. (Also depends on ESP32 BLE Arduino by Neil Kolban, usually
   bundled with the `esp32` core already.)
5. Open `BuildYourOwn/software/rheometer-firmware/2P1VX.ino` from this repo (or your sketchbook
   copy — keep them in sync).
6. Board: **SparkFun ESP32 Thing Plus** (or generic **ESP32 Dev Module** — this is a plain
   ESP32-WROOM-32D/E, not S2/S3); select the micro-USB serial port.
7. Upload. Open Serial Monitor @ **115200** baud.
8. **Success signal:** boot message includes `2P1VX initialized`. Optional: type `REP` on serial
   (with motor power on and plumbing complete) to exercise the pneumatic cycle.

**Media:** screenshot of board + port selection → save as [`images/ide-settings.png`](images/) when captured.

**Tips / common mistakes:**

- Use a **data-capable** micro-USB cable — charge-only cables won't show a serial port.
- If MPRLS or Button is missing/misaddressed, firmware may still boot but that device's readings
  won't work — check the Qwiic chain order and cable seating.
- Full OSC/API docs: [`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**Check before moving on:**

- [ ] Firmware upload completes without error
- [ ] Serial Monitor shows `2P1VX initialized` @ 115200

---

## Step 05: Mount and set up

- **Time:** ~15–20 min
- **Difficulty:** easy

Mechanical mounting, sensor geometry, and anything that must be fixed in place before power-on
measurements mean anything (stable stand, level surface, orientation). By the end of this step the
rig is physically ready for power — Step 06 turns it on.

**Parts:** the fully assembled panel from Steps 02–03 (platform + zip-tied components +
electrical wiring + pneumatic tubing). **Design files:** [`laser-cut/`](laser-cut/) — panel
dimensions and the chamber/foot positions;
[`hardware/wiring/pneumatic-plumbing.md`](hardware/wiring/pneumatic-plumbing.md) for what the
shared line/chamber connects to. **Tools:** none beyond what's already in hand (no new fasteners —
the panel's 4 corner feet are its only "mount").

**Instructions:**

1. **Place the panel** on a flat, level, stable surface — a workbench, not something that flexes
   or vibrates (a wobbly folding table will show up as noise in the pressure trace). The 4 corner
   feet are the only leveling/isolation the panel has; don't stack anything on top of it.
2. **Check tubing routing:** confirm no line from PUMP1/PUMP2 → VALVE2 → the shared T-connector →
   chamber is kinked, pinched under a zip tie, or under tension from panel placement. A kinked
   line reads as a phantom pressure spike or a dead channel that isn't actually a wiring fault.
3. **Position the chamber/nozzle** (the Ø10 bulkhead fitting — "the line we sense" per
   [`pneumatic-plumbing.md`](hardware/wiring/pneumatic-plumbing.md)) at whatever sample or test
   surface it needs to interface with for your measurement. **This is the one part of this step we
   can't fully spec yet** — the exact sample/fixture geometry (standoff distance, alignment,
   contact angle) is part of the RheoMap product spec, which is still TBD in
   [`product-specs.md`](product-specs.md). Until that spec exists, use judgment: the nozzle should
   reach the sample without the tubing pulling on the panel, and its position should be repeatable
   between measurements (mark it if you'll be moving it between runs). **Update this step once
   RheoMap's fixture spec lands.**
4. **Cable slack:** route the micro-USB and 12 V adapter cables (connected in the next step) with
   enough slack that plugging in doesn't tug on the panel or its feet.
5. **Environment:** avoid direct drafts or a heat source pointed at the chamber/sensor — the REP's
   baseline phase (ambient pressure sampling, default 420 ms — see
   [`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md)) is short but
   not instant, and a sudden draft during that window will bias the whole trace.

**Media:** photo of the fully mounted assembly — panel placement, tubing routing, and chamber
position — before wiring power/data in Step 06.

**Tips / common mistakes:**

- A kinked or pinched tube after mounting is easy to miss and looks like a sensor problem, not a
  mechanical one — physically trace every line from pump to chamber after placing the panel.
- Don't over-tighten zip ties against the panel edge when routing tubing near Step 02's slots —
  crushing the silicone ID changes flow resistance.

**Check before moving on:**

- [ ] Panel sits flat and stable — no wobble that would affect measurements
- [ ] Every pneumatic line traced end-to-end: no kinks, no pinches, no tension
- [ ] Chamber/nozzle positioned at the sample interface (repeatable position noted, if applicable)
- [ ] Power/data cable routing won't stress the panel once connected in Step 06

---

## Step 06: Power and data connections

- **Time:** ~15–30 min
- **Difficulty:** easy

Power this design: **12 V adapter** to both L298N motor rails, plus micro-USB to the ESP32. Data
path: USB serial (bench) and BLE (RheoData).

**Parts:** 12 V DC adapter (≥ 2 A recommended), micro-USB cable, Qwiic MicroPressure + Qwiic
Button already wired — see [`hardware/BOM.md`](hardware/BOM.md). **Design files:** power section
of [`hardware/wiring/2P1V-wiring-diagram.png`](hardware/wiring/2P1V-wiring-diagram.png). **Tools:**
multimeter (recommended).

**Instructions:**

1. **Common ground:** confirm ESP32 GND, both L298N GND, and 12 V adapter (−) are tied together
   before energizing.
2. **Motor power:** connect **12 V (+)** to both L298N motor power inputs. Polarity per module labels.
3. **ESP32:** power via micro-USB (the same cable used for programming). Do not back-feed 12 V
   into the ESP32.
4. **First power-on:** with pumps/valve off (firmware idle), verify no excessive current draw or
   hot components. Then connect Serial Monitor @ 115200 and confirm MPRLS reads (~ambient) and the
   Qwiic Button responds (LED lights on press).
5. **BLE:** from RheoData, connect to device name **`2P1VX`**.

**Media:** photo of final cable routing; short video of power-on if helpful.

**Tips / common mistakes:**

- Never run pump/valve loads from the ESP32 5 V pin.
- 12 V at the L298N is the motor **supply rail**; effective drive to ~4.5 V pumps and ~6 V valve
  is set by PWM duty in firmware — tune `rheo/rep/pull/power` and `rheo/rep/push/power` if motion
  is too aggressive.
- If MPRLS reads are stale or `nan`, reseat the Qwiic cable and confirm I2C address `0x18`.
- USB-only power is fine for firmware upload; pneumatic tests need the 12 V adapter.

**Check before moving on:**

- [ ] Powers up without excessive current draw or component heat
- [ ] Serial pressure read looks plausible at ambient
- [ ] BLE device `2P1VX` visible to host (RheoData)

---

## Step 07: Calibrate

- **Time:** ~20–30 min for an initial tuning pass (ongoing — re-tune per fluid/fixture as needed)
- **Difficulty:** moderate

RheoBoard DIY uses **runtime BLE parameters** rather than a one-shot onboard calibration. Firmware
defaults are tuned for this design's bench rig; adjust per fluid/fixture via RheoData OSC.

**Parts:** assembled rig with firmware running; sample chamber/nozzle plumbed. **Tools:** RheoData
(BLE), optional USB serial for bench commands.

**Instructions:**

1. **Warm-up:** run a few idle REP cycles or wait for MPRLS to stabilize after power-on.
2. **Baseline:** default `rheo/rep/baseline/time` = 420 ms — increase if ambient drift is visible.
3. **Retract (pull):** tune `rheo/rep/pull/power` (default 54%) and `rheo/rep/pull/time`
   (default 315 ms) until retract is consistent without cavitation.
4. **Extrude (push):** tune `rheo/rep/push/power`, `push/time`, and ramp (`push/ramp/start`,
   `push/ramp/time`) — defaults 100%, 345 ms, 40% start, 300 ms ramp.
5. **Sampling:** set `rheo/sense/rate` (default 10 ms) to match RheoData capture needs.
6. **Verify:** trigger `rheo/rep` and confirm pressure trace shape is repeatable across 3 runs
   (`rheo/rep/triad`).

Full parameter list: [`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**Media:** video of calibration procedure recommended (external YouTube hosting is fine).

**Tips / common mistakes:**

- If extrude is too aggressive, lower `push/power` or lengthen `push/ramp/time` before shortening
  `push/time`.
- Valve state must match pump: retract = VALVE2 OFF + PUMP1; extrude = VALVE2 ON + PUMP2 (see
  [`hardware/wiring/pneumatic-plumbing.md`](hardware/wiring/pneumatic-plumbing.md)).

**Check before moving on:**

- [ ] REP parameters documented for your fluid/fixture (screenshot or RheoData preset)
- [ ] Three consecutive REPs produce repeatable pressure traces

---

## Step 08: Ready to use

- **Time:** ~10 min to confirm every control path works
- **Difficulty:** easy

First measurement workflow, controls overview, and troubleshooting pointers — what the builder
should be able to do once every prior step is complete. By the end of this step you should have
triggered a REP from all three control paths at least once.

**Parts:** none beyond the completed, powered-on build (Steps 01–07 done, firmware running, BLE
advertising as confirmed in Step 06). **Tools:** RheoData (BLE) and/or a serial monitor (USB) for
the workflow below. Full OSC/serial API:
[`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**1. Trigger your first REP, three ways**

A **REP** is one retract → extrude pulse (baseline → retract/PUMP1 → extrude/PUMP2 → relax,
1500 ms total — see Step 07 for tuning). Confirm each trigger path works at least once:

| Path | How |
|---|---|
| **BLE (RheoData)** | Send OSC `rheo/rep` |
| **Onboard button** | Press the ESP32's boot button (GPIO 0) once |
| **Qwiic Button** | Single click |
| **USB serial (bench)** | Type `REP` + Enter in Serial Monitor @ 115200 |

The onboard LED (GPIO 13) lights for the duration of the REP — a quick visual confirmation without
needing Serial Monitor open.

**2. Controls overview**

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

Full parameter reference (REP timing, sampling rate, etc.): Step 07 and
[`software/rheometer-firmware/README.md`](software/rheometer-firmware/README.md).

**3. Read a measurement**

- **Via RheoData:** connect to device `2P1VX`, trigger `rheo/rep`, and the pressure trace
  (`/rheo/sense/air` samples between `/db/start` and `/db/save`) should appear in whatever
  RheoData's capture/plot view is — see RheoMap/RheoData's own docs for that side once they exist
  (currently TBD in [`product-specs.md`](product-specs.md)).
- **Via USB serial (bench, no RheoData needed):** Serial Monitor @ 115200, type `REP`. You should
  see `#REP_START`, a stream of `#S,<ms>,<Pa>` samples, `#PH,<ms>,<phase>` phase markers, then
  `#REP_END`.

**4. Sign off**

Run the full [`VERIFICATION.md`](VERIFICATION.md) checklist and complete the human sign-off block
at the bottom — this is the final gate before calling the build "done."

**Media:** [`images/teaser.jpg`](images/teaser.jpg) has an assembled-rig photo. Still wanted: a
photo/video of the rig mid-REP (LED lit) to show it actually running, not just assembled.

**Tips / common mistakes:**

- If nothing happens on `rheo/rep` but the onboard button works, check the BLE connection in
  RheoData first — the firmware side is very likely fine.
- The onboard button and Qwiic Button single-click do the *exact same thing* (`rheo.startRep()`)
  — there's no behavioral difference, just convenience depending on what's within reach.
- A REP already in progress ignores a second trigger (BLE, serial, and both buttons all check
  `rheo.recording` first) — this is expected, not a bug; wait for `#REP_END` / the LED to go out.
- Link a troubleshooting FAQ here once real field experience/failure modes accumulate.

**Check before moving on:**

- [ ] REP successfully triggered from BLE, onboard button, Qwiic Button, and USB serial (all four
      paths)
- [ ] A pressure trace was observed end-to-end (either in RheoData or via serial `#S` lines)
- [ ] Builder can complete a basic measurement workflow without hand-holding
- [ ] `VERIFICATION.md` human sign-off completed

---

## Finished

_(fill in once there's a working build — final photos/video, what the builder should be able to
do with it, troubleshooting/FAQ, calibration notes if applicable.)_

## Tips (all steps, at a glance)

- **(01)** Don't start wiring until every `hardware/BOM.md` row is accounted for — discovering a
  missing part mid-build is far more costly than catching it during inventory.
- **(02)** Leave the VALVE1 zip-tie slot empty — it's a reserved position for a future 2-valve
  variant, not part of this build.
- **(03)** L298N ENA/ENB jumpers must be **removed** — the ESP32 drives those pins with PWM, and a
  jumper would fight it.
- **(03)** Pumps are ~4.5 V parts riding on a 12 V motor rail; effective drive is set by firmware
  PWM duty, not adapter voltage. Don't run either pump at 100% duty continuously — Adafruit rates
  the 4700 for ~50%.
- **(04)** Charge-only micro-USB cables won't expose a serial port — use a known data-capable one.
- **(04)/(06)** If the MPRLS or Qwiic Button seems missing or misbehaving, check Qwiic daisy-chain
  order and reseat the cable before suspecting the part itself.
- **(06)** Never route pump/valve current through the ESP32's 5 V pin — it's a logic supply, not a
  motor rail. Motors get their own 12 V adapter.
- **(07)** If extrude feels too aggressive, lower `rheo/rep/push/power` or lengthen
  `push/ramp/time` before shortening `push/time` — a longer ramp is gentler than a shorter pulse.
- **(08)** The onboard boot button (GPIO 0), the Qwiic Button, BLE (`rheo/rep`), and USB serial
  (`REP`) all trigger the same REP routine — pick whichever's convenient at the bench.

## Media conventions

- **Images:** commit into [`images/`](images/) (PNG/JPG, reasonably sized/compressed), named by
  step (e.g. `step02-panel-placement.jpg`) since this guide is a single file rather than per-step
  folders.
- **Video:** prefer hosting externally (e.g. an unlisted YouTube video) and embedding a link/
  thumbnail, rather than committing large video files to git — keeps the repo cloneable. If a clip
  is very short and small, committing it directly is fine; use judgment.
