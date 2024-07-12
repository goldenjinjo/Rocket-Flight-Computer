#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

#include "Wire.h"
#include "SparkFunMPL3115A2.h"
#include "constants.hpp"

class pressureSensor {
private:
    // ------------------------- MEMBERS ------------------------- //
    MPL3115A2 baro; // MPL3115A2 object for pressure sensing

public:
    // ------------------------- METHODS ------------------------- //
    /**
     * @brief  Constructor for pressureSensor class.
     * @param  rate Oversample rate for sensor readings.
     */
    pressureSensor(byte rate);
    
    /**
     * @brief  Initialize the pressure sensor.
     * @param  rate Oversample rate for sensor readings.
     */
    void init(byte rate);
    
    /**
     * @brief  Get altitude from the pressure sensor.
     * @return Altitude in meters.
     */
    float getAltitude();
    
    /**
     * @brief  Get pressure from the pressure sensor.
     * @return Pressure in Pascals (Pa).
     */
    float getPressure();
    
    /**
     * @brief  Get temperature from the pressure sensor.
     * @return Temperature in degrees Celsius (°C).
     */
    float getTemperature();

    // ------------------------- MEMBERS ------------------------- //
    float pressure; // Current pressure reading
    float temp; // Current temperature reading
};

#endif // PRESSURE_SENSOR_H
