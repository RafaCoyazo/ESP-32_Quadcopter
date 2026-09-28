# Version 2: Failure Analysis & Root Cause Analysis

## Overview
Version 2 was designed to resolve the wireless link drops and signal instability observed in the initial Version 1 breadboard setup. The objective was to eliminate point-to-point jumper wire noise by routing a custom double-layer PCB in KiCad and upgrading the RF transceivers. However, despite custom PCB fabrication and hardware iteration, persistent communication failures prevented stable flight operations, ultimately prompting the architectural pivot to the ESP32 in Version 3.

## Initial Problem Statement and Hypotheses
During testing on Version 1, the flight controller experienced frequent loss of control signals and telemetry dropouts. Two primary hypotheses were formed to explain the failures:

First, parasitic noise and trace impedance. Long jumper wires, breadboard contact resistance, and unshielded connections were introducing electromagnetic interference and signal degradation into the SPI bus lines between the Arduino Nano and the radio module.

Second, transceiver limitations. The initial 2.4 GHz NRF24L01+ modules were overly sensitive to signal attenuation and power rail ripple caused by motor ESC current draws.

## Experimental Iterations and Diagnostics
To isolate and systematically resolve these issues, two major revisions were tested in Version 2:

1. Fabrication of Custom Double-Layer PCB
Designed and fabricated a custom double-layer flight controller PCB in KiCad to replace all breadboards and point-to-point wiring. The board integrated socket headers for the LAFVIN Arduino Nano V3.0, dedicated ground copper pours, and direct trace routing to the radio header. Result: While physical connections were secured and breadboard intermittent contact issues were eliminated, the radio communication link remained erratic and frequently failed during initialization or mid-operation.

2. Transceiver Upgrade (NRF24L01+ to CC1101)
Replaced the 2.4 GHz NRF24L01+ transceivers with CC1101 multi-band wireless modules equipped with external high-gain SMA dipole antennas to improve RF penetration and signal reliability. Result: The CC1101 modules failed to establish a consistent real-time control link. Data packets were frequently dropped or corrupted, and transceiver state machines frequently froze during execution.

## Root Cause Breakdown
After exhaustive hardware and software troubleshooting, three core design bottlenecks were identified:

A. Microcontroller Processing and SPI Overhead
The 8-bit ATmega328P (16 MHz single-core) microcontroller on the Arduino Nano lacked the processing bandwidth required to simultaneously process high-frequency MPU6050 IMU sensor reads, run active PID control calculations, execute hardware PWM output updates, and handle complex SPI transceiver polling and buffer management.

B. SPI Bus and Driver Instability
External SPI radio modules require heavy software driver polling and manual re-transmission handling. Any minor voltage drop or timing jitter caused by motor current spikes resulted in the SPI bus hanging, locking up the microcontroller's main execution loop.

C. Airframe Mechanical Flexibility
The initial 3D-printed airframe lacked structural stiffness, causing excessive motor vibrations to transfer directly into the PCB and IMU sensor, further amplifying noise across the power and signal traces.

## Key Lessons and Pivot to Version 3
Rather than continuing to patch an architectural combination of the Arduino Nano and external SPI radio that was fundamentally constrained by processing power and bus vulnerability, the project was restructured for Version 3:

Microcontroller Upgrade: Migrated from the 8-bit ATmega328P to the 32-bit dual-core ESP32, providing massive processing overhead for real-time PID loops.

Protocol Shift (ESP-NOW): Eliminated external SPI transceivers entirely in favor of the ESP32's native ESP-NOW protocol. ESP-NOW operates directly on the MAC layer, drastically reducing transmission latency and hardware points of failure.

Airframe Redesign: Redesigned the 3D-printed frame in CAD with optimized infill geometry and wall thickness to isolate motor vibrations from the flight controller.
