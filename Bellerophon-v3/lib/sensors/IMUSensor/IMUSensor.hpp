#ifndef IMU_SENSOR_HPP
#define IMU_SENSOR_HPP

#include <Wire.h>
#include <LSM6DSLSensor.h>
#include "sensor.hpp"

/**
 * @class IMUSensor
 * @brief Wrapper class for the LSM6DSLSensor to provide IMU data.
 *
 * This class provides an interface for initializing, enabling, and retrieving
 * data from the LSM6DSLSensor. It implements the Sensor interface to 
 * ensure compatibility with the SensorFusion system.
 */
class IMUSensor: public Sensor {
private:
    // ------------------------- MEMBERS ------------------------- //
    LSM6DSLSensor imu; ///< LSM6DSLSensor object
    TwoWire* wire; ///< Pointer to the I2C bus
    uint8_t address; ///< I2C address of the sensor
    uint8_t accelRange; ///< Accelerometer range
    uint16_t gyroRange; ///< Gyroscope range
    float accelSense; ///< Accelerometer sensitivity
    float gyroSense; ///< Gyroscope sensitivity

    int32_t gyroArray_[3]; ///< Array to store raw gyroscope data
    int32_t accelArray_[3]; ///< Array to store raw accelerometer data

    /**
     * @brief Initialize the IMU sensor.
     */
    void initialize() override;
    
    /**
     * @brief Enable the IMU sensor.
     */
    void enable();

public:
    // ------------------------- METHODS ------------------------- //
    /**
     * @brief Constructor for IMUSensor class.
     * 
     * @param i2c Pointer to the I2C bus.
     * @param addr I2C address of the sensor.
     * @param accelRange Accelerometer range.
     * @param gyroRange Gyroscope range.
     */
    IMUSensor(TwoWire* i2c, uint8_t addr, uint8_t accelRange, 
              uint16_t gyroRange);
    
    /**
     * @brief Get accelerometer data as an array.
     * 
     * @return Pointer to the array of accelerometer data.
     */
    float* getAccelerometerData();
    
    /**
     * @brief Get gyroscope data as an array.
     * 
     * @return Pointer to the array of gyroscope data.
     */
    float* getGyroscopeData();
    
    /**
     * @brief Adjust polling rate (ODR).
     * 
     * @param rate Rate value from 1 to 10 (1.6 - 6664 Hz).
     */
    void setPollRate(char rate);
    
    /**
     * @brief Get the polling rate (ODR) of the accelerometer.
     * 
     * @return Polling rate (ODR) of the accelerometer.
     */
    float getAccelPollRate();
    
    /**
     * @brief Get the polling rate (ODR) of the gyroscope.
     * 
     * @return Polling rate (ODR) of the gyroscope.
     */
    float getGyroPollRate();

    /**
     * @brief Get the current sensor data.
     * 
     * @return Current sensor data.
     */
    float getData() override {
        return accelArray_[0];
    }

    /**
     * @brief Update the sensor data.
     */
    void update() override;

    /**
     * @brief Get all unique data values available by the sensor.
     * 
     * @return Pointer to an array of current sensor data values.
     */
    float* getAllData() override;

    /**
     * @brief Get the number of sensor values available from the sensor.
     * 
     * @return Number of sensor values.
     */
    size_t getNumValues() override {
        return 6;
    }

    /**
     * @brief Get the names of the sensor values.
     * 
     * @return Comma-separated string of sensor value names.
     */
    std::string getNames() override {
        return "a_x,a_y,a_z,g_x,g_y,g_z";
    }
};

#endif // IMU_SENSOR_HPP



