# J18 removal verification

J18 was removed from v1.05 at the maintainer's request on 2026-10-09. J3 remains the four-pin JST EH debug header: pin 1 GND, pin 2 onboard +3V3, pin 3 SDA, pin 4 SCL. J9 remains the Qwiic master connector; J1/J12/J17 remain powered Qwiic sensor connectors.

The schematic symbol, four wire stubs, three associated net labels and J18-specific annotation text were removed or updated through Konnect. The PCB footprint, its three holes, ten dedicated back-layer trace segments and three service silkscreen labels were removed through live KiCad IPC. Both five-segment branches were traced from J18 to J3; no interior shared trace junction, pad or via was found. Copper was refilled and the board saved before checking and exporting.

| Check | Result |
| --- | --- |
| KiCad ERC | 0 errors, 0 warnings |
| Saved/refilled KiCad DRC | 0 errors, 0 warnings, 0 unconnected items, 0 schematic-parity findings |
| Schematic structure | 0 orphan items, 0 shorted nets |
| Retained circuit | All 413 remaining pin nodes keep their nets and reconcile with PCB pads; 89 schematic nets remain |
| Retained layout | All 165 remaining components keep values, footprints, positions, rotations, sides and pad geometry; outline and 301 vias unchanged |
| Copper routing | Exactly ten dedicated J18 branch segments removed; 611 remaining copper line/arc primitives |
| BOM | 146 fitted parts: 140 SMT, six manual; J18 housing/contacts removed from accessories |
| Assembly files | 140 SMT BOM/CPL references reconcile; coordinates and rotations unchanged |
| Fabrication | Fresh nine Gerbers, PTH/NPTH drills and job file; 12-file archive matches exports; exactly J18's three 0.95 mm plated holes removed |
| Preservation | All original v1 files unchanged; v1.05 project rule settings and other circuit values unchanged |

The native unfiltered manufacturing package still reports its mechanical/manual-part population mismatch. The delivered SMT BOM/CPL pair is separately reconciled to the 140 purchased SMT parts. This does not change the raw package verdict or validate supplier rotation conventions. JLCPCB placement preview and physical tests remain open. Previously accepted U5 startup/reset and valve-margin risks are unchanged.

- [Validation and saved source hashes](validation.json)
- [ERC](after-erc-decoded.json), [DRC](after-drc-decoded.json), [orphan items](after-orphans-decoded.json), [shorted nets](after-shorts-decoded.json)
- [Exclusive branch proof](branch-proof.json), [updated v1 comparison](v1-comparison.json)
- [BOM validation](bom-validation.json), [native manufacturing result](after-manufacturing-decoded.json)
- [Current schematic](../../Manufacturing/Drawings/RheoBoard_v1.05_schematic.pdf), [front copper](../../Manufacturing/Drawings/RheoBoard_v1.05_front_copper.pdf), [back copper](../../Manufacturing/Drawings/RheoBoard_v1.05_back_copper.pdf), [silkscreen](silkscreen.pdf)

Native before/after netlists and IPC-2581 exports are retained here. Earlier Qwiic, Thorough and Comparison evidence directories describe the pre-removal checkpoints; their source hashes and 166-component/147-fitted counts are historical. The current release manifest binds the updated sources and manufacturing files.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
