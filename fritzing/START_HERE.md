# Fritzing Starter

You do not need to start from scratch.

Several of your physical parts already have community Fritzing parts available. The fastest path is:

1. Import the custom parts listed below into Fritzing.
2. Place the parts in Breadboard view.
3. Use `wiring-reference.csv` in this folder as the source of truth while you connect everything.
4. Use the proposed control-pin map below as your starting point.

## Parts To Import

### Exact or near-exact parts

- Arduino Uno R4 Minima part:
  - Forum thread: <https://forum.fritzing.org/t/arduino-r4-minima/20064>
  - Attachment on that page: `Arduino-R4-Minima.fzpz`
- TB6600 v1.2 stepper driver:
  - Forum thread: <https://forum.fritzing.org/t/where-can-i-get-tb6600-v1-2-parts/17237>
  - Attachment on that page: `tb6600-v1.2-module.fzpz`
- Linear actuator:
  - Forum thread: <https://forum.fritzing.org/t/linear-actuator/12343>
  - Attachment on that page: `linear-actuator.fzpz`
  - If your actuator has an internal feedback potentiometer, also look at `linear-actuator+pot.fzpz` on the same page

### Good proxies

- IMU:
  - I did not find a ready-made `Adafruit ICM-20948` part in the sources I checked
  - Use an `MPU-9250` breakout as a proxy, or a generic 8-pin breakout board
  - Forum thread for MPU-9250 part: <https://forum.fritzing.org/t/new-part-for-mpu-9250/6624>
- Industrial joystick:
  - Use a generic joystick / dual-potentiometer joystick part
  - Fritzing forum guidance says the existing joystick parts are electrically fine even if the breadboard art differs
  - Reference thread: <https://forum.fritzing.org/t/part-resquest-joystick-part-needed/9761>
- Boost converters:
  - Use a generic 4-pin DC-DC converter or a labeled rectangle part
  - These are only in the power path, so visual accuracy matters more than exact electrical metadata
- CW / CCW limit switches:
  - Use generic microswitch parts
  - For the hardware drawing, use `normally closed (NC)` wiring because it is the safer default for end stops

## Proposed Pin Map

This is a hardware-first suggestion, not a statement about the current firmware:

- `D2` = CCW limit switch
- `D3` = CW limit switch
- `D4` = actuator direction
- `D5` = actuator PWM / speed
- `D6` = joystick button / push switch
- `D7` = TB6600 `DIR`
- `D8` = TB6600 `PUL / STEP`
- `D9` = TB6600 `ENA` optional
- `A0` = supply-voltage sense divider optional
- `A1` = joystick X axis
- `A2` = joystick Y axis optional
- `SDA` / `SCL` = IMU on I2C

## Suggested Layout

Place parts in roughly this order:

1. `Arduino Uno R4 Minima`
2. `MD10C-style actuator driver` or your actual linear actuator driver board
3. `Linear actuator`
4. `TB6600`
5. `NEMA 23 stepper motor`
6. `Joystick`
7. `CW` and `CCW` limit switches
8. `ICM-20948` proxy breakout
9. `24V supply`
10. `Boost converter(s)` only if they actually belong in your power architecture

Keep power wiring visually separate from signal wiring if possible:

- red for `24V` or positive supply
- black for `GND`
- green or yellow for control signals
- blue for I2C

## Power Plan

Use the diagram as a hardware map first, then adapt the firmware to match it.

Recommended baseline:

- `24V bus` feeds the linear actuator driver motor supply
- `24V bus` feeds the `TB6600` motor supply
- `Uno R4 Minima` may be powered from `VIN` because the official board supports `6-24V` on `VIN`
- if motor noise causes resets later, split the logic supply from the motor bus with a dedicated regulator

Important note about your listed converters:

- the modules you listed are `boost` converters
- a boost converter can raise voltage, not step `24V` down to `5V`
- so do **not** draw them as `24V -> 5V` supplies
- only include them if your real system starts from a lower-voltage source and you are intentionally boosting one rail

## Wiring Notes

- The driver in your original diagram looks like a `Cytron MD10C` or a similar brushed DC motor driver.
- If it is an `MD10C`, wire it in `PWM + DIR` mode.
- For the limit switches, use `COM` and `NC` so an open circuit looks like a fault.
- The Adafruit `ICM-20948` breakout accepts `3-5V` on `VIN` and has level-shifted I2C, so it can sit directly on the `Uno R4 Minima` I2C bus.
- For the industrial joystick, wire both axes now even if you only use one in software at first.
- The `TB6600` input side varies a little by module labeling; confirm `PUL`, `DIR`, `ENA`, and supply terminals against the markings on your exact driver before finalizing the drawing.

## Next Step

After you import the parts, open `wiring-reference.csv` and wire the sketch row by row.
