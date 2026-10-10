# Connecting

License: CC BY-SA 4.0 — see [`../LICENSE`](../LICENSE).

From the [firmware reference](../code/firmware/README.md):

- BLE: the device advertises as `2P1V_Adafruit`; control via RheoData bridge / OSC (`rheo/rep`, etc.).
- Serial baud is 115200.

RheoData is the app used with that firmware for RheoBoardOTS - DIY, RheoBoard_v1 - PCB, and RheoBoardPipette - Portable. From [Step 04 of the build guide](../BuildYourOwn/README.md#step-04-power-on-test): connect RheoData to `2P1V_Adafruit` after the power-on checks. TODO: the download, pairing steps, the connected indicator, and how to start and name a REP ([`code/software/README.md`](../code/software/README.md)).
