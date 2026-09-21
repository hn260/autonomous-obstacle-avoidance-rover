# System Architecture

## Physical rover

Battery → regulated power → Arduino Uno / motor driver / sensors

Arduino Uno → L298N → four BO motors

Arduino Uno → HC-SR04 + servo → obstacle distance measurements

## Simulation

ROS 2 + Gazebo → simulated robot model → simulated environment → sensor/motion interfaces

The simulation and physical rover represent the same overall robotics problem, but they should be treated as separate implementations unless a verified hardware-to-ROS interface is added.
