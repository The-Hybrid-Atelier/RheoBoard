# Core beliefs

Opinionated, mechanical-where-possible rules for keeping this project legible to future
sessions (agent or human). Update this file when a recurring mistake or preference emerges —
that's cheaper than repeating the correction every session.

- **Prose is the diff for hardware.** Binary CAD files don't show intent in `git diff`. Any
  schematic/layout/library change must be described in `docs/PROGRESS.md` and/or the relevant
  exec-plan in enough detail that someone could reconstruct *why* without opening Altium.
- **Every part is traceable.** A component only belongs in the BOM if its datasheet is linked
  (in `docs/references/index.md` or inline in the exec-plan) and its footprint/symbol has been
  checked against that datasheet. No guessing pinouts or package dimensions.
- **Prefer boring, available parts.** Mainstream, well-documented, multi-sourced components
  beat exotic ones unless there's a documented reason (performance, cost, form factor) — write
  that reason down when it applies.
- **Don't silently resolve ambiguity.** If a spec or requirement is unclear, record the open
  question in the relevant exec-plan's decision log rather than picking an interpretation and
  moving on quietly.
- **Small, meaningful commits.** Even without PRs, each commit on `main` should represent one
  coherent change with a message that explains *why*, not just *what*.
- **Docs are updated in the same sitting as the change**, not deferred — `docs/PROGRESS.md`
  and the relevant exec-plan get updated as part of finishing the work, not as separate cleanup.
