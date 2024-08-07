#ifndef barometricProcessor_HPP
#define barometricProcessor_HPP

#include "dataProcessor.hpp"
#include "sensorProcessor.hpp"
#include "pressureSensor.hpp"
#include "configKeys.hpp"

/**
 * @class BarometricProcessor
 * @brief Class for processing barometric pressure data and estimating 
 *        altitude and vertical velocity.
 */
class BarometricProcessor : public DataProcessor, public SensorProcessor {
public:
    /**
     * @brief Constructor with history size and outlier threshold.
     * 
     * @param pressureSensor Reference to the pressure sensor.
     * @param historySize The size of the history buffer for smoothing 
     *        and differentiation.
     * @param outlierThreshold The threshold for detecting outliers.
     */
    BarometricProcessor(size_t historySize, 
                        float outlierThreshold = 10.0);

    /**
     * @brief Update the pressure sensor data and process it.
     */
    void update() override;

    /**
     * @brief Get the raw data from the pressure sensor, wrapper of getAllData()
     * 
     * @return raw pressure (Pa) and temperature (C)
     */
    float* getRawData() const override;

    /**
     * @brief returns number of unique sensor values, wrapper of getNumValues()
     * 
     * @return size_t num of unique sensors
     */
    size_t getNumSensorValues() const override;

    /**
     * @brief Get the sensor names.
     * 
     * @return Sensor names as a string.
     */
    std::string getSensorNames() const override;

    /**
     * @brief Get the estimated altitude.
     * 
     * @return Estimated altitude.
     */
    float getAltitude() const override;

    /**
     * @brief Get the estimated vertical velocity.
     * 
     * @return Estimated vertical velocity.
     */
    float getVerticalVelocity() const override;

    /**
     * @brief Get the maximum recorded altitude.
     * 
     * @return Maximum recorded altitude.
     */
    float getMaxAltitude() const override;

    /**
     * @brief Get the maximum recorded vertical velocity.
     * 
     * @return Maximum recorded vertical velocity.
     */
    float getMaxVelocity() const override;

    /**
     * @brief Get the ground altitude recorded at end of stabilization period.
     * 
     * @return Ground altitude.
     */
    float getGroundAltitude() const override;

    /**
     * @brief Override of virtual method. Barometer not able to calculate 
     *        acceleration.
     * 
     * @return 0
     */
    float getAcceleration() const override {
        return 0;
    }

    /**
     * @brief Override of virtual method. Barometer not able to calculate 
     *        maximum acceleration.
     * 
     * @return 0
     */
    float getMaxAcceleration() const override {
        return 0;
    }

private:
    PressureSensor pressureSensor_; ///< the pressure sensor
    float maxAltitude_; ///< Maximum recorded altitude
    float maxVelocity_; ///< Maximum recorded vertical velocity
    float groundAltitude_; ///< Ground altitude

    /**
     * @brief Helper function to calculate altitude from pressure.
     * 
     * @param pressure The pressure value.
     * @return Calculated altitude.
     */
    float calculateAltitude(float pressure) const;

    /**
     * @brief Update the maximum recorded altitude.
     */
    void updateMaxAltitude();

    /**
     * @brief Update the maximum recorded vertical velocity.
     */
    void updateMaxVelocity();

    /**
     * @brief Update the ground altitude.
     */
    void updateGroundAltitude();
};

#endif // barometricProcessor_HPP
