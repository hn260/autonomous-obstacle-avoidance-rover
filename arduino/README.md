# Arduino Firmware

Firmware for the physical autonomous obstacle avoidance rover.

## Hardware

- Arduino Uno
- HC-SR04 ultrasonic sensor
- SG90 servo motor
- L298N motor driver
- 4x BO geared DC motors

## Control Logic

The rover continuously measures the distance ahead.

When the path is clear, the rover moves forward.

When an obstacle is detected within the configured threshold:

1. The rover stops.
2. The servo moves the ultrasonic sensor to the left.
3. The left-side distance is measured.
4. The servo moves to the right.
5. The right-side distance is measured.
6. The rover selects the direction with greater clearance.
7. The rover turns and resumes forward movement.

## Pin Configuration

| Arduino Pin | Function |
|---|---|
| D2 | HC-SR04 TRIG |
| D3 | HC-SR04 ECHO |
| D5 | L298N ENA |
| D6 | L298N ENB |
| D7 | L298N IN1 |
| D8 | L298N IN2 |
| D9 | Servo |
| D11 | L298N IN3 |
| D12 | L298N IN4 |

> Pin assignments should be verified against the original wiring before using this firmware on the physical rover. This file is a reconstructed implementation based on the project's documented hardware and control flow.
