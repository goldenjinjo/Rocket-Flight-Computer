# Flight-Computer

Current hardware Implementation: Bellerophon V3.5

Sensors in use:
MPL3115A2 (Pressure Sensor)
LSM6DSL IMU (3-axis accelerometer and 3-axis gyroscope)

Memory:
Nand Flash Memory 4GB (using SdFat library)

Perpherials:
PWM Buzzer
RGB general purpose LEDs
Pressure sensor, IMU and memory LEDs
4 Pyro Channels for deployment events controlled by MOSFETs
4 PWM Servos designed for active rear fin stabilisation


________________-

The goal of the computer is to autonomously track and control the rocket and control deployment events.

Currently loosely dependent on Arduino framework, but the plan is to move away from it.

## Current features (as of commit #270):
- Full flight state estimation using only the barometer (altitude prediction) (not tested in flight or vacuum chamber yet)
- General sensor processing implementation with rudimentery noise reduction and data smoothing for barometer
- Flight state machine logic with pyro controllers for deployment events at apogee and pre-configured altitude above ground for main parachute
- Non blocking timer class implementation
- Non blocking LED and buzzer control
- Proof of concept sensor fusion implementation that allows sensors to be dynamically added and removed from the flight logic and logging system
- Data and flight events logging system, with unique files generated (with unique name) on each reboot (still needs to be reconfigured to only create new files if a flight event happens)
- Simple python serial communication interface to communicate with flight computer
- Configuration file management system using flash memory to store configuration variables and load them back in between resets of the flight computer
- File management system with ability to download files to the interfacing computer and to delete all data and log files from the flight computer
- Manual fin control mode with ability to set zero deflection point of the fins to the config file with each control input

## Features currently in process (as of commit #270):
- LQG Controller implementation for correcting to zero angular rate on IMU using servo control inputs
- IMU implementation with better data processing and a kalman filter

## Planned Features:
- Full non-blocking serial communication system with a dedicated management class using serial communication mode enum (replacing existing mode system in main.cpp)
- Improved python serial codebase
- Improved 2-way communication and heartbeat system for flight computer and python serial communication
- Unique identifers for each flight computer and integration of those identifers into serial communication
- Improved file management system (more precise downloading and deleting of files)
- Diagnostics file that reports sensor health (if sensors are responding, etc)
- Autonomous calibration mode and proper initilisation sequence for sensors (and have sensor fusion system accept or deny sensors being added based on their health)
- Manual calibration mode for sensors (mostly IMU)
- Bluetooth implementation (hardware dependent)
- Sensor communication through UART
- GPS implementation
- Wireless serial communication implementation through radio transcievers
