#include "barometricProcessor.hpp"

BarometricProcessor::BarometricProcessor(PressureSensor& pressureSensor, size_t historySize, float outlierThreshold)
    : DataProcessor(historySize, outlierThreshold), pressureSensor_(pressureSensor), maxAltitude_(0), maxVelocity_(0),
    groundAltitude_(-999) {}

void BarometricProcessor::update() {
    pressureSensor_.update();
    float pressure = pressureSensor_.getData();
    float altitude = calculateAltitude(pressure);
    updateBuffer(altitude); // Use the base class method to update buffer and timestamp

    updateMaxAltitude();
    updateMaxVelocity();
    updateGroundAltitude();
}

void BarometricProcessor::updateGroundAltitude() {
    if(isStabilized() && groundAltitude_ == -999) {
        groundAltitude_ = getAltitude();
    }
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
    return 44330.77 * (1 - pow(pressure / REFERENCE_PRESSURE, 0.1902632));
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
