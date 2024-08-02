#include "IMUProcessor.hpp"

IMUProcessor::IMUProcessor(IMUSensor& imu, size_t historySize, float outlierThreshold) 
    : imu_(imu) {}

void IMUProcessor::update() {
    imu_.update();
}
