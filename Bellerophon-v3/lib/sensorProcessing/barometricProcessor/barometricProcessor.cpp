#include "barometricProcessor.hpp"

BarometricProcessor::BarometricProcessor(size_t historySize, float outlierThreshold)
    : DataProcessor(historySize, outlierThreshold), pressureSensor_(0), maxAltitude_(0), maxVelocity_(0),
    groundAltitude_(0) {}

void BarometricProcessor::update() {
    pressureSensor_.update();
    float pressure = pressureSensor_.getData();

    
    float altitude = calculateAltitude(pressure);
    updateBuffer(altitude); // Use the base class method to update buffer and timestamp

    updateGroundAltitude();
    updateMaxAltitude();
    updateMaxVelocity();
}


float* BarometricProcessor::getRawData() const {
    return pressureSensor_.getAllData();
}

size_t BarometricProcessor::getNumSensorValues() const {
    return pressureSensor_.getNumValues();
}

std::string BarometricProcessor::getSensorNames() const {
    return pressureSensor_.getNames();
}

void BarometricProcessor::updateGroundAltitude() {
    // update ground altitude only if data is stable
    if(!isStabilized()){
        return;
    }
    // update ground altitude only once
    if(groundAltitude_ != 0){
        return;
    }

    groundAltitude_ = getAltitude();
    // clear buffer to reset stabilization period, now with ground calibration
    clearBuffer(); 
}

float BarometricProcessor::getAltitude() const {
    return getSmoothedValue();
}

float BarometricProcessor::getVerticalVelocity() const {
    return calculateDifferentiatedValue();
}

float BarometricProcessor::getGroundAltitude() const {
    return groundAltitude_;
}

float BarometricProcessor::getMaxAltitude() const {
    return maxAltitude_;
}

float BarometricProcessor::getMaxVelocity() const {
    return maxVelocity_;
}

float BarometricProcessor::calculateAltitude(float pressure) const {
    float seaLevelAltitude = 44330.77 * (1 - pow(pressure / REFERENCE_PRESSURE, 0.1902632));
    return seaLevelAltitude - groundAltitude_;
}

void BarometricProcessor::updateMaxAltitude() {
    
    float altitude = getAltitude();
    if (altitude > maxAltitude_) {
        maxAltitude_ = altitude;
    }
}

void BarometricProcessor::updateMaxVelocity() {
    float velocity = getVerticalVelocity();
    if (velocity > maxVelocity_) {
        maxVelocity_ = velocity;
    }
}
