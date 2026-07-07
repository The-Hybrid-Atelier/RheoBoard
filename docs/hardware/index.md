# Hardware

There are two hardware tracks for RheoMap/SlipTopo:

- **Build Your Own** — off-the-shelf modules/dev boards, breadboard/perfboard, wiring diagrams
  instead of CAD. Everything (BOM, design files, verification checklist, tutorial) lives
  directly in [`../../BuildYourOwn/`](../../BuildYourOwn/) — no subfolder here. **This is the
  current focus.**
- **[`pcb/`](pcb/)** — the custom Altium PCB. Source files live in
  [`../../RheoBoard_V8_Final/`](../../RheoBoard_V8_Final/). Not actively worked on right now.

## Why PCB gets a `docs/hardware/pcb/` subfolder and BYO doesn't

`RheoBoard_V8_Final/` is Altium binaries — no meaningful git diff, so BOM notes and a revision
history have to live somewhere text-based to be legible at all. `BuildYourOwn/` is already
plain markdown, which diffs and greps natively, so a parallel `docs/hardware/build-your-own/`
tree would just be indirection to files that could hold that content themselves. Don't recreate
it — if BYO ever needs a process doc, put it directly in `BuildYourOwn/`.
