#ifndef IMU_PROCESSOR_HPP
#define IMU_PROCESSOR_HPP

#include "sensorProcessor.hpp"
#include "dataProcessor.hpp"
#include "IMUSensor.hpp"

/**
 * @class IMUProcessor
 * @brief Class for processing IMU sensor data.
 *
 * This class inherits from SensorProcessor and implements the specific 
 * functionalities for an IMU sensor. Note that this implementation is 
 * not complete, hence many functions return 0.
 */
class IMUProcessor : public SensorProcessor {

public:
    /**
     * @brief Constructor for IMUProcessor.
     * 
     * @param imu Reference to the IMU sensor.
     * @param historySize The size of the history buffer for smoothing 
     *        and differentiation.
     * @param outlierThreshold The threshold for detecting outliers.
     */
    IMUProcessor(IMUSensor& imu, size_t historySize, 
                 float outlierThreshold = 10.0);

    /**
     * @brief Update the IMU sensor data.
     */
    void update() override;

    /**
     * @brief Get the raw data from the IMU sensor.
     * 
     * @return Pointer to the array of IMU sensor data.
     */
    float* getRawData() const override;

    /**
     * @brief Returns the number of unique sensor values.
     * 
     * @return Number of unique sensor values.
     */
    size_t getNumSensorValues() const override;

    /**
     * @brief Get the sensor names.
     * 
     * @return Sensor names as a string.
     */
    std::string getSensorNames() const override;

protected:
    /**
     * @brief Get the estimated altitude.
     * 
     * @return Estimated altitude.
     */
    float getAltitude() const override {
        return 0.0;
    }

    /**
     * @brief Get the estimated vertical velocity.
     * 
     * @return Estimated vertical velocity.
     */
    float getVerticalVelocity() const override {
        return 0.0;
    }

    /**
     * @brief Get the estimated acceleration.
     * 
     * @return Estimated acceleration.
     */
    float getAcceleration() const override {
        return 0.0;
    }

    /**
     * @brief Get the ground altitude.
     * 
     * @return Zero, as IMU can not calculate ground altitude.
     */
    float getGroundAltitude() const override {
        return 0.0;
    }

    /**
     * @brief Get the maximum recorded altitude.
     * 
     * @return Maximum recorded altitude.
     */
    float getMaxAltitude() const override {
        return 0.0;
    }

    /**
     * @brief Get the maximum recorded vertical velocity.
     * 
     * @return Maximum recorded vertical velocity.
     */
    float getMaxVelocity() const override {
        return 0.0;
    }

    /**
     * @brief Get the maximum recorded acceleration.
     * 
     * @return Maximum recorded acceleration.
     */
    float getMaxAcceleration() const override {
        return 0.0;
    }

private:
    IMUSensor& imu_; ///< Reference to the IMU sensor
};

#endif // IMU_PROCESSOR_HPP
