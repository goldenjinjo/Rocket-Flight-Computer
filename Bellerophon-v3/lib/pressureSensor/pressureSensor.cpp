#include "pressureSensor.hpp"

// Constructor with oversample rate as argument
pressureSensor::pressureSensor(byte rate) {
    // Initialize the sensor with the provided oversample rate
    init(rate);
}

// Initialize the sensor
void pressureSensor::init(byte rate) {
    baro.begin();
    baro.setOversampleRate(rate); // Set oversample rate
    baro.enableEventFlags(); 
    baro.setModeBarometer(); 
}

// Read pressure from the sensor
float pressureSensor::getPressure() {
    pressure = baro.readPressure();
    return pressure;
}

// Read temperature from the sensor
float pressureSensor::getTemperature() {
    temp = baro.readTemp();
    return temp;
}

// Calculate and return altitude based on pressure and temperature
float pressureSensor::getAltitude() {
    pressure = getPressure();
    temp = getTemperature();
    return pow(P_0 / pressure, EXP) * (temp + C_TO_K) / L_B;
}
