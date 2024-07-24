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

float SensorFusion::getGroundAltitude() const {
    float groundAltitude = 0;
    for (const auto& sensor : sensors) {
        groundAltitude += sensor->getGroundAltitude();
    }
    return groundAltitude / sensors.size(); // Simple averaging for now
}

float SensorFusion::getMaxVelocity() const {
    float maxVelocity = 0;
    for (const auto& sensor : sensors) {
        maxVelocity += sensor->getMaxVelocity();
    }
    return maxVelocity / sensors.size(); // Simple averaging for now
}

float SensorFusion::getMaxAltitude() const {
    float maxAltitude = 0;
    for (const auto& sensor : sensors) {
        maxAltitude += sensor->getMaxAltitude();
    }
    return maxAltitude / sensors.size(); // Simple averaging for now
}

float SensorFusion::getMaxAcceleration() const {
    float maxAcceleration = 0;
    for (const auto& sensor : sensors) {
        maxAcceleration += sensor->getMaxAcceleration();
    }
    return maxAcceleration / sensors.size(); // Simple averaging for now
}
