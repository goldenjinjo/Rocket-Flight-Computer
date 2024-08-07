#ifndef SENSOR_FUSION_HPP
#define SENSOR_FUSION_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <cstring>
#include "sensorProcessor.hpp"
#include "dataLogger.hpp"

/**
 * @class SensorFusion
 * @brief Combines data from multiple sensor processors to produce fused estimates of altitude, velocity, and acceleration.
 */
class SensorFusion {
private:
    std::vector<std::shared_ptr<SensorProcessor>> sensors; ///< Vector of shared pointers to sensor processors
    DataLogger& logger_; ///< Reference to the DataLogger instance
    size_t numFusedDataPoints_; ///< Number of fused data points (e.g., altitude, velocity, acceleration)
    size_t numSensorValues_; ///< Total number of sensor values
    const char* dataHeaderString_; ///< Header string for the logged data
    float fusedAltitude_; ///< Fused altitude value
    float fusedVerticalVelocity_; ///< Fused vertical velocity value
    float fusedAcceleration_; ///< Fused acceleration value

    /**
     * @brief Calculates the total number of sensor values.
     */
    void calculateNumSensorValues();

    /**
     * @brief Updates the information about sensors 
     * (e.g., total number of values, data header string).
     */
    void updateSensorInformation();

    /**
     * @brief Updates the fused altitude from all sensors.
     */
    void updateFusedAltitude();
    
    /**
     * @brief Updates the fused vertical velocity from all sensors.
     */
    void updateFusedVerticalVelocity();

    /**
     * @brief Updates the fused acceleration from all sensors.
     */
    void updateFusedAcceleration();

    /**
     * @brief Updates all fused data values.
     */
    void updateFusedData();

    /**
     * @brief Updates all sensors in the fusion system.
     */
    void updateSensors();

    /**
     * @brief Generates a string representing the fused data headers.
     * @return String with the fused data headers.
     */
    std::string getFusedDataString();

public:
    /**
     * @brief Constructor for the SensorFusion class.
     * @param logger Reference to the DataLogger instance.
     */
    SensorFusion(DataLogger& logger);

    /**
     * @brief Adds a sensor processor to the fusion system.
     * @param sensor Shared pointer to the sensor processor to be added.
     * @return true if the sensor was successfully added, false otherwise.
     */
    bool addSensor(const std::shared_ptr<SensorProcessor>& sensor);

    /**
     * @brief Removes a sensor processor from the fusion system.
     * @param sensor Shared pointer to the sensor processor to be removed.
     * @return true if the sensor was successfully removed, false otherwise.
     */
    bool removeSensor(const std::shared_ptr<SensorProcessor>& sensor);

    /**
     * @brief Updates the fusion system by updating all sensors and fused data.
     */
    void update();

    /**
     * @brief Gets the fused altitude from all sensors.
     * @return Fused altitude value.
     */
    float getFusedAltitude() const;

    /**
     * @brief Gets the fused vertical velocity from all sensors.
     * @return Fused vertical velocity value.
     */
    float getFusedVerticalVelocity() const;

    /**
     * @brief Gets the fused acceleration from all sensors.
     * @return Fused acceleration value.
     */
    float getFusedAcceleration() const;

    /**
     * @brief Gets the fused ground altitude data from all sensors that can 
     * compute it (e.g., barometers, GPS).
     * @return Fused ground altitude value.
     */
    float getGroundAltitude() const;

    /**
     * @brief Get the maximum recorded altitude.
     * @return Maximum recorded altitude.
     */
    float getMaxAltitude() const;

    /**
     * @brief Get the maximum recorded vertical velocity.
     * @return Maximum recorded vertical velocity.
     */
    float getMaxVelocity() const;

    /**
     * @brief Get the maximum recorded vertical acceleration.
     * @return Maximum recorded vertical acceleration.
     */
    float getMaxAcceleration() const;

    /**
     * @brief Gets all raw data from all sensors.
     * @return Pointer to the combined raw data array.
     */
    float* getAllRawData() const;

    /**
     * @brief Logs sensor data by combining fused data and individual sensor data.
     */
    void logSensorData();

    /**
     * @brief Writes the data header string for logging.
     */
    void writeDataHeaderString();

    /**
     * @brief Gets the total number of sensor values.
     * @return Total number of sensor values.
     */
    size_t getNumSensorValues() const;
};

#endif // SENSOR_FUSION_HPP
