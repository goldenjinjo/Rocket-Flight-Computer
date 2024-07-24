#include "sensorFusion.hpp"

void SensorFusion::addSensor(const std::shared_ptr<SensorProcessor>& sensor) {
    sensors.push_back(sensor);
}

void SensorFusion::updateSensors() {
    for (auto& sensor : sensors) {
        sensor->update();
    }
}

float SensorFusion::getFusedAltitude() const {
    float totalAltitude = 0.0;
    for (const auto& sensor : sensors) {
        totalAltitude += sensor->getAltitude();
    }
    return totalAltitude / sensors.size(); // Simple averaging for now
}

float SensorFusion::getFusedVerticalVelocity() const {
    float totalVelocity = 0.0;
    for (const auto& sensor : sensors) {
        totalVelocity += sensor->getVerticalVelocity();
    }
    return totalVelocity / sensors.size(); // Simple averaging for now
}

float SensorFusion::getFusedAcceleration() const {
    float totalAcceleration = 0.0;
    for (const auto& sensor : sensors) {
        totalAcceleration += sensor->getAcceleration();
    }
    return totalAcceleration / sensors.size(); // Simple averaging for now
}
