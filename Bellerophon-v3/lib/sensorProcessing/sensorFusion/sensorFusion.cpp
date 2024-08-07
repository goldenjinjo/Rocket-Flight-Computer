#include "sensorFusion.hpp"

/// TODO: Add general initalisation implementation for each sensor, 
// prevent data logging or processing until initilisation complete


SensorFusion::SensorFusion(DataLogger& logger) : logger_(logger), numFusedDataPoints_(3), numSensorValues_(0),
dataHeaderString_(""), fusedAltitude_(0), fusedVerticalVelocity_(0), fusedAcceleration_(0) {}

void SensorFusion::updateSensorInformation() {
    calculateNumSensorValues();
    writeDataHeaderString();
}

bool SensorFusion::addSensor(const std::shared_ptr<SensorProcessor>& sensor) {
    // Check if the argument is a valid SensorProcessor
    if (sensor && dynamic_cast<SensorProcessor*>(sensor.get())) {
        sensors.push_back(sensor);
        updateSensorInformation();
        return true;
    }
    return false;
}

bool SensorFusion::removeSensor(const std::shared_ptr<SensorProcessor>& sensor) {
    auto it = std::find(sensors.begin(), sensors.end(), sensor);
    // remove sensor and return true if found
    if (it != sensors.end()) {
        sensors.erase(it);
        updateSensorInformation();
        return true;
    }
    // return false if sensor not found
    return false;
}

void SensorFusion::update() {
    updateSensors();
    updateFusedData();
}

void SensorFusion::updateSensors() {
    for (auto& sensor : sensors) {
        sensor->update();
    }
}

void SensorFusion::updateFusedData() {
    updateFusedAltitude();
    updateFusedVerticalVelocity();
    updateFusedAcceleration();
}

float SensorFusion::getFusedAltitude() const {
    return fusedAltitude_;
}

float SensorFusion::getFusedVerticalVelocity() const {
    return fusedVerticalVelocity_;
}

float SensorFusion::getFusedAcceleration() const {
    return fusedAcceleration_;
}

size_t SensorFusion::getNumSensorValues() const {
    return numSensorValues_;
}

void SensorFusion::logSensorData() {
    
    // Write title for logging file
    logger_.addDataFileHeading(dataHeaderString_);

    size_t combinedDataLength = numSensorValues_+ numFusedDataPoints_;

    float combinedData[combinedDataLength];

    combinedData[0] = getFusedAltitude();
    combinedData[1] = getFusedVerticalVelocity();
    combinedData[2] = getFusedAcceleration();

    // Copy data from each sensor into the combined array
    size_t offset = numFusedDataPoints_;
    for (const auto& sensor : sensors) {
        float* sensorData = sensor->getRawData();
        size_t sensorDataSize = sensor->getNumSensorValues();
        std::memcpy(combinedData + offset, sensorData, sensorDataSize * sizeof(float));
        offset += sensorDataSize;
    }

    // DEBUG
    for (size_t i = 0; i < combinedDataLength; ++i) {
        Serial.println(combinedData[i]);
    }

    // MORE DEBUG
    Serial.print("Max Altitude: ");
    Serial.println(getMaxAltitude());
    Serial.print("Max Velocity: ");
    Serial.println(getMaxVelocity());
    Serial.print("Ground Altitude: ");
    Serial.println(getGroundAltitude());

    // Log the combined array
    logger_.logData(combinedData, numSensorValues_);
}


void SensorFusion::updateFusedAltitude() {
    float totalAltitude = 0.0;
    for (const auto& sensor : sensors) {
        totalAltitude += sensor->getAltitude();
    }
    // Simple averaging for now
    fusedAltitude_ = totalAltitude / sensors.size(); 
}

void SensorFusion::updateFusedVerticalVelocity() {
    float totalVelocity = 0.0;
    for (const auto& sensor : sensors) {
        totalVelocity += sensor->getVerticalVelocity();
    }
    // Simple averaging for now
    fusedVerticalVelocity_ = totalVelocity / sensors.size(); 
}

void SensorFusion::updateFusedAcceleration() {
    float totalAcceleration = 0.0;
    for (const auto& sensor : sensors) {
        totalAcceleration += sensor->getAcceleration();
    }
    // Simple averaging for now
    fusedAcceleration_ = totalAcceleration / sensors.size(); 
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

void SensorFusion::calculateNumSensorValues() {
    size_t totalSize = 0;
    for (const auto& sensor : sensors) {
        totalSize += sensor->getNumSensorValues();
    }
    // assign to private member variable
    numSensorValues_ = totalSize;
}

void SensorFusion::writeDataHeaderString() {
    std::string header = "time";
    header+= "," + getFusedDataString();
    for (const auto& sensor : sensors) {
        std::string names = sensor->getSensorNames();
        if (!names.empty()) {
            header += "," + names;
        }
    }
    dataHeaderString_ = header.c_str();
}

std::string SensorFusion::getFusedDataString() {
    return "altitude,velocity(Z),acceleration(Z)";
}

