# Build verification checklist (Build Your Own)

Run through before calling a BYO build revision "done." Copy the relevant items into the
exec-plan for the change and check them off there.

## Build

- [ ] Wiring matches the diagram in `BuildYourOwn/wiring/` (no ad hoc deviations left
      undocumented)
- [ ] Continuity/short check on all new connections before first power-up
- [ ] Powers up without excessive current draw or heat on any component
- [ ] Every signal/sensor reads plausible values (not stuck, not noise)

## Parts & docs

- [ ] Every part in `BuildYourOwn/BOM.md` has a product/datasheet link
      (`docs/references/index.md`)
- [ ] No unresolved/placeholder parts
- [ ] `BuildYourOwn/ASSEMBLY.md` matches what was actually built (steps, part orientation,
      photos if applicable) — someone should be able to follow it cold and get the same result

## Documentation

- [ ] `docs/PROGRESS.md` updated with what changed and why
- [ ] `docs/hardware/build-your-own/revision-history.md` updated
- [ ] Relevant exec-plan moved to `docs/exec-plans/completed/` (if finished)
