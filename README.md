# RheoBoard

### A low-cost pneumatic rheometer for RheoMap, RheoData, and SlipAtlas

Maintained by Charlie Vuong (The Hybrid Atelier at UT Arlington).

<img src="BuildYourOwn/images/teaser.jpg" alt="RheoBoard assembled bench prototype" width="480">

RheoBoard is open hardware for pneumatic retraction-extrusion measurements. The DIY version uses
two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs
a REP (retraction-extrusion pulse) and streams the pressure trace.

## Start here

1. [Step-by-step DIY build guide](BuildYourOwn/README.md)
2. [Custom PCB](RheoBoard_PCB/README.md)

## DIY version

Supporting instructions:

- [Hardware and bill of materials](BuildYourOwn/hardware/README.md)
- [Laser-cut panel](BuildYourOwn/laser-cut/)
- [Electronic wiring](BuildYourOwn/hardware/electronic-wiring/)
- [Tube wiring](BuildYourOwn/hardware/tube-wiring/)
- [Firmware and OSC API](BuildYourOwn/software/)
- [Sensing-tube CAD](BuildYourOwn/cad/connector/)

> **Build status:** The current panel files are Rev C
> ([laser-cut panel](BuildYourOwn/laser-cut/README.md)). Physical test-fit is still required.
> OSHWA certification has not yet been submitted.

## Custom PCB

[`RheoBoard_PCB/`](RheoBoard_PCB/) is a separate KiCad 10 board: schematic, layout, libraries,
Gerbers, assembly BOM, and pick-and-place. The firmware in `BuildYourOwn/software/` drives the
DIY module build. Read [`RheoBoard_PCB/README.md`](RheoBoard_PCB/README.md) before ordering boards.

## Documentation

### Getting Started

- [Specifications](docs/specs.md)
- [Panel layout](docs/panel-layout.md)
- [Safety](docs/safety.md)

### Background

- [Project goals](docs/project-goals.md)
- [Part numbers](docs/part-numbers.md)
- [Versions](CHANGELOG.md)

### Building

- [Build options](docs/build-options.md)
- [Parts to buy](docs/parts-to-buy.md)
- [Parts to 3D print](docs/parts-to-3d-print.md)
- [Parts to laser-cut](docs/parts-to-laser-cut.md)
- [Parts to machine](docs/parts-to-machine.md)
- [Assembly tools](docs/assembly-tools.md)
- [Assembly instructions](BuildYourOwn/README.md)
- [Wiring](docs/wiring.md)
- [Firmware](docs/firmware.md)

### Using

- [Connecting](docs/connecting.md)
- [Power-on checks](docs/power-on-checks.md)
- [Calibration and measurement](docs/calibration-and-measurement.md)

### Maintenance

- [Maintenance](docs/maintenance.md)

### Extending

- [Mods](docs/mods.md)
- [Guidelines](docs/guidelines.md)

### Troubleshooting

- [Troubleshooting](docs/troubleshooting.md)

### Contributing

- [Contributing](CONTRIBUTING.md)

### Acknowledgements

- [Acknowledgements](docs/acknowledgements.md)

### FAQ

- [FAQ](docs/faq.md)

### Pages still to be written

- [Assembly packets](docs/assembly-packets.md)
- [Tools](docs/tools.md)
- [Certification](docs/certification.md)
- [Logo](docs/logo.md)
- [Kit vendors](docs/kit-vendors.md)
- [Community](docs/community.md)
- [Citation](docs/citation.md)
- [Assembly video](docs/assembly-video.md)

## Repository layout

```text
BuildYourOwn/
├── README.md              Step-by-step DIY build guide
├── hardware/              BOM, wiring, tube diagram, datasheets
├── laser-cut/             Panel vectors and cutting instructions
├── cad/                   Sensing-tube CAD; enclosure STEP exports
├── software/              ESP32 firmware and API (DIY build)
└── images/                Project and component photos

docs/                      Documentation pages listed under Documentation above
RheoBoard_PCB/             KiCad board, fabrication files, and board notes
CHANGELOG.md               Public version notes
CONTRIBUTING.md            How to contribute
okh-RheoBoard.yml          Open Know-How metadata
LICENSE                    Project license notices and texts
```

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
