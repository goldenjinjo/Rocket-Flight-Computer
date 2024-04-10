#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

#include "Wire.h"
#include "SparkFunMPL3115A2.h"
#include "constants.hpp"

class pressureSensor {
private:
    MPL3115A2 baro; // Declaring baro inside pressureSensor class



public:
    pressureSensor(byte rate); // Constructor with oversample rate as argument
    void init(byte rate); // Initialize the sensor
    float getAltitude(); // Get altitude
    float getPressure();
    float getTemperature();

    float pressure;
    float temp;
};

#endif // PRESSURESENSOR_H
