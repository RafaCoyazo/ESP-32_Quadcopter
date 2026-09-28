# Version 2: Failure Analysis & Root Cause Analysis

## Overview
Version 2 was designed to resolve the wireless link drops and signal instability observed in the initial Version 1 breadboard setup. The objective was to eliminate point-to-point jumper wire noise by routing a custom double-layer PCB in KiCad and upgrading the RF transceivers. However, despite custom PCB fabrication and hardware iteration, persistent communication failures prevented stable flight operations, ultimately deciding to pivot to the ESP32 in Version 3.

## Initial Problem Statement and Hypotheses
During testing on Version 1, the flight controller failed to establish a stable wireless connection between the Arduino Nano and the initial 2.4 GHz NRF24L01+ radio module. 

Primary Hypothesis: High jumper wire impedance, breadboard contact resistance, and causing signal degradation into the SPI bus lines, preventing proper module initialization.
## Experimental Iterations and Diagnostics
To test this hypothesis and systematically isolate the failure, so I made a custom PCB and tested across two transceiver configurations:

1. Custom Double-Layer PCB Fabrication (NRF24L01+ Testing)
Designed and fabricated a custom double-layer flight controller PCB in KiCad to eliminate all long jumper wires and point-to-point breadboard connections. The board integrated socket headers for the LAFVIN Arduino Nano V3.0, dedicated ground copper pours, and direct trace routing to an NRF24L01+ header.

Result: Eliminating the long wires via the custom PCB did not resolve the connection failure, proving that signal path length and breadboard noise were not the primary root cause.

3. Transceiver Module Swap (CC1101 on original PCB Layout)
Using the same custom PCB design, the NRF24L01+ module was replaced with a CC1101 multi-band wireless module equipped with an external SMA antenna to test if switching radio architecture and frequency bands would establish a connection.
Result: The CC1101 module on the custom PCB also failed to achieve reliable communication, meaning there was another cause for this problem.

## Root Cause Breakdown
After these hardware iterations across breadboards and custom PCBs, three core problems were found:

A. Microcontroller Processing and SPI Overhead
The ATmega328P (16 MHz single-core) microcontroller on the Arduino Nano lacked the processing bandwidth to reliably manage low-level SPI transceiver state machines while simultaneously executing high-frequency MPU6050 IMU reads, PID calculations, and ESC PWM updates.

B. SPI Bus Sensitivity and Driver Instability
External SPI radio modules require heavy software polling and strict timing buffers. Power ripple from motor draws or minor bus timing delays caused the SPI driver to hang, freezing communication entirely regardless of whether wires or copper PCB traces were used.

C. Airframe Mechanical Flexibility
The initial 3D-printed airframe lacked structural rigidity, allowing motor vibrations to transfer directly to the flight controller board and IMU sensor, adding mechanical noise into the system.

## Key Lessons and to improve on to Version 3
Rather than continuing to debug an architectural combination (Arduino Nano + External SPI Radio) that repeatedly failed on both breadboards and custom PCBs, the project was restructured for Version 3:

Microcontroller Upgrade: Migrated from the ATmega328P Ardunio to the ESP32 DevKit , providing better clock speed and memory overhead and a built in comunication module.

(ESP-NOW): Scrapped external SPI transceivers entirely in favor of the ESP32's native ESP-NOW protocol. ESP-NOW communicates directly on the Wi-Fi MAC layer, completely removing SPI hardware buses, radio sub-boards, and external driver overhead.

Airframe Redesign: Redesigned the 3D-printed frame in CAD with optimized infill geometry and wall thickness to isolate motor vibrations from the flight controller board.
