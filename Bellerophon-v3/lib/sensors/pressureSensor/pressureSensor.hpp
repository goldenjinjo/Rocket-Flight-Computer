#ifndef PRESSURE_SENSOR_HPP
#define PRESSURE_SENSOR_HPP

#include "SparkFunMPL3115A2.h"
#include "sensor.hpp"
#include "constants.hpp"
#include "configKeys.hpp"

/**
 * @class PressureSensor
 * @brief Derived class for handling pressure sensor operations.
 *
 * The PressureSensor class inherits from the Sensor base class and 
 * implements the specific functionalities for a pressure sensor.
 * It interfaces with the MPL3115A2 pressure sensor to retrieve 
 * pressure and temperature data.
 */
class PressureSensor : public Sensor {
public:
    /**
     * @brief Constructor with oversample rate as argument.
     * 
     * Initializes the PressureSensor object with the specified 
     * oversample rate for the pressure sensor.
     *
     * @param rate The oversample rate for the pressure sensor.
     */
    PressureSensor(byte rate);

    /**
     * @brief Update the pressure sensor data.
     *
     * This method reads the latest data from the pressure sensor and 
     * updates the internal state with the new pressure and temperature values.
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
     * @brief Get all data values from the sensor.
     *
     * This method returns an array containing the current pressure 
     * and temperature values from the sensor.
     * 
     * @return Pointer to an array of current sensor data values.
     */
    float* getAllData() override {
        static float allData[2];
        allData[0] = getData();
        allData[1] = getTemperature();
        return allData;
    }

    /**
     * @brief Get the number of sensor values.
     *
     * This method returns the number of unique data values the 
     * pressure sensor provides, which is 2 (pressure and temperature).
     * 
     * @return Number of sensor values.
     */
    size_t getNumValues() override {
        return 2;
    }

    /**
     * @brief Get the names of all data values from the sensor.
     *
     * This method returns a string with the names of the sensor values 
     * separated by a comma, specifically "pressure,temperature".
     * 
     * @return String of sensor names.
     */
    std::string getNames() override {
        return "pressure,temperature";
    }

    /**
     * @brief Get the current temperature from the sensor.
     *
     * This method reads and returns the current temperature.
     * 
     * @return Current temperature.
     */
    float getTemperature();

private:
    float pressure_;       ///< Current pressure value
    float temp_;           ///< Current temperature value
    byte oversampleRate_;  ///< Oversample rate for the sensor
    MPL3115A2 baro_;       ///< Instance of the pressure sensor library

    /**
     * @brief Initialize the pressure sensor.
     *
     * This method initializes the pressure sensor hardware, setting 
     * up any necessary configurations.
     */
    void initialize() override;
};

#endif // PRESSURESENSOR_HPP


