# Version 1: Hardware Bill of Materials (BOM)

Below is the list of early components used in Version 1 of the quadcopter and ground station, prior to the migration to custom PCBs and ESP32 microcontrollers.

| Component | Specification / Model | Quantity | Notes |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | LAFVIN Nano V3.0 (ATmega328P) | 2 | Driven by the CH340 chip. Used for the original flight controller and remote logic.[cite: 12] |
| **Wireless Module** | HiLetgo NRF24L01+ 2.4G Transceiver | 2 | Handled the RF communication link between the remote and the aircraft.[cite: 13] |
| **Prototyping Board** | ElectroCookie Mini PCB Prototype Board | As needed | Used to solder the Nano, NRF24L01+, and wiring point-to-point before custom PCBs were designed.[cite: 16] |
| **Motors** | A2212 1000KV Brushless Motors 13T | 4 | High-torque outrunner motors equipped with pre-soldered 3.5mm banana plugs.[cite: 15] |
| **ESCs** | 30A OPTO 2-6S Brushless ESC | 4 | Electronic speed controllers for power distribution to the motors.[cite: 14] |
