# RheoBoard v1.05 assembly and bringup

This revision retains two independently controlled, fixed-direction pumps and two valves. The pumps are for intermittent use. Losing communication intentionally leaves the last commanded outputs running; removing board power or sending an OFF command stops them. The board has no onboard microcontroller.

Hardware design licensing follows the repository root LICENSE, CERN-OHL-W-2.0. Documentation is CC BY-SA 4.0.

## Power and connectors

Use a regulated, center-positive 12 V DC supply with the plug dimensions required by the PJ-082BH jack: 5.5 mm outside and 2.5 mm inside. For initial testing, use a bench supply with adjustable current limiting. A 12 V, 2 A adapter is a starting selection for one board with the specified loads, subject to measured startup and simultaneous-load current. A larger adapter does not increase the board's allowable current. The input fuse is 2 A and the added eFuse has an approximately 2 A current limit.

The board makes its own 3.3 V logic supply from the 12 V input. It also makes approximately 4.52 V for pumps and 6.0 V for valves. J9 is the dedicated Qwiic master connection: its 3.3 V pin receives power from the USB/battery-powered ESP32 Thing Plus and supplies only the host side of U13. U13 separates the two I²C signal domains without joining the regulated 3.3 V rails. All grounds remain common; this is not galvanic isolation.

| Connection | Pin 1 | Pin 2 | Pin 3 | Pin 4 |
| --- | --- | --- | --- | --- |
| J9 QWIIC MASTER, JST SH | GND | Host 3.3 V input | HOST_SDA | HOST_SCL |
| J1, J12, J17 QWIIC SENSOR, JST SH | GND | Local 3.3 V output | SDA | SCL |
| J3, JST EH 4-pin peripheral | GND | Local 3.3 V output | SDA | SCL |
| J18 LOCAL BUS SERVICE, JST EH 3-pin | GND | SDA | SCL | — |

All four JST SH connectors retain the standard Qwiic order: pin 1 GND, pin 2 3.3 V, pin 3 SDA, pin 4 SCL. Read the PCB pin-1 mark and connector drawing; a cable viewed from its mating end reverses the apparent left-to-right order. Standard Qwiic cable colors are black, red, blue and yellow in that order. [SparkFun Qwiic](https://www.sparkfun.com/qwiic).

Connect the ESP32 to **J9**, using an ordinary four-wire Qwiic cable. J1, J12 and J17 power local sensor modules from RheoBoard and share its local bus. Do not plug another independently powered controller or RheoBoard into these sensor ports. J18 is retained for local service access and bypasses U13; it is no longer the preferred master/mux port.

The checked controller is SparkFun's ESP32 Thing Plus Micro-B **WRL-15663**: Qwiic SDA is GPIO23 and SCL is GPIO22, and the Qwiic power pin connects to its onboard 3.3 V regulator. Other Thing Plus variants require their own power/pin check. U13 is TCA9517ADGKR, with the RheoBoard bus on its A side and the ESP32 on its B side. Both sides have their own supply bypass and 4.7 kΩ pullups. EN follows host power. TI specifies high-impedance bus outputs when either supply is off; verify power sequencing and leakage on the assembled system. [SparkFun schematic](https://cdn.sparkfun.com/assets/6/d/c/6/c/ESP32_Thing_Plus_Schematic.pdf), [TI TCA9517A](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

## Multiple boards

The working system configuration uses an external TCA9548A multiplexer: one RheoBoard per channel, up to eight boards per multiplexer. The master and multiplexer use the master's 3.3 V supply. Each RheoBoard receives 12 V and connects **GND, master-supplied 3.3 V, channel SDA and channel SCL to J9**, preserving the standard four-wire Qwiic connection. The mux switches SDA/SCL; its master-supplied 3.3 V can feed every J9 host interface. All grounds are common. The boards' local 3.3 V outputs remain separate. A bare-header mux module such as Adafruit 2717 needs a correctly pinned Qwiic adapter harness.

Set the mux to 7-bit address **0x71**: A0 high, A1 and A2 low. Select exactly one channel by writing `1 << channel` to its control register and ending the transaction with STOP. Deselect all channels with `0x00`. The branch devices keep the same addresses on every board:

| Device | Address | Purpose |
| --- | --- | --- |
| U5 Honeywell MPRLS | 0x18 | Pressure |
| U4 TCA9534A | 0x3F | Valves and buttons |
| U7 PCA9685, all address jumpers open | 0x40 | Pumps and indicator PWM |

The PCA9685 responds to All Call address 0x70 after reset. Disable All Call during initialization and keep SUB1, SUB2 and SUB3 disabled; their default programmed addresses include 0x71. Do not use a mux at 0x70 with this setup. Changing the PCA address jumpers alone does not resolve the fixed pressure-sensor address collision. [TI mux datasheet](https://www.ti.com/lit/ds/symlink/tca9548a.pdf), [NXP PWM datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf).

Start at 100 kHz with short wiring. Each board has 4.7 kΩ pullups on each side of U13. Include the selected channel's host pullups and the mux module's pullups in the combined resistance and rise-time check; keep the total selected host-segment pullup current within 3 mA. On the selected mux branch specifically, keep the effective pullup resistance **at least 2.2 kΩ per signal**, including parallel module resistors and their tolerances. This limits the voltage drop through the mux's pass switch while it acknowledges commands, preserving U13 B's 0.45 V input limit. The board's 4.7 kΩ alone satisfies this condition; 4.7 kΩ in parallel with an extra 2.2 kΩ does not. Check the actual mux module's resistors. Local sensor-module pullups also combine on the local side. Keep unpowered branches deselected. Confirm signal low levels and rise times with the actual harness before increasing speed or length; the interface is limited to 400 kHz.

Enable **exactly one mux channel at a time**. In addition to duplicate device addresses, TI prohibits joining TCA9517A B sides together. Do not put a B-side repeater or rise-time accelerator upstream of J9. The selected ESP32 and a passive-switch TCA9548A arrangement were checked; arbitrary Qwiic hubs, repeaters and cable combinations are not covered by that check. [TI interface restrictions](https://www.ti.com/lit/ds/symlink/tca9517a.pdf).

Provide pullups on the master side of the mux and keep its active-low RESET input pulled high to the master's 3.3 V supply; account for resistors already fitted to the selected module. Connect RESET to a suitable master GPIO for bus recovery. If a branch loses power or holds SDA/SCL low, a hardware reset can disconnect all branches even when an I²C command cannot get through. Serialize channel selection and each board transaction so two software tasks cannot switch channels underneath one another.

## Assembly completion

Use the revised BOM's exact manufacturer part numbers and footprints. The SMT placement file excludes through-hole and manually fitted parts. J2, J3, J18, the two valve headers and the pumps require the manual assembly steps shown in the BOM. U9 and U10 identify the fitted valve headers; the two external FA0520E valves are separately listed accessories.

Inspect the fuse, all three inductors, diode bands, IC pin-1 marks and electrolytic polarities. Confirm the valve cable polarity against the header markings before insertion. Check pump terminal polarity and support the pump bodies mechanically; soldered wires are not mechanical mounts. Fit the valve bodies and hoses without loading the connectors or sensor port.

The specified pump's drawing gives a 27.0 ±0.2 mm plastic-head diameter and 24.0 ±0.3 mm motor-can diameter. Its body length is 58.2 ±0.1 mm, with a further 6.1 ±0.2 mm axial nozzle projection. Reserve additional room for tubing and its bend radius. The drawing does not dimension the side nozzle's radial reach or guarantee the PCB terminal-hole fit, so check the supplied pumps and hose routing physically. [Pump dimension drawing](https://cdn-shop.adafruit.com/product-files/4699/4699_C14656_diagram.jpg).

The old JLCPCB image belongs to the v1 order. A new v1.05 order requires a new placement preview matching the revised BOM and placement export.

## Controller initialization and channel map

**Open design issue:** U5's reset pin currently has only a pull-up and TP3 access. A guaranteed sensor reset strategy after stable power is still needed; waiting in firmware alone does not supply that reset. Resolve the [startup finding](Verification/REVIEW_REPORT.md) before treating this initialization sequence as qualified.

For cold startup, keep all loads OFF until the three rails are stable. On the selected mux channel, write TCA9534A register **0x01 = 0x00 first**, then register **0x02 = 0x00**, then configuration register **0x03 = 0xFC**. This sets P0/P1 as outputs only after their output latches are low; P2–P7 remain inputs. Configuring P0/P1 first would briefly apply their reset latch value of HIGH. [TI GPIO datasheet](https://www.ti.com/lit/ds/symlink/tca9534a.pdf).

| Function | Control | Physical reference |
| --- | --- | --- |
| Pump 1 | PCA9685 LED0 | U8, Q3 |
| Pump 2 | PCA9685 LED1 | U11, Q4 |
| Valve 1 | TCA9534A P0 | U10, Q10 |
| Valve 2 | TCA9534A P1 | U9, Q8 |
| Valve buttons | TCA9534A P4/P5, active LOW | SW1/SW2 |
| Pump buttons | TCA9534A P6/P7, active LOW | SW3/SW4 |

Initialize the PCA9685 with non-inverted push-pull outputs and both pump channels set fully OFF. Program its common PWM frequency using the manufacturer's sleep/prescaler sequence, wake its oscillator, and wait at least 500 µs before PWM operation. Set All Call and Sub Call enable bits low. Use full-OFF/full-ON bits for the endpoints; intermediate duty controls average motor drive, not calibrated flow. Both pumps share one PWM frequency, while their duties are independent. Establish the lowest reliable starting duty by measurement, allowing a starting pulse if needed.

Selecting another mux channel does not stop a pump. The PCA and GPIO expander retain their commands. After a master restart, read back the device registers and reconcile the retained state before issuing new commands; do not confuse cold initialization with reconnecting to an already running board. Buttons are read by the master and need firmware handling and debounce; they are not independent hardware emergency stops.

Pump rotation is not reversible with this board. Each pump moves air from its inlet to its outlet. Inflation and suction use the appropriate tube and valve connections. The pump vendor recommends roughly equal run and rest time; this duty guidance is distinct from PWM duty. [Pump specifications](https://www.adafruit.com/product/4699).

The MPRLS0025PA00001A measures 0–25 psi absolute. Convert its raw readings using the selected sensor's transfer function and subtract a measured ambient baseline when gauge pressure is required. Check its status bits and wait for conversion completion. Ambient pressure should not be assumed to read zero. [Honeywell sensor datasheet](https://automation.honeywell.com/content/dam/honeywell-edam/sps/ast/en-us/campaigns/pressure-sensors/documents/sps-siot-mpr-series-datasheet-32332628-ciid-172626.pdf).

## Hardware acceptance

These are physical measurements to record on the first assembled v1.05 boards. They remain open until performed.

| Check | Evidence to record |
| --- | --- |
| Unpowered inspection | Fitted identities and orientation; no bridges; no sustained rail-to-ground short after capacitors settle. |
| Initial power, host and loads disconnected | Input current and approximately 3.3 V, 4.52 V and 6.0 V rails. Stop for current limiting, unexpected voltage or heating. |
| Supply startup and hot plug | Scope VIN_RAW, protected VIN and all three outputs. Verify protected VIN remains within the buck's operating range, including transient peaks. |
| Sensor startup and brownout | Resolve the U5 reset strategy, then measure VDD and RES together and verify reliable initialization across power cycles and supply dips. |
| Qwiic power states | Test host-only power, board-only power, both powered and each power-up order. Measure both 3.3 V rails for backfeed and both SDA/SCL pairs for valid levels, edges and leakage. |
| One load at a time | Correct physical channel, pump starting/stall current, valve operation, supply droop and flyback waveform. Keep stall tests brief and current limited. |
| Combined loads and PWM | Startup, load-step overshoot/undershoot, switching ripple, input current and temperatures over the intended run/rest cycle. |
| Communication interruption | Last commanded state remains while board power stays stable; reconnect/readback works; brownout returns to a known state. |
| Multiple boards | Select each mux channel independently, confirm all three device addresses, check bus edges and absence of cross-board actuation or supply backfeed. |
| Pressure and mechanics | Ambient baseline, comparison with known pressure, hose/valve routing, leaks, connector fit and enclosure clearance. |

The four reduced bulk capacitors remain 22 µF / 50 V and bring each pump/valve rail to approximately 88.3 µF nominal, within TI's 22–110 µF recommended range. The 105.9 µF estimate applies positive initial tolerance only; it is not a full temperature/DC-bias worst case. More output capacitance is not automatically better for a compensated converter. This calculation does not establish transient stability; capacitance under bias, ESR, wiring inductance and real load behavior remain part of the measurements above. [Buck converter datasheet](https://www.ti.com/lit/gpn/tps563203).

Release repeated production only after the revised fabrication checks, supplier placement review and hardware acceptance are complete. The DIY firmware elsewhere in this repository is not automatically compatible with this PCB's channel map and initialization.
