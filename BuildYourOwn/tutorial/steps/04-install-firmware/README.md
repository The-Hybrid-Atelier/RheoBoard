# Step 04: Install firmware

- **Time:** ~30–60 min (first-time ESP32 toolchain setup)
- **Difficulty:** moderate (requires computer + USB)

Flash **`2P1VX`** — BLE firmware for the 2P1V rig. Source:
[`../../../software/2P1VX/2P1VX.ino`](../../../software/2P1VX/2P1VX.ino).

## What you'll need for this step

- **Parts:** SparkFun ESP32 Thing Plus (wired but motor supplies can stay off for upload).
- **Design files:** none — see [`../../../software/README.md`](../../../software/README.md).
- **Tools:** computer, USB-C cable, Arduino IDE 2.x

## Instructions

1. Install [Arduino IDE](https://www.arduino.cc/en/software) 2.x.
2. **Boards Manager:** add ESP32 package URL from [`../../../references.md`](../../../references.md)
   (Espressif `esp32` core). Install **esp32 by Espressif Systems**.
3. **Libraries** (Library Manager): SparkFun Qwiic Button, SparkFun MicroPressure.
4. **ThingPlusBLEOSC:** copy or symlink the local library into your Arduino `libraries/` folder
   (path noted in [`../../../references.md`](../../../references.md)) — not on Library Manager.
5. Open `BuildYourOwn/software/2P1VX/2P1VX.ino` from this repo (or your sketchbook copy — keep
   them in sync).
6. Board: **ESP32S3 Dev Module** or **SparkFun ESP32 Thing Plus**; select the USB serial port.
7. Upload. Open Serial Monitor @ **115200** baud.
8. **Success signal:** boot message includes `2P1VX initialized`. Optional: type `REP` on serial
   (with motor power on and plumbing complete) to exercise the pneumatic cycle.

## Media

Screenshot of board + port selection → save as `images/ide-settings.png` when captured.

## Tips / common mistakes

- Upload failures on ESP32-S3: hold BOOT if needed, try a data-capable USB-C cable.
- If MPRLS is missing, firmware may still boot but pressure reads will fail — check Qwiic cable.
- Full OSC/API docs: [`../../../software/2P1VX/README.md`](../../../software/2P1VX/README.md).

## Check before moving on

- [ ] Firmware upload completes without error
- [ ] Serial Monitor shows `2P1VX initialized` @ 115200
