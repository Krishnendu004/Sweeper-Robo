# Scrap-Hack

### A small ultrasonic obstacle-detecting sweeper robot.

This Arduino project controls a rear drive wheel and a pair of front rotating disks through a dual-channel motor driver. An HC-SR04 ultrasonic sensor checks for a nearby wall. When the measured distance is under 15 cm, the sketch stops sending direction signals to both motor channels; otherwise, it runs the drive wheel in reverse and spins the disks.

## What it does

- Reads distance from an HC-SR04 ultrasonic sensor.
- Runs two motor-driver channels: a rear drive wheel and front rotating disks.
- Sets both motor-channel PWM outputs to full duty (`255`).
- Sets both motor directions to LOW when an obstacle is detected closer than 15 cm.
- Prints a wall warning to the Serial Monitor when that threshold is crossed.

## Hardware

- Arduino-compatible board with `analogWrite()` support on pins 3 and 11
- Dual-channel H-bridge motor driver
- Rear drive motor/wheel and front rotating-disk motor(s), connected to the driver outputs
- HC-SR04 ultrasonic sensor
- Suitable motor power supply, jumper wires, and a USB cable for programming

### Pin map

| Module | Pin | Connect to |
|---|---|---|
| Motor driver, channel A enable | ENA | Arduino pin 3 |
| Motor driver, channel A direction | IN1 | Arduino pin 5 |
| Motor driver, channel A direction | IN2 | Arduino pin 6 |
| Motor driver, channel B enable | ENB | Arduino pin 11 |
| Motor driver, channel B direction | IN3 | Arduino pin 9 |
| Motor driver, channel B direction | IN4 | Arduino pin 10 |
| HC-SR04 | TRIG | Arduino pin 12 |
| HC-SR04 | ECHO | Arduino pin 13 |
| HC-SR04 | VCC | Appropriate sensor supply |
| Arduino, sensor, and motor driver | GND | Common ground |

Channel A is used for the rear drive wheel; channel B is used for the front rotating disks. Connect each motor to the corresponding driver output terminals. Driver terminal names and jumper arrangements vary by module; consult its documentation and disconnect enable jumpers if required for Arduino PWM control.

## Safety first

> [!WARNING]
> HC-SR04 ECHO can output about 5 V. If your board's GPIO is not 5 V tolerant, use a suitable voltage divider or level shifter between ECHO and pin 13.

- Use a motor supply suited to the motors and driver; do not power motors from an Arduino GPIO pin.
- Connect the Arduino, sensor, and motor-driver grounds together.
- Check the driver board's power and regulator requirements before wiring it. Do not assume its 5 V terminal is always an output.
- Disconnect power before changing wiring, and keep wheels and rotating disks clear of hands during tests.
- Raise the wheels and secure the robot for initial tests. The motors can start as soon as the board is powered.

## Upload the firmware

1. Install the Arduino IDE and the board package appropriate for your Arduino-compatible board.
2. Connect the board over USB and open [`main/main.ino`](main/main.ino).
3. In **Tools**, select the correct board and serial port.
4. Upload the sketch.
5. Open **Serial Monitor** at **9600 baud**.
6. Test with the robot secured and its wheels and disks clear.

The sketch uses only Arduino core functions and needs no additional library.

## How it works

1. The sketch sends a 10 μs trigger pulse to the HC-SR04.
2. It waits up to 25,000 μs for the ECHO pulse, then estimates distance with `duration × 0.034 / 2`.
3. When the computed distance is greater than 0 and less than 15 cm, it prints `Wall at ... cm. BRAKING.` and sets IN1, IN2, IN3, and IN4 LOW.
4. For all other computed distances, it sets IN1 LOW / IN2 HIGH for the rear drive channel and IN3 HIGH / IN4 LOW for the disk channel.
5. The loop waits 50 ms before taking the next reading.

| Setting | Value | Purpose |
|---|---:|---|
| Drive PWM (`driveSpeed`) | `255` | Full PWM duty request on pin 3 |
| Disk PWM (`diskSpeed`) | `255` | Full PWM duty request on pin 11 |
| Obstacle threshold | `15` cm | Stop direction signals when distance is strictly less than this value |
| Echo timeout | `25,000` μs | Maximum time to wait for an echo |
| Serial speed | `9600` baud | Serial Monitor communication |

### Reading and motor behavior to note

- A reading of exactly 15 cm follows the run path; the condition is strictly `< 15`.
- When `pulseIn()` times out it returns zero. The current sketch calculates that as a zero distance, which does **not** satisfy `distance > 0 && distance < 15`; it therefore follows the run path. An absent echo is not currently treated as an obstacle or a safe stop.
- The direction pins are set LOW on an obstacle, but the motor driver's resulting stop/brake behavior depends on its design. The enable PWM outputs remain at `255`.
- The sketch requests maximum PWM on both channels and has no acceleration, speed adjustment, steering, or obstacle-avoidance turn.

## Project files

| Path | Contents |
|---|---|
| [`main/main.ino`](main/main.ino) | Arduino sketch |
| [`README.md`](README.md) | Project overview, wiring reference, and firmware behavior |
