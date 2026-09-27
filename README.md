# ESP-32_Quadcopter

Purpose:
This repository documents a summer project dedicated to designing and building a custom quadcopter from scratch. At the core of the drone is a custom-designed PCB featuring an ESP32 microcontroller and an onboard accelerometer. While the final build has not yet achieved stable flight, the project served as an intensive, hands-on exploration of embedded C++, custom PCB routing, and writing raw PID control loops for hardware stabilization.

## Current Status & Known Issues

The physical build and custom PCB are complete, and the ESP32 successfully interfaces with the MPU6050 IMU and all four ESCs. However, the drone currently experiences a thrust imbalance where the rear motors do not reach the same RPM as the front motors, preventing a stable takeoff. 

**Troubleshooting to date:**

* Swapped the rear ESCs to rule out faulty hardware; the RPM deficit persisted.
* The isolated problem likely lies within the firmware's motor mixing logic, a bug in the PWM signal generation routed to the rear pins, or an uncompensated center of gravity on the 3D-printed frame.
