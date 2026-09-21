# Hardware

## Main components

- Arduino Uno
- HC-SR04 ultrasonic sensor
- Servo motor for directional scanning
- L298N motor driver
- Four BO geared DC motors
- Rover chassis and wheels
- Li-ion battery pack
- Buck converter / regulated supply

## Architecture

The physical rover uses the Arduino as its embedded controller. The ROS 2/Gazebo work provides a corresponding simulation environment for robotics and navigation experiments.

See `../media/system-block-diagram.jpg` and `../media/autonomous-rover-flowchart.jpg` for the documented system architecture and control logic.
