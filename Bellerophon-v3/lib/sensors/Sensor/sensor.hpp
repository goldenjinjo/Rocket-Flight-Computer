#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <array>
#include <vector>

/**
 * @class Sensor
 * @brief Abstract base class for all sensor types.
 *
 * This class provides a common interface for initializing,
 * updating, and retrieving data from various sensors.
 * Derived sensor classes must implement the pure virtual functions
 * to provide specific functionality.
 */
class Sensor {
public:
    /**
     * @brief Virtual destructor for proper cleanup.
     *
     * Ensures derived class destructors are called correctly.
     */
    virtual ~Sensor() = default;

    /**
     * @brief Initialize the sensor.
     *
     * This method should contain code to initialize the sensor hardware.
     * It must be implemented by the derived sensor class.
     */
    virtual void initialize() = 0;

    /**
     * @brief Update the sensor data.
     *
     * This method should contain code to read and update the sensor data.
     * It must be implemented by the derived sensor class.
     */
    virtual void update() = 0;

    /**
     * @brief Get the current sensor data.
     *
     * This method should return the current sensor data as a float.
     * It must be implemented by the derived sensor class.
     * 
     * @return Current sensor data.
     */
    virtual float getData() = 0;

    /**
     * @brief Get all unique data values available by the sensor
     *
     * This method should return an array of sensor data values.
     * It must be implemented by the derived sensor class.
     * 
     * @return Pointer to an array of current sensor data values.
     */
    virtual float* getAllData() = 0;

    virtual size_t getNumValues() = 0;

     /**
     * @brief Get all unique name values from a sensor
     *
     * This method should return a string with sensor value names separated
     * by a column. e.g "value1,value2".
     * 
     * @return string of sensor names
     */
    virtual std::string getNames() = 0;

};

#endif // SENSOR_HPP

