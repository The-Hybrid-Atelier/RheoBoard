# RheoBoard v1.05 assembly and bringup

This revision retains two independently controlled, fixed-direction pumps and two valves. The pumps are for intermittent use. Losing communication intentionally leaves the last commanded outputs running; removing board power or sending an OFF command stops them. The board has no onboard microcontroller.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Power and connectors

Use a regulated, center-positive 12 V DC supply with the plug dimensions required by the PJ-082BH jack: 5.5 mm outside and 2.5 mm inside. For initial testing, use a bench supply with adjustable current limiting. A 12 V, 2 A adapter is a starting selection for one board with the specified loads, subject to measured startup and simultaneous-load current. A larger adapter does not increase the board's allowable current. The input fuse is 2 A and the added eFuse has an approximately 2 A current limit.

The board makes its own 3.3 V logic supply from the 12 V input. It also makes approximately 4.52 V for pumps and 6.0 V for valves. J9 is the dedicated Qwiic master connection: its 3.3 V pin receives power from the USB/battery-powered ESP32 Thing Plus and supplies the host side of U13 and the four button pullups. U13 separates the two I²C signal domains without joining the regulated 3.3 V rails. All grounds remain common; this is not galvanic isolation.

| Connection | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
| --- | --- | --- | --- | --- |
| J9 QWIIC MASTER, JST SH | GND | Host 3.3 V input | HOST_SDA | HOST_SCL |
| J1, J12, J17 QWIIC SENSOR, JST SH | GND | Local 3.3 V output | SDA | SCL |
| J3, JST EH 4-pin peripheral | GND | Local 3.3 V output | SDA | SCL |

All four JST SH connectors retain the standard Qwiic order: pin 1 GND, pin 2 3.3 V, pin 3 SDA, pin 4 SCL. Read the PCB pin-1 mark and connector drawing; a cable viewed from its mating end reverses the apparent left-to-right order. Standard Qwiic cable colors are black, red, blue and yellow in that order. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

Connect the ESP32 to **J9**, using an ordinary four-wire Qwiic cable. J1, J12 and J17 power local sensor modules from RheoBoard and share its local bus. Do not plug another independently powered controller or RheoBoard into these sensor ports. J3 provides local debug access with onboard 3.3 V and bypasses U13. The redundant J18 header has been removed.

The checked controller is SparkFun's ESP32 Thing Plus Micro-B **WRL-15663**: Qwiic SDA is GPIO23 and SCL is GPIO22, and the Qwiic power pin connects to its onboard 3.3 V regulator. Other Thing Plus variants require their own power/pin check. U13 is TCA9517ADGKR, with the RheoBoard bus on its A side and the ESP32 on its B side. Both sides have their own supply bypass and 4.7 kΩ pullups. EN follows host power. TI specifies high-impedance bus outputs when either supply is off; verify power sequencing and leakage on the assembled system. [SparkFun schematic](https://cdn.sparkfun.com/assets/6/d/c/6/c/ESP32_Thing_Plus_Schematic.pdf), [TI TCA9517A](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

## Button GPIO cable

J19 is a **five-pin, 2.54 mm button header**, separate from Qwiic and the retained J3 debug port. Fit a straight male header and connect these five leads to the ESP32. Pin 1 is the square pad; use the pin numbers and board labels rather than assuming the cable viewing direction.

| J19 pin | Button signal | Switch | ESP32 Thing Plus Micro-B connection |
| --- | --- | --- | --- |
| 1 | GND | Common return | GND |
| 2 | VALVE1_IN | SW1 | GPIO32, board label 32 |
| 3 | VALVE2_IN | SW2 | GPIO33, board label 33 |
| 4 | PUMP1_IN | SW3 | GPIO25, board label A1 |
| 5 | PUMP2_IN | SW4 | GPIO26, board label A0 |

These are recommended firmware assignments for the stated WRL-15663 controller. They are available non-strapping GPIOs and do not use Qwiic's GPIO23/22. Configure them as digital **inputs**, never driven outputs. Each button is active LOW with an existing 10 kΩ pullup and 100 nF filter (nominal 1 ms RC); add software debounce. [SparkFun schematic](https://cdn.sparkfun.com/assets/6/d/c/6/c/ESP32_Thing_Plus_Schematic.pdf), [ESP32 pin and boot configuration](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf).

Both cables are required when using the onboard buttons: J9 supplies host 3.3 V to R16–R19, and J19 carries button signals and ground. **J19 has no power pin.** Its signals are not I²C and must not connect to SDA/SCL. Button pullups use HOST_3V3, not the RheoBoard converter's local 3.3 V, so the GPIO wiring does not join those supplies. Connect/disconnect the harness with power off; without J9 host power, the button interface is not valid.

For manual assembly, use a five-position segment of a standard straight breakaway header, such as [Adafruit 392](https://www.adafruit.com/product/392), and five short female/female leads such as [Adafruit 1950](https://www.adafruit.com/product/1950). The ESP32 also needs mating male headers. Verify the selected header's fit and pin numbering before soldering, and secure the unkeyed leads against reversal or a one-pin offset.

## Multiple boards

The working system configuration uses an external TCA9548A multiplexer: one RheoBoard per channel, up to eight boards per multiplexer. The master and multiplexer use the master's 3.3 V supply. Each RheoBoard receives 12 V and connects **GND, master-supplied 3.3 V, channel SDA and channel SCL to J9**, preserving the standard four-wire Qwiic connection. The mux switches SDA/SCL; its master-supplied 3.3 V can feed every J9 host interface. All grounds are common. The boards' local 3.3 V outputs remain separate. A bare-header mux module such as Adafruit 2717 needs a correctly pinned Qwiic adapter harness.

Set the mux to 7-bit address **0x71**: A0 high, A1 and A2 low. Select exactly one channel by writing `1 << channel` to its control register and ending the transaction with STOP. Deselect all channels with `0x00`. The branch devices keep the same addresses on every board:

| Device | Address | Purpose |
| --- | --- | --- |
| U5 Honeywell MPRLS | 0x18 | Pressure |
| U7 PCA9685, all address jumpers open | 0x40 | Pumps, valves and indicators |

The TCA9534 has been removed, so there is no onboard device at 0x3F. Each connected four-button bank needs four separate ESP32 input GPIOs plus ground; the I²C mux does not multiplex J19. Do not join button signals from different boards. J19 may remain unconnected on boards whose onboard buttons are not used.

The PCA9685 responds to All Call address 0x70 after reset. Disable All Call during initialization and keep SUB1, SUB2 and SUB3 disabled; their default programmed addresses include 0x71. Do not use a mux at 0x70 with this setup. Changing the PCA address jumpers alone does not resolve the fixed pressure-sensor address collision. [TI mux datasheet](https://www.ti.com/lit/ds/symlink/tca9548a.pdf), [NXP PWM datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf).

Start at 100 kHz with short wiring. Each board has 4.7 kΩ pullups on each side of U13. Include the selected channel's host pullups and the mux module's pullups in the combined resistance and rise-time check; keep the total selected host-segment pullup current within 3 mA. On the selected mux branch specifically, keep the effective pullup resistance **at least 2.2 kΩ per signal**, including parallel module resistors and their tolerances. This limits the voltage drop through the mux's pass switch while it acknowledges commands, preserving U13 B's 0.45 V input limit. The board's 4.7 kΩ alone satisfies this condition; 4.7 kΩ in parallel with an extra 2.2 kΩ does not. Check the actual mux module's resistors. Local sensor-module pullups also combine on the local side. Keep unpowered branches deselected. Confirm signal low levels and rise times with the actual harness before increasing speed or length; the interface is limited to 400 kHz.

Enable **exactly one mux channel at a time**. In addition to duplicate device addresses, TI prohibits joining TCA9517A B sides together. Do not put a B-side repeater or rise-time accelerator upstream of J9. The selected ESP32 and a passive-switch TCA9548A arrangement were checked; arbitrary Qwiic hubs, repeaters and cable combinations are not covered by that check. [TI interface restrictions](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

Provide pullups on the master side of the mux and keep its active-low RESET input pulled high to the master's 3.3 V supply; account for resistors already fitted to the selected module. Connect RESET to a suitable master GPIO for bus recovery. If a branch loses power or holds SDA/SCL low, a hardware reset can disconnect all branches even when an I²C command cannot get through. Serialize channel selection and each board transaction so two software tasks cannot switch channels underneath one another.

## Assembly completion

Use the revised BOM's exact manufacturer part numbers and footprints. The SMT placement file excludes through-hole and manually fitted parts. J2, J3, J19, the two valve headers and the pumps require the manual assembly steps shown in the BOM. U9 and U10 identify the fitted valve headers; the two external FA0520E valves are separately listed accessories.

Inspect the fuse, all three inductors, diode bands, IC pin-1 marks and electrolytic polarities. Q11 is the SOT-223 input MOSFET: pin 1 gate, pin 2/tab drain to the fused jack input, pin 3 source to VIN_RAW. D37's banded cathode faces VIN_RAW/source, and its anode connects to the gate. Confirm these against the new placement preview; the old v1 preview shows D1 instead. Confirm the valve cable polarity against the header markings before insertion. Check pump terminal polarity and support the pump bodies mechanically; soldered wires are not mechanical mounts. Fit the valve bodies and hoses without loading the connectors or sensor port.

The specified pump's drawing gives a 27.0 ±0.2 mm plastic-head diameter and 24.0 ±0.3 mm motor-can diameter. Its body length is 58.2 ±0.1 mm, with a further 6.1 ±0.2 mm axial nozzle projection. Reserve additional room for tubing and its bend radius. The drawing does not dimension the side nozzle's radial reach or guarantee the PCB terminal-hole fit, so check the supplied pumps and hose routing physically. [Pump dimension drawing](https://cdn-shop.adafruit.com/product-files/4699/4699_C14656_diagram.jpg).

The [JLCPCB top placement](Archive_v1_outputs/8815214A_Y73_SMT026093063736_top.png) belongs to v1 order 8815214A_Y73 / SMT026093063736. A new v1.05 order requires a new placement preview matching the revised BOM and placement export.

## Controller initialization and channel map

**Accepted prototype risk:** U5's reset pin retains a pull-up and TP3 access. The maintainer requested no reset-circuit change. Waiting in firmware alone does not guarantee the required reset; reliable startup and recovery remain unmeasured. See the retained [startup finding](Verification/REVIEW_REPORT.md).

For cold startup, keep all loads OFF until the three rails are stable. There is no TCA9534 initialization in this revision. Configure U7 for non-inverted totem-pole outputs; initialize its used channels FULL OFF, disable All Call/Sub Call addressing, set its common frequency using the specified sleep/prescaler sequence, wake the oscillator and wait at least 500 µs. [NXP PCA9685 datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf).

| Function | Controller channel | Physical circuit |
| --- | --- | --- |
| Pump 1 | PCA9685 LED0 | U8, Q3 |
| Pump 2 | PCA9685 LED1 | U11, Q4 |
| Indicators | PCA9685 LED2–LED4 | Existing indicators, unchanged |
| Valve 1 | PCA9685 LED5, pin 11 | U10, Q10, gate resistor R55 |
| Valve 2 | PCA9685 LED6, pin 12 | U9, Q8, gate resistor R36 |
| Valve buttons | ESP32 inputs through J19 pins 2/3 | SW1/SW2, active LOW |
| Pump buttons | ESP32 inputs through J19 pins 4/5 | SW3/SW4, active LOW |

Command valves with the PCA9685's **FULL ON/FULL OFF bits**, keeping steady binary operation. In register order ON_L, ON_H, OFF_L, OFF_H, ON is `00 10 00 00` and OFF is `00 00 00 10` (hex bytes). LED5 occupies 0x1A–0x1D and LED6 0x1E–0x21. Duty value 4095 still produces PWM. This revision does not qualify reduced valve holding power.

Before sending a four-byte register block in one I²C transaction, enable **MODE1.AI = 1** (auto-increment); its reset value is 0. Otherwise, write each register at its individual address. Keep **MODE2.OCH = 0** so the channel updates at STOP, and set both FULL ON and FULL OFF fields explicitly for each command.

Pump duties remain independent at one shared frequency. Use full-OFF/full-ON for endpoints and measure the lowest reliable starting duty, adding a starting pulse if needed. Average motor drive is not calibrated flow. Pumps and valves now share U7's OE, reset, SLEEP and ALL_LED operations. Set frequency during cold initialization with loads off; do not change these global controls casually while loads are running.

Selecting another mux channel or losing communication intentionally leaves the last commanded outputs active while board power stays stable. After a master restart, read and reconcile U7's retained state before issuing new commands. Cold initialization and reconnection are different cases. Buttons require the ESP32 firmware and are not independent hardware emergency stops.

Pump rotation is not reversible with this board. Each pump moves air from its inlet to its outlet. Inflation and suction use the appropriate tube and valve connections. The pump vendor recommends roughly equal run and rest time; this duty guidance is distinct from PWM duty. [Pump specifications](https://www.adafruit.com/product/4699).

The MPRLS0025PA00001A measures 0–25 psi absolute. Convert its raw readings using the selected sensor's transfer function and subtract a measured ambient baseline when gauge pressure is required. Check its status bits and wait for conversion completion. Ambient pressure should not be assumed to read zero. [Honeywell sensor datasheet](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf).

## Hardware acceptance

These are physical measurements to record on the first assembled v1.05 boards. They remain open until performed.

| Check | Evidence to record |
| --- | --- |
| Unpowered inspection | Fitted identities and orientation; no bridges; no sustained rail-to-ground short after capacitors settle. |
| Initial power, host and loads disconnected | Input current and approximately 3.3 V, 4.52 V and 6.0 V rails. Stop for current limiting, unexpected voltage or heating. |
| Supply startup and hot plug | Scope fused input, VIN_RAW, protected VIN, Q11 gate-source voltage and all three outputs. Verify protected VIN remains within the buck's operating range, including transient peaks. |
| New reverse-polarity stage | With a current-limited test setup, verify Q11 pinout, gate voltage, voltage drop, temperature and reverse-input behavior. Live reversal needs a controlled bench test; the P-MOS alone is not an ideal-diode reverse-current controller. |
| Sensor startup and brownout | Record VDD/RES and initialization across power cycles and supply dips; the retained reset circuit is an accepted prototype risk. |
| Qwiic power states | Test host-only power, board-only power, both powered and each power-up order. Measure both 3.3 V rails for backfeed and both SDA/SCL pairs for valid levels, edges and leakage. |
| Buttons and GPIO cable | Verify J19 pin order, each switch/input mapping, inactive HIGH/pressed LOW, debounce, and host-only/board-only power behavior. |
| One load at a time | Correct physical channel, pump starting/stall current, valve operation, supply droop and flyback waveform. Keep stall tests brief and current limited. |
| Combined loads and PWM | Startup, load-step overshoot/undershoot, switching ripple, input current and temperatures over the intended run/rest cycle. |
| Communication interruption | Last commanded state remains while board power stays stable; reconnect/readback works; brownout returns to a known state. |
| Multiple boards | Select each mux channel independently, confirm the two onboard device addresses, check bus edges and absence of cross-board actuation or supply backfeed. |
| Pressure and mechanics | Ambient baseline, comparison with known pressure, hose/valve routing, leaks, connector fit and enclosure clearance. |

The four reduced bulk capacitors remain 22 µF / 50 V and bring each pump/valve rail to approximately 88.3 µF nominal, within TI's 22–110 µF recommended range. The 105.9 µF estimate applies positive initial tolerance only; it is not a full temperature/DC-bias worst case. More output capacitance is not automatically better for a compensated converter. This calculation does not establish transient stability; capacitance under bias, ESR, wiring inductance and real load behavior remain part of the measurements above. [Buck converter datasheet](https://www.ti.com/lit/gpn/tps563203).

Release repeated production only after the revised fabrication checks, supplier placement review and hardware acceptance are complete. The DIY firmware elsewhere in this repository is not automatically compatible with this PCB's channel map and initialization.
