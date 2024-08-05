#include "IMUProcessor.hpp"

IMUProcessor::IMUProcessor(IMUSensor& imu, size_t historySize, float outlierThreshold) 
    : imu_(imu) {}

void IMUProcessor::update() {
    imu_.update();
}


float* IMUProcessor::getRawData() const {
    return imu_.getAllData();
}

size_t IMUProcessor::getNumSensorValues() const {
    return imu_.getNumValues();
}

std::string IMUProcessor::getSensorNames() const {
    return imu_.getNames();
}