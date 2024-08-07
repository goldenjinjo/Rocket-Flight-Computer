// sensorProcessor.hpp
#ifndef SENSOR_PROCESSOR_HPP
#define SENSOR_PROCESSOR_HPP

#include <cstddef>
#include <string>

/**
 * @class SensorProcessor
 * @brief Abstract base class for sensor data processing.
 *
 * This class provides a common interface for updating sensor data,
 * retrieving various sensor measurements, and obtaining sensor metadata.
 */
class SensorProcessor {
public:
    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures derived class destructors are called correctly.
     */
    virtual ~SensorProcessor() = default;

    /**
     * @brief Update the sensor data.
     *
     * This method should contain code to read and update the sensor data.
     */
    virtual void update() = 0;

    /**
     * @brief Get the current altitude from the sensor.
     *
     * This method should return the current altitude measurement from the sensor.
     * 
     * @return Current altitude.
     */
    virtual float getAltitude() const = 0;

    /**
     * @brief Get the current vertical velocity from the sensor.
     *
     * This method should return the current vertical velocity measurement from the sensor.
     * 
     * @return Current vertical velocity.
     */
    virtual float getVerticalVelocity() const = 0;

    /**
     * @brief Get the current acceleration from the sensor.
     *
     * This method should return the current acceleration measurement from the sensor.
     * 
     * @return Current acceleration.
     */
    virtual float getAcceleration() const = 0;

    /**
     * @brief Get the ground altitude recorded by the sensor.
     *
     * This method should return the ground altitude measurement from the sensor.
     * 
     * @return Ground altitude.
     */
    virtual float getGroundAltitude() const = 0;

    /**
     * @brief Get the maximum recorded altitude from the sensor.
     *
     * This method should return the maximum altitude measurement recorded by the sensor.
     * 
     * @return Maximum recorded altitude.
     */
    virtual float getMaxAltitude() const = 0;

    /**
     * @brief Get the maximum recorded vertical velocity from the sensor.
     *
     * This method should return the maximum vertical velocity measurement recorded by the sensor.
     * 
     * @return Maximum recorded vertical velocity.
     */
    virtual float getMaxVelocity() const = 0;

    /**
     * @brief Get the maximum recorded acceleration from the sensor.
     *
     * This method should return the maximum acceleration measurement recorded by the sensor.
     * 
     * @return Maximum recorded acceleration.
     */
    virtual float getMaxAcceleration() const = 0;

    /**
     * @brief Get all raw data values from the sensor.
     *
     * This method should return an array of all current sensor data values.
     * 
     * @return Array of current sensor data values.
     */
    virtual float* getRawData() const = 0;

    /**
     * @brief Get the number of sensor values available from the sensor.
     *
     * This method should return the number of data points provided by the sensor.
     * 
     * @return Number of sensor values.
     */
    virtual size_t getNumSensorValues() const = 0;

    /**
     * @brief Get the names of the sensor values.
     *
     * This method should return a comma-separated string of sensor value names.
     * There should be no spaces between data points. e.g. "a,b,c"
     * 
     * @return Comma-separated string of sensor value names.
     */
    virtual std::string getSensorNames() const = 0;
};

#endif // SENSOR_PROCESSOR_HPP
