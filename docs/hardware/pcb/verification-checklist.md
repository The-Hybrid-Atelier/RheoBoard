# Hardware verification checklist

Run through before calling a board revision "done." Copy the relevant items into the exec-plan
for the change and check them off there.

> **Every item below requires a human to physically do or observe something (or run Altium's
> DRC/ERC themselves).** An agent must never check these off itself — see `AGENTS.md` → "What
> the agent can and can't verify." Use the sign-off block at the bottom to record who checked.

## Design

- [ ] DRC (Design Rule Check) clean in Altium
- [ ] ERC (Electrical Rule Check) clean in Altium
- [ ] All footprints verified against the manufacturer's datasheet/package drawing
- [ ] All schematic symbols verified against datasheet pinout
- [ ] Net names/labels reviewed for typos and consistency

## BOM

- [ ] Every part has a datasheet reference (`docs/references/index.md`)
- [ ] No unresolved/placeholder parts
- [ ] Availability checked with at least one distributor

## Documentation

- [ ] `docs/PROGRESS.md` updated with what changed and why
- [ ] `docs/hardware/pcb/revision-history.md` updated
- [ ] Relevant exec-plan moved to `docs/exec-plans/completed/` (if finished)

## Sign-off

- Verified by:
- Date:
- Notes (anything that failed, was skipped, or needs a follow-up in `tech-debt-tracker.md`):
