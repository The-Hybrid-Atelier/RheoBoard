# Step 06: Power and data connections

- **Time:** ~15–30 min
- **Difficulty:** easy

Power this design: **12 V adapter** to both L298N motor rails, plus micro-USB to the ESP32. Data
path: USB serial (bench) and BLE (RheoData).

## What you'll need for this step

- **Parts:** 12 V DC adapter (≥ 2 A recommended), micro-USB cable, Qwiic MicroPressure + Qwiic
  Button already wired — see [`../../../BOM.md`](../../../BOM.md).
- **Design files:** power section of [`../../../wiring/2P1V-wiring-diagram.png`](../../../wiring/2P1V-wiring-diagram.png).
- **Tools:** multimeter (recommended)

## Instructions

1. **Common ground:** confirm ESP32 GND, both L298N GND, and 12 V adapter (−) are tied together
   before energizing.
2. **Motor power:** connect **12 V (+)** to both L298N motor power inputs. Polarity per module labels.
3. **ESP32:** power via micro-USB (the same cable used for programming). Do not back-feed 12 V into
   the ESP32.
4. **First power-on:** with pumps/valve off (firmware idle), verify no excessive current draw or hot
   components. Then connect Serial Monitor @ 115200 and confirm MPRLS reads (~ambient) and the
   Qwiic Button responds (LED lights on press).
5. **BLE:** from RheoData, connect to device name **`2P1VX`**.

## Media

Photo of final cable routing; short video of power-on if helpful.

## Tips / common mistakes

- Never run pump/valve loads from the ESP32 5 V pin.
- 12 V at the L298N is the motor **supply rail**; effective drive to ~4.5 V pumps and ~6 V valve
  is set by PWM duty in firmware — tune `rheo/rep/pull/power` and `rheo/rep/push/power` if motion
  is too aggressive.
- If MPRLS reads are stale or `nan`, reseat the Qwiic cable and confirm I2C address `0x18`.
- USB-only power is fine for firmware upload; pneumatic tests need the 12 V adapter.

## Check before moving on

- [ ] Powers up without excessive current draw or component heat
- [ ] Serial pressure read looks plausible at ambient
- [ ] BLE device `2P1VX` visible to host (RheoData)
