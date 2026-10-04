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

- [Specifications](docs/specs.md)
- [Safety](docs/safety.md)
- [FAQ](docs/faq.md)
- [Changelog](CHANGELOG.md)
- [Contributing](CONTRIBUTING.md)

Pages still to be written:

- [Assembly packets](docs/assembly-packets.md)
- [Tools](docs/tools.md)
- [Mods](docs/mods.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Certification](docs/certification.md)
- [Logo](docs/logo.md)
- [Acknowledgements](docs/acknowledgements.md)
- [Kit vendors](docs/kit-vendors.md)
- [Community](docs/community.md)
- [Citation](docs/citation.md)
- [Assembly video](docs/assembly-video.md)

Linked statements already in the repository:

- [PCB has no firmware](RheoBoard_PCB/README.md)
- [Connector meshes without an editable source](BuildYourOwn/cad/connector/README.md)
- [Enclosure STEP without an editable source](BuildYourOwn/cad/encloser/README.md)
- [Rev C panel has not been test-fit](BuildYourOwn/laser-cut/README.md)

## Repository layout

```text
BuildYourOwn/
├── README.md              Step-by-step DIY build guide
├── hardware/              BOM, wiring, tube diagram, datasheets
├── laser-cut/             Panel vectors and cutting instructions
├── cad/                   Sensing-tube CAD; enclosure STEP exports
├── software/              ESP32 firmware and API (DIY build)
└── images/                Project and component photos

docs/                      Specifications, safety, FAQ, and pages still to be written
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
