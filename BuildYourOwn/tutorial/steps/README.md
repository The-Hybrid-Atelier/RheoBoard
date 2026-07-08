# Steps

One folder per tutorial step, numbered in build order:

```
steps/
├── 01-kit-contents-and-tools/
│   ├── README.md
│   └── media/
├── 02-assemble-platform/
│   ├── README.md
│   └── media/
└── ...
```

To add a step:

1. Copy `../_step-template/` to `steps/NN-short-slug/` (zero-padded two-digit prefix, lowercase
   kebab-case slug describing the step).
2. Fill in its `README.md`.
3. Drop any images/videos for that step into its `media/` folder and reference them from the
   `README.md`.
4. Add it to the numbered list in `../README.md`.

Keep step numbers stable once referenced elsewhere (e.g. from `BOM.md` notes) — if a step needs
to be inserted later, it's fine to leave gaps (`05`, `06`, `07a`) rather than renumbering everything.

_All eight step folders have written instructions now — see `../README.md` for per-step status
and any remaining blockers. No step has photos or video yet regardless of text status._
