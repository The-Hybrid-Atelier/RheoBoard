# Step 05: Mount and set up

- **Time:** ~15–20 min
- **Difficulty:** easy

Mechanical mounting, sensor geometry, and anything that must be fixed in place before power-on
measurements mean anything (stable stand, level surface, orientation). By the end of this step
the rig is physically ready for power — step 06 turns it on.

## What you'll need for this step

- **Parts:** the fully assembled panel from steps 02–03 (platform + zip-tied components +
  electrical wiring + pneumatic tubing).
- **Design files:** [`../../../laser-cut/`](../../../laser-cut/) — panel dimensions and the
  chamber/foot positions. [`../../../hardware/wiring/pneumatic-plumbing.md`](../../../hardware/wiring/pneumatic-plumbing.md)
  for what the shared line/chamber connects to.
- **Tools:** none beyond what's already in hand (no new fasteners — the panel's 4 corner feet
  are its only "mount").

## Instructions

1. **Place the panel** on a flat, level, stable surface — a workbench, not something that
   flexes or vibrates (a wobbly folding table will show up as noise in the pressure trace).
   The 4 corner feet are the only leveling/isolation the panel has; don't stack anything on top
   of it.
2. **Check tubing routing:** confirm no line from PUMP1/PUMP2 → VALVE2 → the shared T-connector →
   chamber is kinked, pinched under a zip tie, or under tension from panel placement. A kinked
   line reads as a phantom pressure spike or a dead channel that isn't actually a wiring fault.
3. **Position the chamber/nozzle** (the Ø10 bulkhead fitting — "the line we sense" per
   [`pneumatic-plumbing.md`](../../../hardware/wiring/pneumatic-plumbing.md)) at whatever sample or test
   surface it needs to interface with for your measurement.
   **This is the one part of this step we can't fully spec yet** — the exact sample/fixture
   geometry (standoff distance, alignment, contact angle) is part of the RheoMap product spec,
   which is still TBD in [`../../../product-specs.md`](../../../product-specs.md). Until that
   spec exists, use judgment: the nozzle should reach the sample without the tubing pulling on
   the panel, and its position should be repeatable between measurements (mark it if you'll be
   moving it between runs). **Update this step once RheoMap's fixture spec lands.**
4. **Cable slack:** route the micro-USB and 12 V adapter cables (connected in the next step) with
   enough slack that plugging in doesn't tug on the panel or its feet.
5. **Environment:** avoid direct drafts or a heat source pointed at the chamber/sensor — the
   REP's baseline phase (ambient pressure sampling, default 420 ms — see
   [`../../../software/rheometer-firmware/README.md`](../../../software/rheometer-firmware/README.md)) is short but not
   instant, and a sudden draft during that window will bias the whole trace.

## Media

Photo of the fully mounted assembly — panel placement, tubing routing, and chamber position —
before wiring power/data in step 06.

## Tips / common mistakes

- A kinked or pinched tube after mounting is easy to miss and looks like a sensor problem, not a
  mechanical one — physically trace every line from pump to chamber after placing the panel.
- Don't over-tighten zip ties against the panel edge when routing tubing near step 02's slots —
  crushing the silicone ID changes flow resistance.

## Check before moving on

- [ ] Panel sits flat and stable — no wobble that would affect measurements
- [ ] Every pneumatic line traced end-to-end: no kinks, no pinches, no tension
- [ ] Chamber/nozzle positioned at the sample interface (repeatable position noted, if applicable)
- [ ] Power/data cable routing won't stress the panel once connected in step 06
