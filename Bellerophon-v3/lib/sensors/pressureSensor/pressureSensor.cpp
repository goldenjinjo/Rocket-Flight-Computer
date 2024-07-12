#include "pressureSensor.hpp"

// Constructor with oversample rate as argument
PressureSensor::PressureSensor(byte rate) : oversampleRate_(rate) {
    // Initialize the sensor with the provided oversample rate
    initialize();
}

// Initialize the sensor
void PressureSensor::initialize() {
    baro_.begin();
    baro_.setOversampleRate(oversampleRate_); // Set oversample rate
    baro_.enableEventFlags(); 
    baro_.setModeBarometer(); 
}

// Read and update pressure from the sensor
void PressureSensor::update() {
    pressure_ = baro_.readPressure();
    temp_ = baro_.readTemp();
}

// Get pressure data
float PressureSensor::getData() {
    return pressure_;
}

// Read temperature from the sensor
float PressureSensor::getTemperature() {
    return temp_;
}

// Calculate and return altitude based on pressure and temperature
float PressureSensor::getAltitude() {
    pressure_ = getData();
    temp_ = getTemperature();
    return pow(P_0 / pressure_, EXP) * (temp_ + C_TO_K) / L_B;
}

