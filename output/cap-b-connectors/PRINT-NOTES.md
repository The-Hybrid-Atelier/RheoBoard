# Two matching hose connectors for Cap B

Print `hose_connector_PLA.stl` twice. Each connector is one continuous solid.
Use the existing `../cap-b-one-piece/cap_B_one_piece_PLA.stl` cap unchanged.
The assembly STEP is for checking the arrangement; do not print the assembled
cap and connectors fused together.

Dimensions from the supplied drawing: 18.6 mm overall length, 6 mm male threaded
section, 1/4-28 UNF pitch (0.907143 mm), 3.2 mm hose tip OD, 4.5 mm stem base OD,
and 2.36 mm flow passage. The 4 mm callout is interpreted as the threaded-end
inlet bore, transitioning to the narrower passage inside the head. The hex head
is estimated at 9 mm across corners and 2.6 mm thick because it is not dimensioned.
The stem is tapered as shown, without extra barb ridges.

Male major diameter is 6.35 mm. The existing cap has a 6.65 mm thread root and
5.80 mm crest bore, including its previous provisional 0.15 mm radial PLA
allowance. These are matching prototype profiles, not certified thread gauges.
There is 6 mm nominal shank engagement; entrance chamfers reduce full-thread
engagement slightly. Each connector shoulder seats against the raised cap top.
The ports remain open into the bottle; no bottoming occurs in the 14 mm cap top.

CAD checks include both seated connectors, clearance between their heads, and
8 quarter-turn screw-motion positions over two turns. No cap change was needed.
The standalone connector STEP reimports as one valid solid; the STL has one
connected watertight surface with consistent edge winding.

For PLA on your Bambu printer, select the actual machine/nozzle and filament
preset. The STL is threaded-end down and hose tip up. Suggested trial settings
for a 0.4 mm nozzle: 0.12 mm layers, with build-plate support only beneath the
hex head if needed. Keep support out of the bore and threads. Inspect the slice:
the tip wall is only 0.42 mm thick, approximately one 0.4 mm extrusion width.
This is a delicate prototype at the reference dimensions; do not force the tube
or over-tighten the fitting. Print at 100% scale. No tested machine profile or
G-code is supplied.

Trial-fit one connector before printing the second. Printed fit, tube retention
and leak tightness are not established by collision checks. Thread clearance is
not a seal; the model does not include a gasket. The PLA parts do not inherit
the commercial cap's material or temperature ratings.

Hardware design files: see repository root LICENSE (CERN-OHL-W-2.0).
