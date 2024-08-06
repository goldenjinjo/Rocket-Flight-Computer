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
    DataLogger& logger_;
    size_t numSensorValues_;
    const char* dataHeaderString_;


    void calculateNumSensorValues();

    void updateSensorInformation();

public:
   
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
     * @brief Updates all sensors in the fusion system.
     */
    void updateSensors();

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
     * @brief Gets the fused ground altitude data from all sensors that can compute it (i.e barometers, gps)
     * @return Fused ground altitude value.
     */
    float getGroundAltitude() const;

    /**
     * @brief Get the maximum recorded altitude.
     * 
     * @return Maximum recorded altitude.
     */
    float getMaxAltitude() const;

    /**
     * @brief Get the maximum recorded vertical velocity.
     * 
     * @return Maximum recorded vertical velocity.
     */
    float getMaxVelocity() const;

    /**
     * @brief Get the maximum recorded vertical acceleration.
     * 
     * @return Maximum recorded vertical acceleration.
     */
    float getMaxAcceleration() const;


    float* getAllRawData() const;

    void logSensorData();

    void writeDataHeaderString();


    size_t getNumSensorValues() const;
};

#endif // SENSOR_FUSION_HPP
