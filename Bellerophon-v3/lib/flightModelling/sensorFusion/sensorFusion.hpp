#ifndef SENSOR_FUSION_HPP
#define SENSOR_FUSION_HPP

#include <vector>
#include <memory>
#include "sensorProcessor.hpp"

/**
 * @class SensorFusion
 * @brief Combines data from multiple sensor processors to produce fused estimates of altitude, velocity, and acceleration.
 */
class SensorFusion {
private:
    std::vector<std::shared_ptr<SensorProcessor>> sensors; ///< Vector of shared pointers to sensor processors

public:
    /**
     * @brief Default constructor for SensorFusion.
     */
    SensorFusion() = default;

    /**
     * @brief Adds a sensor processor to the fusion system.
     * @param sensor Shared pointer to the sensor processor to be added.
     */
    void addSensor(const std::shared_ptr<SensorProcessor>& sensor);

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
};

#endif // SENSOR_FUSION_HPP
