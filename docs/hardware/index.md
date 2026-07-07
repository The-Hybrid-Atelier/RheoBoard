# Hardware

There are two hardware tracks for RheoMap/SlipTopo:

- **[`build-your-own/`](build-your-own/)** — off-the-shelf modules/dev boards, breadboard or
  perfboard, wiring diagrams instead of CAD. Source files live in `../../BuildYourOwn/`.
  **This is the current focus.**
- **[`pcb/`](pcb/)** — the custom Altium PCB. Source files live in `../../RheoBoard_V8_Final/`.
  Not actively worked on right now, but kept in sync opportunistically.

Each subfolder holds process docs (BOM notes, revision history, verification checklist) for
that track. The actual build artifacts (schematics, wiring diagrams, parts lists) live with
the hardware itself, not under `docs/` — see each subfolder for where to look.
