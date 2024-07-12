#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP

#include "SparkFunMPL3115A2.h"
#include "sensor.hpp"
#include "constants.hpp"

/**
 * @class PressureSensor
 * @brief Derived class for handling pressure sensor operations.
 *
 * This class inherits from the Sensor base class and implements
 * the specific functionalities for a pressure sensor.
 */
class PressureSensor : public Sensor {
public:
    /**
     * @brief Constructor with oversample rate as argument.
     * 
     * @param rate The oversample rate for the pressure sensor.
     */
    PressureSensor(byte rate);

    /**
     * @brief Initialize the pressure sensor.
     *
     * This method initializes the pressure sensor hardware.
     */
    void initialize() override;

    /**
     * @brief Update the pressure sensor data.
     *
     * This method reads and updates the pressure sensor data.
     */
    void update() override;

    /**
     * @brief Get the current pressure sensor data.
     *
     * This method returns the current pressure sensor data.
     * 
     * @return Current pressure sensor data.
     */
    float getData() override;

    /**
     * @brief Get the current temperature from the sensor.
     *
     * This method reads and returns the current temperature.
     * 
     * @return Current temperature.
     */
    float getTemperature();

    /**
     * @brief Calculate and return altitude based on pressure and temperature.
     *
     * This method calculates and returns the altitude.
     * 
     * @return Calculated altitude.
     */
    float getAltitude();

private:

    float pressure_;       ///< Current pressure value
    float temp_;           ///< Current temperature value
    byte oversampleRate_;  ///< Oversample rate for the sensor
    MPL3115A2 baro_; ///< Instance of the pressure sensor library
};

#endif // PRESSURESENSOR_HPP
