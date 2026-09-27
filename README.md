# ESP-32_Quadcopter

Purpose:
This repository documents a summer project dedicated to designing and building a custom quadcopter from scratch. At the core of the drone is a custom-designed PCB featuring an ESP32 microcontroller and an onboard accelerometer. While the final build has not yet achieved stable flight, the project served as an intensive, hands-on exploration of embedded C++, custom PCB routing, and writing raw PID control loops for hardware stabilization.

## Current Status & Known Issues

The physical build and custom PCB are complete, and the ESP32 successfully interfaces with the MPU6050 IMU and all four ESCs. However, the drone currently experiences a thrust imbalance where the rear motors do not reach the same RPM as the front motors, preventing a stable takeoff. 

**Troubleshooting to date:**

* Swapped the rear ESCs to rule out faulty hardware; the RPM deficit persisted.
* The isolated problem likely lies within the firmware's motor mixing logic, a bug in the PWM signal generation routed to the rear pins, or an uncompensated center of gravity on the 3D-printed frame.

## Hardware & Design

* **Flight Controller Evolution:**
  * **Version 1:** Initial proof-of-concept testing was done using an Arduino and jumper wires.
  * **Version 3:** Upgraded to a custom double-layer PCB (View the [KiCad Project Files](./Version_3/KiCadV3)) to integrate the ESP32 and peripherals, eliminating fragile wiring and reducing electrical noise.
 * **Schematic Design**
*(Below: The wiring schematic integrating the ESP32 microcontroller, MPU6050 IMU, and peripheral modules.)*
  * <img width="780" height="595" alt="image" src="https://github.com/user-attachments/assets/aba6ac5e-01d4-4533-a27e-a9dbf1184d4b" />
  <img width="488" height="682" alt="image" src="https://github.com/user-attachments/assets/547577a7-10a6-4643-bf57-f2cb0859f740" />


* **Microcontroller:** ESP32.
* **Sensors:** Onboard MPU6050 IMU for 6-axis spatial tracking. **Frame Design & Manufacturing** *(Below: The original CAD model...)*
<img width="952" height="566" alt="image" src="https://github.com/user-attachments/assets/6e87f3b6-8fb6-4eca-8e27-060713a9e57b" />
rplace with image]


## Software Stack
* **Language:** Embedded C++.
* **Flight Logic:** Custom-written PID control loops to process IMU data and calculate real-time motor compensation. 
* **Communication:** I2C protocol for reading sensor telemetry and PWM signal generation for the ESCs.

## Project Media
*(Note to self: Upload a photo of the completed drone, a screenshot of the KiCad PCB layout, and a render of the CAD frame here!)*
