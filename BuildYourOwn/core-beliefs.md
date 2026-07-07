# Core beliefs

Opinionated, mechanical-where-possible rules for keeping this project legible to future
sessions (agent or human). Update this file when a recurring mistake or preference emerges —
that's cheaper than repeating the correction every session.

- **Prose is the diff for hardware.** Binary CAD files (PCB track) don't show intent in
  `git diff`. Any schematic/layout/library change must be described in `PROGRESS.md` and/or the
  relevant exec-plan in enough detail that someone could reconstruct *why* without opening Altium.
- **Every part is traceable.** A component only belongs in the BOM if its datasheet is linked
  (in `references.md` or inline in the exec-plan) and its footprint/symbol has been checked
  against that datasheet. No guessing pinouts or package dimensions.
- **Prefer boring, available parts.** Mainstream, well-documented, multi-sourced components
  beat exotic ones unless there's a documented reason (performance, cost, form factor) — write
  that reason down when it applies.
- **Don't silently resolve ambiguity.** If a spec or requirement is unclear, record the open
  question in the relevant exec-plan's decision log rather than picking an interpretation and
  moving on quietly.
- **Small, meaningful commits.** Even without PRs, each commit on `main` should represent one
  coherent change with a message that explains *why*, not just *what*.
- **Docs are updated in the same sitting as the change**, not deferred — `PROGRESS.md` and the
  relevant exec-plan get updated as part of finishing the work, not as separate cleanup.
- **One deliverable at a time.** Don't try to write the whole BOM, the whole tutorial, or a
  whole board revision in one pass. Pick the single next thing (one BOM section, one tutorial
  step, one sprint contract), finish and verify it, then move on — this is the main way large
  hardware/doc efforts stay coherent instead of sprawling into a half-finished mess.
- **Checklists are a floor, not a self-congratulation exercise.** Don't check off, delete, or
  quietly water down a verification item to make something look done. If an item is genuinely
  obsolete, say so explicitly in the exec-plan's decision log — don't just remove it. If an item
  can't be verified without a human physically doing something, it stays unchecked until a
  human reports back (see `AGENTS.md` → "What the agent can and can't verify").
