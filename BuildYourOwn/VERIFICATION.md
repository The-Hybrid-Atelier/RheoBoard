# Build verification checklist

Run through before calling a build revision "done." Copy the relevant items into the exec-plan
for the change and check them off there.

## Build

- [ ] Wiring matches the diagram in [`wiring/`](wiring/) (no ad hoc deviations left undocumented)
- [ ] Continuity/short check on all new connections before first power-up
- [ ] Powers up without excessive current draw or heat on any component
- [ ] Every signal/sensor reads plausible values (not stuck, not noise)

## Parts & docs

- [ ] Every part in [`BOM.md`](BOM.md) has a product/datasheet link (see `docs/references/index.md`)
- [ ] No unresolved/placeholder parts
- [ ] [`tutorial/`](tutorial/) matches what was actually built (steps, part orientation, media)
      — someone should be able to follow it cold and get the same result

## Documentation

- [ ] `docs/PROGRESS.md` updated with what changed and why (this is the revision history for
      this track — see note in `BOM.md`, no separate log needed since these files diff natively)
- [ ] Relevant exec-plan moved to `docs/exec-plans/completed/` (if finished)
