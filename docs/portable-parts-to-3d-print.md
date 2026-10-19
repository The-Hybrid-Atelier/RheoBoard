# Portable parts to 3D print

Print files are on the [Portable parts to 3D print](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki/Portable-parts-to-3D-print) wiki page. The CAD is in [`RheoBoardPipette_CAD`](../BuildYourOwn/RheoBoardPipette/RheoBoardPipette_CAD/).

Trays are for three boards: the ESP32 Thing Plus, the SparkFun Qwiic Button, and the SparkFun Qwiic MicroPressure. [`ESP32_ThingPlus_tray.stl`](../BuildYourOwn/RheoBoardPipette/RheoBoardPipette_CAD/ESP32_ThingPlus_tray.stl) is the ESP32 tray. [`MicroPressure_tray.stl`](../BuildYourOwn/RheoBoardPipette/RheoBoardPipette_CAD/MicroPressure_tray.stl) is the tray for both the MicroPressure and the Qwiic Button, because the boards are the same size. The pictures are the same CAD views used for the RheoBoardOTS trays, in [`images`](../BuildYourOwn/RheoBoardOTS/images/).

[`inline_pipette_sensor_prototype.scad`](../BuildYourOwn/RheoBoardPipette/RheoBoardPipette_CAD/inline_pipette_sensor_prototype.scad) is the editable source of the inline pipette adapter. [`inline_pipette_sensor_prototype.stl`](../BuildYourOwn/RheoBoardPipette/RheoBoardPipette_CAD/inline_pipette_sensor_prototype.stl) is the mesh, about 19.4 × 11 × 30 mm. The source says this is a fitting prototype and is not calibrated.

Open gaps:

- TODO: copy count, orientation, supports, and material for these STL files.
