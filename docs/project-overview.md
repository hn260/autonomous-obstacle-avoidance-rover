# Project Overview

This project explores low-cost autonomous obstacle avoidance through two complementary implementations:

1. A physical four-wheel rover using Arduino-based sensing and motor control.
2. A ROS 2 + Gazebo simulation used to study robotic simulation, models, environments, sensors, and navigation concepts.

The physical rover detects obstacles using an HC-SR04 ultrasonic sensor mounted on a servo. When an obstacle is detected within the configured threshold, the controller scans alternative directions and selects a turn based on measured clearance.

The project was developed as an engineering/robotics prototype rather than as a production autonomous navigation stack. Advanced systems such as SLAM, Nav2, autonomous localization, and ROS-controlled hardware are not claimed unless their source implementation is added later.
