# ESP-32_Quadcopter

Purpose:
This repository documents a summer project dedicated to designing and building a custom quadcopter from scratch. At the core of the drone is a custom-designed PCB featuring an ESP32 microcontroller and an onboard accelerometer. While the final build has not yet achieved stable flight, the project served as an intensive, hands-on exploration of embedded C++, custom PCB routing, and writing raw PID control loops for hardware stabilization.

## Current Status & Known Issues

The physical build and custom PCB are complete, and the ESP32 successfully interfaces with the MPU6050 IMU and all four ESCs. However, the drone currently experiences a thrust imbalance where the rear motors do not reach the same RPM as the front motors, preventing a stable takeoff. 

**Troubleshooting to date:**

* Swapped the rear ESCs to rule out faulty hardware; the RPM deficit persisted.
* The isolated problem likely lies within the firmware's motor mixing logic, a bug in the PWM signal generation routed to the rear pins, or an uncompensated center of gravity on the 3D-printed frame.

## Hardware & Design

## Hardware Specifications

| Component | Description |
| :--- | :--- |
| **Microcontroller Evolution** | **V1 & V2:** Arduino (View [Version 1 Details](./Version_1) and [Version 2 Details](./Version_2))
|**Version 3**| **V3 Details of the Hardware:** [Version 3 Details](./Version_3)
  
* **Flight Controller:**
  * **Version 1:** Initial proof-of-concept testing was done using an Arduino and jumper wires.
  *  **Version 2:** Upgraded to a custom double-layer PCB (View the [KiCad Project Files](./Version_3/KiCadV3)) to show proof of concept with Arduino Nano
  * **Version 3:** Redigned custom double-layer PCB (View the [KiCad Project Files](./Version_3/KiCadV3)) to integrate the ESP32 and peripherals, wiring and reduce electrical noise.
 * **Schematic Design for V3**
*(Below: The wiring schematic integrating the ESP32 microcontroller, MPU6050 IMU, and peripheral modules.)*
  <img width="780" height="595" alt="image" src="https://github.com/user-attachments/assets/aba6ac5e-01d4-4533-a27e-a9dbf1184d4b" />
  <img width="488" height="682" alt="image" src="https://github.com/user-attachments/assets/547577a7-10a6-4643-bf57-f2cb0859f740" />




**Frame Design & Manufacturing** 
*(Below: The original CAD model showcasing the structural assembly, followed by the final 3D-printed physical frame.)*

<img width="952" height="566" alt="image" src="https://github.com/user-attachments/assets/6e87f3b6-8fb6-4eca-8e27-060713a9e57b" />
rplace with image]


## Software 

* **Language & Codebase Evolution:**
  * **V1 & V2:** Arduino / C++ (View [Version 1 Code](./Version_1) and [Version 2 Code](./Version_2)).
  * **V3:** Embedded C++ (View [Version 3 Code](./Version_3/CodeV3)).
* **Flight Logic:** Custom-written PID control loops to process IMU data and calculate real-time motor compensation.
* **Communication:** I2C protocol for reading sensor telemetry and PWM signal generation for the ESCs.

## Project Media
*(Note to self: Upload a photo of the completed drone and video)*

## Takeoff Dynamics & Motor Imbalance Diagnostics

During takeoff testing, the aircraft experienced a severe pitch imbalance where the rear motors visibly and physically generated significantly lower RPM and thrust compared to the front motors, preventing vertical liftoff.

### Diagnosed Causes & Experimental Troubleshooting:

1. **Power Supply & Input Voltage Verification:**
   * **Action:** Measured the primary voltage supply lines under load to check for power starvation or voltage drops to the rear power distribution traces.
   * **Result:** Confirmed that the power supply unit consistently delivers 12V across all board rails and ESC power feeds. Power delivery was ruled out as the root cause.

2. **ESC Timing, Calibration & Hardware Swaps:**
   * **Hypothesis:** Because four individual ESCs are used rather than an integrated 4-in-1 board, timing drift or throttle calibration offsets could cause rotational speed discrepancies between channels.
   * **Action:** Replaced the rear Electronic Speed Controllers with brand-new units to test for hardware degradation and calibration drift.
   * **Result:** Installing new ESCs slightly increased the RPM of one rear motor, but the total thrust was still insufficient to equalize the outputs. This confirms that while minor ESC variances exist, the main bottleneck is not faulty ESC hardware.

3. **Firmware Pitch Control Loops & Sensor Isolation:**
   * **Action:** To test if the MPU6050 IMU feedback loop was falsely commanding lower duty cycles to the rear channels, firmware was modified to temporarily isolate and disable active pitch compensation.
   * **Result:** Testing is currently ongoing to evaluate raw PWM output signal scaling, timer channel allocations on the ESP32 pins, and pitch PID loop behavior.
