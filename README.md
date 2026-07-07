# RheoBoard

Open hardware powering **RheoMap** and **SlipTopo**. There are two ways to build the hardware:

1. **Build Your Own** ([`BuildYourOwn/`](BuildYourOwn/)) — off-the-shelf modules/dev boards,
   breadboard/perfboard, wiring diagrams + BOM + assembly guide. **Current focus.**
2. **Custom PCB** ([`RheoBoard_V8_Final/`](RheoBoard_V8_Final/)) — an Altium-designed board.
   Existing, not actively worked on right now.

> Full project background, goals, and specs are being written up in
> `BuildYourOwn/product-specs.md` — this README will grow as that lands.

## Repository layout

- `BuildYourOwn/` — the DIY build, and essentially the whole project harness: parts list,
  design files, wiring diagrams, tutorial, plus the docs (progress log, exec-plans, core
  beliefs, references, product specs).
- `RheoBoard_V8_Final/` — Altium Designer project (schematic, PCB layout, symbol/footprint
  libraries, BOM, manufacturing outputs). Not actively worked on.
- `AGENTS.md` — map for AI coding agents (and future-me) working in this repo.

## Status

Actively developed, solo maintainer. See [`BuildYourOwn/PROGRESS.md`](BuildYourOwn/PROGRESS.md)
for the latest state.

## License

TBD.
