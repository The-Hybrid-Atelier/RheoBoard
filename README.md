# RheoBoard

Maintained by [Charlie Vuong](https://charlie-vuong.com) (The Hybrid Atelier at UT Arlington).

<img src="BuildYourOwn/images/teaser.jpg" alt="RheoBoard assembled bench prototype" width="480">

RheoBoard is open hardware for pneumatic retraction-extrusion measurements. The DIY version uses
two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs
a REP (retraction-extrusion pulse) and streams the pressure trace.

## Start here

1. [Step-by-step DIY build guide](BuildYourOwn/README.md)
2. [Custom PCB](RheoBoard_PCB/README.md)

The firmware in `BuildYourOwn/software/` drives the DIY module build.

The current panel files are Rev C. Physical test-fit is still required.

The detailed guide is the [RheoBoard wiki](https://github.com/The-Hybrid-Atelier/RheoBoard/wiki).

## License

All license notices are consolidated in [`LICENSE`](LICENSE):

- Hardware designs: CERN-OHL-W-2.0
- Firmware: MIT
- Documentation: [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/)

Third-party parts, photos, datasheets, and libraries remain under their original terms; sources
are listed in [`BuildYourOwn/images/README.md`](BuildYourOwn/images/README.md) and
[`BuildYourOwn/hardware/references/README.md`](BuildYourOwn/hardware/references/README.md).

RheoBoard is not yet OSHWA-certified. Machine-readable project metadata and the current version
are in [`okh-RheoBoard.yml`](okh-RheoBoard.yml).

If you build or distribute a derived unit, do not imply that it is manufactured, sold, warranted,
or endorsed by the original designer.
