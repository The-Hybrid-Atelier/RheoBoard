# RheoBoardOTS

This folder is the ESP32 sketch for the RheoBoardOTS DIY controller. The entry point is `RheoBoardOTS.ino`. Arduino requires the folder name to match that file. Install steps are on the [Firmware](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Firmware) wiki page. The channel map, OSC addresses, serial commands, and REP timing are in `../README.md`.

`HardwareProfile.h` also offers an experimental profile for the older v1 PCB. DIY remains the default. See the [profile instructions](../README.md#older-v1-pcb-profile-experimental) before building for that board; v1.05 is not supported by this profile.

`PneumaticSystem` provides hardware operations, `RheoSystem` runs REP and bubble sequences, and `RoutinePlayer` runs uploaded programs. `V1Hardware` handles the older PCB's expanders and pressure sensor.

Firmware is MIT-licensed. The license notice and full text are in [`LICENSE`](../../../LICENSE).
