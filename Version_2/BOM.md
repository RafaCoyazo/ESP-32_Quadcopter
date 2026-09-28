# Version 2: Hardware Bill of Materials (BOM)

Below is the list of components for the Version 2 build. This iteration marks the transition away from off-the-shelf prototyping boards to the first custom-designed PCB and the initial attempt at a 3D-printed airframe.

## Custom Manufactured Parts
| Component | Specification / Origin | Quantity | Notes |
| :--- | :--- | :--- | :--- |
| **Flight Controller Board** | Custom Double-Layer PCB | 1 | Early revision routed in KiCad to replace the point-to-point soldered perfboards. |
| **Quadcopter Frame** | Custom FDM 3D Print | 1 | The initial, highly experimental airframe iteration. (Replaced in Version 3 due to structural/flight issues). |

## Off-The-Shelf Electronics
| Component | Specification / Model | Quantity | Notes |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | LAFVIN Nano V3.0 (ATmega328P) | 2 | Retained from Version 1 for both flight and ground control logic. |
| **Wireless Module** | CC1101 Multi-Band Wireless Module | 2 | Swapped in to replace the NRF24L01+. Includes an SMA antenna for improved range and stability.[cite: 18] |
| **Motors** | A2212 1000KV Brushless Motors 13T | 4 | High-torque outrunner motors equipped with pre-soldered 3.5mm banana plugs. |
| **ESCs** | 30A OPTO 2-6S Brushless ESC | 4 | Electronic speed controllers for power distribution to the motors. |
