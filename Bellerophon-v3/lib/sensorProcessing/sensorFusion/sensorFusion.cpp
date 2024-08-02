#include "sensorFusion.hpp"

bool SensorFusion::addSensor(const std::shared_ptr<SensorProcessor>& sensor) {
    // Check if the argument is a valid SensorProcessor
    if (sensor && dynamic_cast<SensorProcessor*>(sensor.get())) {
        sensors.push_back(sensor);
        return true;
    }
    return false;
}

bool SensorFusion::removeSensor(const std::shared_ptr<SensorProcessor>& sensor) {
    auto it = std::find(sensors.begin(), sensors.end(), sensor);
    // remove sensor and return true if found
    if (it != sensors.end()) {
        sensors.erase(it);
        return true;
    }
    // return false if sensor not found
    return false;
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


float* SensorFusion::getAllRawData() const {
    // First determine the total size required
    size_t totalSize = 0;
    for (const auto& sensor : sensors) {
        totalSize += sensor->getNumSensorValues();
    }

    // Allocate a sufficiently large array
    // Todo reassess dynamic allocation
    float* combinedData = new float[totalSize];

    // Copy data from each sensor into the combined array
    size_t offset = 0;
    for (const auto& sensor : sensors) {
        float* sensorData = sensor->getRawData();
        size_t sensorDataSize = sensor->getNumSensorValues();
        std::memcpy(combinedData + offset, sensorData, sensorDataSize * sizeof(float));
        offset += sensorDataSize;
    }

    return combinedData;
}