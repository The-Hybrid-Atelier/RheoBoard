# Step 04: Install firmware

- **Time:** ~30–60 min (first-time ESP32 toolchain setup)
- **Difficulty:** moderate (requires computer + USB)

Flash the BLE firmware for this design. Source:
[`../../../software/rheometer-firmware/2P1VX.ino`](../../../software/rheometer-firmware/2P1VX.ino).

## What you'll need for this step

- **Parts:** SparkFun ESP32 Thing Plus (wired but motor supplies can stay off for upload).
- **Design files:** none — see [`../../../software/README.md`](../../../software/README.md).
- **Tools:** computer, **micro-USB** cable (data-capable, not charge-only), Arduino IDE 2.x

## Instructions

1. Install [Arduino IDE](https://www.arduino.cc/en/software) 2.x.
2. **Boards Manager:** add ESP32 package URL from
   [`../../../hardware/references/README.md`](../../../hardware/references/README.md)
   (Espressif `esp32` core). Install **esp32 by Espressif Systems**.
3. **Libraries** (Library Manager): SparkFun Qwiic Button, SparkFun MicroPressure, OSC (by Adrian
   Freed).
4. **ThingPlusBLEOSC:** not on Library Manager — clone it into your Arduino `libraries/` folder:
   ```bash
   cd ~/Documents/Arduino/libraries
   git clone https://github.com/cearto/ThingPlusBLEOSC.git
   ```
   Restart the Arduino IDE afterward. (Also depends on ESP32 BLE Arduino by Neil Kolban, usually
   bundled with the `esp32` core already.)
5. Open `BuildYourOwn/software/rheometer-firmware/2P1VX.ino` from this repo (or your sketchbook
   copy — keep them in sync).
6. Board: **SparkFun ESP32 Thing Plus** (or generic **ESP32 Dev Module** — this is a plain
   ESP32-WROOM-32D/E, not S2/S3); select the micro-USB serial port.
7. Upload. Open Serial Monitor @ **115200** baud.
8. **Success signal:** boot message includes `2P1VX initialized`. Optional: type `REP` on serial
   (with motor power on and plumbing complete) to exercise the pneumatic cycle.

## Media

Screenshot of board + port selection → save as `../../../images/ide-settings.png` when captured.

## Tips / common mistakes

- Use a **data-capable** micro-USB cable — charge-only cables won't show a serial port.
- If MPRLS or Button is missing/misaddressed, firmware may still boot but that device's readings
  won't work — check the Qwiic chain order and cable seating.
- Full OSC/API docs: [`../../../software/rheometer-firmware/README.md`](../../../software/rheometer-firmware/README.md).

## Check before moving on

- [ ] Firmware upload completes without error
- [ ] Serial Monitor shows `2P1VX initialized` @ 115200
