# Drone Hardware: Bill of Materials (BOM)

Below is the complete list of components required for the Version 3 quadcopter build and the custom ground controller, including both off-the-shelf electronics and custom-manufactured parts.

## Custom Manufactured Parts
| Component | Specification / Origin | Quantity | Notes |
| :--- | :--- | :--- | :--- |
| **Flight Controller Board** | Custom Double-Layer PCB | 1 | Routed in KiCad. (View [KiCad Project Files](../KiCadV3)) |
| **Quadcopter Frame** | Custom FDM 3D Print | 1 | Modeled in SolidWorks and Fusion 360. Includes arms, top plate, and undercarriage. (View [CAD Files](../CadFilesV3)) |
| **Controller Enclosure** | Custom FDM 3D Print | 1 | Houses the ESP32 transmitter board, dual joysticks, and wiring. |

## Off-The-Shelf Electronics
| Component | Specification / Model | Quantity | Notes |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | ESP-WROOM-32 ESP32 Development Board | 2 | 2.4GHz Dual-Core WiFi/Bluetooth. (1 for the flight controller, 1 for the remote controller) |
| **Controller Joysticks** | KY-023 Dual-axis XY Joystick Module | 2 | PS2-style analog joysticks for reading pilot input (Throttle/Yaw, Pitch/Roll) |
| **Motors** | A2212 1000KV Brushless Outrunner | 4 | Configured for an 880g thrust profile on a 3S battery |
| **Propellers** | 1045 CW & CCW Props | 4 | 2 Clockwise, 2 Counter-Clockwise |
| **ESCs** | 40A OPTO 2-6S Brushless ESC | 4 | Handles motor current distribution without an integrated BEC |
| **IMU / Sensor** | GY-521 MPU6050 | 1 | 6-Axis Accelerometer and Gyroscope communicating via I2C |
| **Power Distribution** | Acxico 3-4S PDB with XT60 | 1 | Steps down battery voltage to provide clean 5V/12V logic power |
| **Battery** | 11.1V 3S 2200mAh 50C LiPo | 1 | High-discharge power source for flight |
| **Connectors** | XT60 Pigtails (14AWG Wire) | As needed | Soldered for high-current battery mating and power routing |
