# Cap B — one-piece PLA prototype

One continuous solid, with the raised top permanently integrated into the ribbed
shell. Two female ports for 1/4-28 fittings; no hose couplers, separate insert,
assembly clearance gaps, or moving parts. STL units are millimeters.

## Files

- cap_B_one_piece_PLA.stl — one connected watertight mesh, oriented for printing.
- cap_B_one_piece.step — same one-solid geometry in the normal upright CAD view.
- preview.png — top and underside views of the same model.
- geometry-check.json — exported-file checks.
- build_cap.py — editable source.

## Geometry and fit

Outside diameter 53.8 mm; skirt height 25 mm; total height 30 mm; raised pad
36.6 mm diameter; port spacing 16 mm. The top and shell are fully joined, with
no trapped annular gap from the earlier separate-insert design.

The two ports have 0.907143 mm pitch (28 TPI) and a 60-degree profile.
A provisional 0.15 mm radial enlargement is included for PLA trial fitting:
5.80 mm crest bore and 6.65 mm thread root diameter. This is an intentionally
oversized prototype thread for nominal 1/4-28 male fittings, not a certified
UNF tolerance class or a measured Bambu calibration. Do not add another hole
compensation automatically. Thread fit depends on nozzle, filament and slicing.

The bottle thread retains the reference reconstruction's approximate rounded
GL45 profile: 4 mm pitch, 45.55 mm root diameter and 42.85 mm crest diameter.
Bottle fit has not been physically checked. The shell/raised-pad proportions
remain estimates from the reference; this is not manufacturer CAD.

## Starting point for Bambu Studio

Use the PLA profile for your actual filament and the preset for your actual
printer/nozzle. The STL is already raised-face down, with the bottle opening up.
This avoids bridging the bottle cavity. Support is needed underneath the outer
shoulder, which begins 5 mm above the bed; use build-plate-only supports and
keep support out of both small ports and the bottle threads. Inspect the layer
preview before printing. Remove the shoulder support after printing.

For a common 0.4 mm nozzle, a proposed starting point is 0.12 mm layers and
4 walls. These are untested starting settings, not a supplied or validated
machine profile. Print at 100% scale; changing scale also changes thread pitch.
No machine-specific G-code or temperature/speed preset is supplied.

Trial-fit the bottle and fittings gently. Fit and sealing are separate checks:
printed threads alone do not establish an airtight seal. This PLA prototype
does not inherit the commercial polypropylene/PTFE cap's 140 C rating shown
in the reference image.

Hardware design files: see the repository root LICENSE (CERN-OHL-W-2.0).
