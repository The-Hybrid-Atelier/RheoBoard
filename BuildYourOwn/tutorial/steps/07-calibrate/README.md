# Step 07: Calibrate

- **Time:** ~20–30 min for an initial tuning pass (ongoing — re-tune per fluid/fixture as needed)
- **Difficulty:** moderate

RheoBoard BYO uses **runtime BLE parameters** rather than a one-shot onboard calibration. Defaults
in `2P1VX` are tuned for the 2P1V bench rig; adjust per fluid/fixture via RheoData OSC.

## What you'll need for this step

- **Parts:** assembled 2P1V rig with firmware running; sample chamber/nozzle plumbed.
- **Design files:** none.
- **Tools:** RheoData (BLE), optional USB serial for bench commands

## Instructions

1. **Warm-up:** run a few idle REP cycles or wait for MPRLS to stabilize after power-on.
2. **Baseline:** default `rheo/rep/baseline/time` = 420 ms — increase if ambient drift is visible.
3. **Retract (pull):** tune `rheo/rep/pull/power` (default 54%) and `rheo/rep/pull/time`
   (default 315 ms) until retract is consistent without cavitation.
4. **Extrude (push):** tune `rheo/rep/push/power`, `push/time`, and ramp (`push/ramp/start`,
   `push/ramp/time`) — defaults 100%, 345 ms, 40% start, 300 ms ramp.
5. **Sampling:** set `rheo/sense/rate` (default 10 ms) to match RheoData capture needs.
6. **Verify:** trigger `rheo/rep` and confirm pressure trace shape is repeatable across 3 runs
   (`rheo/rep/triad`).

Full parameter list: [`../../../software/2P1VX/README.md`](../../../software/2P1VX/README.md).

## Media

Video of calibration procedure recommended (external YouTube hosting is fine).

## Tips / common mistakes

- If extrude is too aggressive, lower `push/power` or lengthen `push/ramp/time` before shortening
  `push/time`.
- Valve state must match pump: retract = VALVE2 OFF + PUMP1; extrude = VALVE2 ON + PUMP2 (see
  [`../../../wiring/pneumatic-plumbing.md`](../../../wiring/pneumatic-plumbing.md)).

## Check before moving on

- [ ] REP parameters documented for your fluid/fixture (screenshot or RheoData preset)
- [ ] Three consecutive REPs produce repeatable pressure traces
