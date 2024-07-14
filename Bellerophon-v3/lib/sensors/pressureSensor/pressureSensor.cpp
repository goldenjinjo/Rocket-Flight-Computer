#include "pressureSensor.hpp"

// Constructor with oversample rate as argument
PressureSensor::PressureSensor(byte rate) : oversampleRate_(rate) {
    // Initialize the sensor with the provided oversample rate
    initialize();
}

// Initialize the sensor
void PressureSensor::initialize() {
    baro_.begin();
    baro_.setModeActive();
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
    return 44330.77 * (1 - pow(pressure_ / (REFERENCE_PRESSURE), 0.1902632));
}

