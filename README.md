# ESP32 Drone Flight Controller

## Overview
This project is a drone flight controller based on the ESP32 microcontroller. It manages flight stabilization, motor control, and sensor data processing to enable  flight.

## Features
- Uses ESP32 for real-time flight control
- Supports MPU6050 for IMU (gyroscope & accelerometer)
- Controls four brushless motors via ESCs (Electronic Speed Controllers)l
- PID-based flight stabilization

## Hardware Requirements
- ESP32 development board
- MPU6050 IMU sensor
- 4x Brushless motors
- 4x ESCs
- Li-Po battery
- Custom PCB with mounting holes for ESP32 and sensors
- Frame for the drone
- Receiver(5 chanel)
- Transmitter

## Software Requirements
- Arduino IDE / PlatformIO
- ESP32 Board Support Package
- MPU6050 Library
- PID Control Algorithm
- Bluetooth/Wi-Fi communication library

## Setup Instructions
1. **Assemble the Hardware**
   - Mount ESP32 on the custom PCB.
   - Connect the MPU6050 to ESP32 using I2C (SDA, SCL).
   - Connect ESCs to ESP32 PWM pins.
   - Power the system using a Li-Po battery.

2. **Install Required Libraries**
   ```sh
   pip install esptool
   ```
   - Install MPU6050 library from Arduino Library Manager.

3. **Calibrate IMU Sensor**
   - Run the calibration script to get offset values.


## Future Improvements
- GPS integration for navigation
- Barometer for altitude hold
- FPV camera support

## Contributing
Feel free to submit issues and pull requests for enhancements.


