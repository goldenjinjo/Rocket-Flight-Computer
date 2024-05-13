#ifndef IMUSensor_h
#define IMUSensor_h

#include <Wire.h>
#include <LSM6DSLSensor.h>

class IMUSensor {
private:
    LSM6DSLSensor imu; // LSM6DSLSensor object
    TwoWire* wire; // Pointer to the I2C bus
    uint8_t address; // I2C address of the sensor
    uint8_t accelRange; // Accelerometer range
    uint16_t gyroRange; // Gyroscope range
    float accelSense; // Store  accelerometer sensitivity as an instance variable
    float gyroSense; // Store gyroscope sensitivity as an instance variable

public:
    IMUSensor(TwoWire* i2c, uint8_t addr, uint8_t accelRange, uint16_t gyroRange);
    void init(); // Initialize the IMU sensor
    void enable(); // Enable the IMU sensor
    float* getAccelerometerData(); // Get accelerometer data as an array
    float* getGyroscopeData(); // Get gyroscope data as an array
};

#endif // IMUSensor_h
