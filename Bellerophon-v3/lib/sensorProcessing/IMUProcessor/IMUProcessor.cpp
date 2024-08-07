#include "IMUProcessor.hpp"

IMUProcessor::IMUProcessor(size_t historySize, float outlierThreshold) 
    : imu_(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000) {

        imu_.setPollRate(10); 
    }

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