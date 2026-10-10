# RheoBoard

Maintained by [Charlie Vuong](https://charlie-vuong.com) (The Hybrid Atelier at UT Arlington).

<img src="BuildYourOwn/images/teaser.jpg" alt="RheoBoard assembled bench prototype" width="480">

RheoBoard is open hardware for pneumatic retraction-extrusion measurements. RheoBoardOTS - DIY uses
two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs
a REP (retraction-extrusion pulse) and streams the pressure trace.

## Start here

Build instructions are on the [RheoBoard wiki](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki).
RheoBoardOTS - DIY is the flat 3D-printed panel: two air pumps, one valve, an ESP32 with BLE, and a
Qwiic MicroPressure sensor. On command, it runs a REP (retraction-extrusion pulse). RheoBoard_v1 - PCB
is the separate KiCad board. Everyday power is a 12 V plug. RheoBoardPipette - Portable uses the
SparkFun ESP32 Thing Plus and the PRT-26059 battery (nominal 3.7 V, JST, schematic V_BATT, 4.2 V
maximum). There is no separate RheoBoardPipette - Portable enclosure in the repository. The firmware
in [`code/firmware/`](code/firmware/) applies to all three.

## License

All license notices are consolidated in [`LICENSE`](LICENSE):

- Hardware designs: CERN-OHL-W-2.0
- Firmware: MIT
- Documentation: [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)

Third-party parts, photos, datasheets, and libraries remain under their original terms; sources
are listed in [`BuildYourOwn/images/README.md`](BuildYourOwn/images/README.md) and
[`BuildYourOwn/hardware/references/README.md`](BuildYourOwn/hardware/references/README.md).

<a href="https://certification.oshwa.org/us002865.html"><img src="docs/img/oshwa-certification-mark-US002865-wide.svg" alt="OSHW certification mark US002865" height="48"></a>

RheoBoard version 1 is certified open source hardware by the Open Source Hardware Association,
UID [US002865](https://certification.oshwa.org/us002865.html) (certified 2026-10-08); its docs are
at the [`v1` tag](https://github.com/The-Hybrid-Atelier/RheoBoard/tree/v1). Version 2, the
3D-printed build on `main`, is not certified yet. See [`docs/certification.md`](docs/certification.md). Machine-readable project metadata and the current
version are in [`okh-RheoBoard.yml`](okh-RheoBoard.yml).

If you build or distribute a derived unit, do not imply that it is manufactured, sold, warranted,
or endorsed by the original designer.
