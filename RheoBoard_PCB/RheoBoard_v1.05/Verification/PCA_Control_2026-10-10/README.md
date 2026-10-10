# PCA control and button header update — 2026-10-10

The saved v1.05 now uses U7 PCA9685 for both pumps and valves. Four buttons remain and connect to the ESP32 through J19. The user selected this Altium-style arrangement to reduce component count.

The independent CAD audit passes 36/36 checks. Direct ERC and saved/refilled DRC evidence are linked below; configured rule coverage and physical qualification limits remain explicit in the report. This is a scoped change audit, not a supplier placement approval or hardware test.

| Item | Current change |
| --- | --- |
| Removed | U4 TCA9534APWR, C22, C47, R10, R11, R13, R14 and obsolete TP1. |
| Actuator map | Pump PWM stays LED0/1; indicators LED2–4; valve1 moves to LED5/pin11 and valve2 to LED6/pin12. Valves use steady FULL ON/OFF. |
| Buttons | J19 pins1–5 are GND, SW1, SW2, SW3, SW4. All four 10 kΩ/100 nF networks remain; R16–R19 now use master-supplied HOST_3V3. |
| Layout | J19 and relocated R16–R19 occupy the former U4 area. Obsolete dedicated copper is removed and the new signals routed. Exact retained/changed geometry is in the independent comparison. |
| Population | 167 → 160 PCB components/features; 418 → 394 schematic pin nodes. Fitted parts 148 → 142; SMT placements 142 → 135; seven manual parts. |
| Preserved | Input protection, three converters, pressure sensor, Qwiic separation, pump/valve load stages, outline and mounting positions. |
| Manufacturing | 135 matched SMT BOM/CPL references in 42 groups, seven manual parts, twelve fresh fabrication files, five refreshed PDF drawings. |

Use the [assembly guide](../../ASSEMBLY_AND_BRINGUP.md) for the two-cable connection, GPIO assignments, auto-increment setting and valve command bytes. The button cable is not Qwiic. Each independent button bank needs four separate master GPIOs. Shared PCA global controls now affect pumps and valves together. The repository's existing DIY firmware has not been ported or tested for this revision.

## Evidence

- [Independent CAD review](final-independent-review.json), [proposed circuit crosscheck](proposed-control-review.json), [design basis and sources](control-design-basis.json).
- [BOM validation](bom-validation.json), [native manufacturing reconciliation](manufacturing-reconciliation.json), [3D link verification](model-path-validation.json).
- [Saved source hashes](source-hashes.json), [native schematic netlist](schematic.net), [native PCB IPC-2581](board.xml).

The manufacturer's new placement preview and first-board electrical, thermal and mechanical measurements remain open. The accepted sensor-reset and valve-voltage prototype risks are unchanged. No manufacturing order was submitted.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.
