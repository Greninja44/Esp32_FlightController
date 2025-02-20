# ESP32 Drone Flight Controller

## Overview
This project is a drone flight controller based on the ESP32 microcontroller. It manages flight stabilization, motor control, and sensor data processing to enable  flight.

## Features
- Uses ESP32 for real-time flight control
- Supports MPU6050 for IMU (gyroscope & accelerometer)
- Controls four brushless motors via ESCs (Electronic Speed Controllers)l
- PID-based flight stabilization


## Get values from gyroscope
Gyro__values.ino Handwritten code after Understanding the architecture of MPU6050 can be used instead of MPU Libraries
 Connections:
- Sensor------>ESP32
- VCC------->5V
- GND------->GND
- SCL------->GPIO 22
- SDA------->GPIO 21
