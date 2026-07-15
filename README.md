# RheoBoard

### A low-cost pneumatic rheometer for RheoMap, RheoData, and SlipAtlas

Maintained by Charlie Vuong (The Hybrid Atelier).

<img src="BuildYourOwn/images/teaser.jpg" alt="RheoBoard assembled bench prototype" width="480">

RheoBoard is open hardware for pneumatic retraction-extrusion measurements. The DIY version uses
two air pumps, one valve, an ESP32 with BLE, and a Qwiic MicroPressure sensor. On command, it runs
a REP (retraction-extrusion pulse) and streams the pressure trace.

## Build the DIY version

Start with the **[step-by-step build guide](BuildYourOwn/README.md)**.

Supporting instructions:

- [Hardware and bill of materials](BuildYourOwn/hardware/README.md)
- [Laser-cut panel](BuildYourOwn/laser-cut/)
- [Electronic wiring](BuildYourOwn/hardware/electronic-wiring/)
- [Tube wiring](BuildYourOwn/hardware/tube-wiring/)
- [Firmware and OSC API](BuildYourOwn/software/)
- [Build verification checklist](BuildYourOwn/VERIFICATION.md)

> **Current limitation:** the laser-cut vector file is a draft and has not been physically cut or
> test-fit against real parts. The complete build also still needs human verification.

## Hardware tracks

1. **DIY** ([`BuildYourOwn/`](BuildYourOwn/)) — the current focus, built from off-the-shelf
   modules on a laser-cut acrylic panel.
2. **Custom PCB** ([`RheoBoard-PCB_V9/`](RheoBoard-PCB_V9/)) — the Altium-designed board.

## Repository layout

```text
BuildYourOwn/
├── README.md              Step-by-step DIY build guide
├── hardware/
│   ├── README.md          Bill of materials
│   ├── electronic-wiring/ Electronic schematic and source
│   ├── tube-wiring/       Pneumatic tube diagram and instructions
│   └── references/        Datasheets
├── laser-cut/             Panel vectors and cutting instructions
├── software/              ESP32 firmware and API
├── images/                Project and component photos
└── VERIFICATION.md        Human build-verification checklist

RheoBoard-PCB_V9/          Custom PCB source files
okh-RheoBoard.yml          Open Know-How metadata
LICENSE                    Project license notices and texts
```

The latest work and remaining tasks are recorded in
[`BuildYourOwn/PROGRESS.md`](BuildYourOwn/PROGRESS.md).

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
