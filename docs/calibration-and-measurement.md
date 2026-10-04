# Calibration and measurement

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

From [Step 05 — Calibrate and use](../BuildYourOwn/README.md#step-05-calibrate-and-use) in the build guide:

RheoBoard uses runtime BLE parameters rather than a one-shot calibration. Parameter definitions and defaults are maintained in the [firmware reference](../BuildYourOwn/software/README.md).

1. Position the chamber/nozzle repeatably at the sample.
2. Let the MPRLS stabilize, then run several REPs with no sample.
3. Tune baseline, retract, extrude, ramp, and sampling parameters until a triad produces three repeatable traces.
4. Measure the sample and confirm its pressure trace appears in RheoData.
5. Use the [firmware reference](../BuildYourOwn/software/README.md) for BLE, button, serial, and debug controls.
6. Save the verified settings and use the same fixture position for comparable measurements.
