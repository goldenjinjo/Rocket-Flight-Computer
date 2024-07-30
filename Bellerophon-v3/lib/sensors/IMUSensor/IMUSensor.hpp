#ifndef IMU_SENSOR_HPP
#define IMU_SENSOR_HPP

#include <Wire.h>
#include <LSM6DSLSensor.h>
#include "sensor.hpp"

class IMUSensor: public Sensor<6> {
private:
    // ------------------------- MEMBERS ------------------------- //
    LSM6DSLSensor imu; // LSM6DSLSensor object
    TwoWire* wire; // Pointer to the I2C bus
    uint8_t address; // I2C address of the sensor
    uint8_t accelRange; // Accelerometer range
    uint16_t gyroRange; // Gyroscope range
    float accelSense; // Store accelerometer sensitivity as an instance variable
    float gyroSense; // Store gyroscope sensitivity as an instance variable

    int32_t gyroArray_[3];
    int32_t accelArray_[3];

public:
    // ------------------------- METHODS ------------------------- //
    // Constructor for IMUSensor class
    /**
     * @brief  Constructor for IMUSensor class.
     * @param  i2c Pointer to the I2C bus.
     * @param  addr I2C address of the sensor.
     * @param  accelRange Accelerometer range.
     * @param  gyroRange Gyroscope range.
     */
    IMUSensor(TwoWire* i2c, uint8_t addr, uint8_t accelRange, uint16_t gyroRange);
    
    /**
     * @brief  Initialize the IMU sensor.
     */
    void initialize() override;
    
    /**
     * @brief  Enable the IMU sensor.
     */
    void enable();
    
    /**
     * @brief  Get accelerometer data as an array.
     * @return Pointer to the array of accelerometer data.
     */
    std::array<float, 3> getAccelerometerData();
    
    /**
     * @brief  Get gyroscope data as an array.
     * @return Pointer to the array of gyroscope data.
     */
    std::array<float, 3> getGyroscopeData();
    
    /**
     * @brief  Adjust polling rate (ODR).
     * @param  rate Rate value from 1 to 10 (1.6 - 6664 Hz).
     */
    void setPollRate(char rate);
    
    /**
     * @brief  Get the polling rate (ODR) of the accelerometer.
     * @return Polling rate (ODR) of the accelerometer.
     */
    float getAccelPollRate();
    
    /**
     * @brief  Get the polling rate (ODR) of the gyroscope.
     * @return Polling rate (ODR) of the gyroscope.
     */
    float getGyroPollRate();


    float getData() override {
        return 1;
    };

    void update() override;

    std::array<float, 6> getAllData() override;
};

#endif // IMU_SENSOR_HPP

