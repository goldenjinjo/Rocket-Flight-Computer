#include "barometricProcessor.hpp"
#include "timer.hpp"

BarometricProcessor::BarometricProcessor(PressureSensor& pressureSensor, size_t historySize, float outlierThreshold)
    : DataProcessor(historySize, outlierThreshold), pressureSensor_(pressureSensor) {}

void BarometricProcessor::update() {
    pressureSensor_.update();
    float pressure = pressureSensor_.getData();
    updateBuffer(pressure); // Use the base class method to update buffer and timestamp
}

float BarometricProcessor::getAltitude() const {
    float smoothedPressure = getSmoothedValue();
    return 44330.77 * (1 - pow(smoothedPressure / REFERENCE_PRESSURE, 0.1902632));
}

float BarometricProcessor::getVerticalVelocity() const {
    return getDifferentiatedValue();
}
