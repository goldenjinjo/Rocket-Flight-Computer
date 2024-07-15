#include "DataProcessor.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>

DataProcessor::DataProcessor(size_t historySize, float outlierThreshold)
    : currentIndex(0), currentSize(0), historySize(historySize), outlierThreshold(outlierThreshold) {
    dataHistory.resize(historySize, 0.0);
}

void DataProcessor::update(float value) {
    dataHistory[currentIndex] = value;
    currentIndex = (currentIndex + 1) % historySize;
    if (currentSize < historySize) {
        ++currentSize;
    }
}

float DataProcessor::getIntegratedValue() const {
    return calculateIntegratedValue();
}

float DataProcessor::getDifferentiatedValue() const {
    return calculateDifferentiatedValue();
}

float DataProcessor::getSmoothedValue() const {
    return calculateSmoothedValue();
}

bool DataProcessor::isOutlier(float value) const {
    return detectOutlier(value);
}

float DataProcessor::calculateSmoothedValue() const {
    if (currentSize == 0) {
        return 0.0;
    }

    float sum = std::accumulate(dataHistory.begin(), dataHistory.begin() + currentSize, 0.0);
    return sum / static_cast<float>(currentSize);
}

float DataProcessor::calculateIntegratedValue() const {
    if (currentSize == 0) {
        return 0.0;
    }

    float sum = std::accumulate(dataHistory.begin(), dataHistory.begin() + currentSize, 0.0);
    return sum; // Assume unit time step
}

float DataProcessor::calculateDifferentiatedValue() const {
    if (currentSize < 2) {
        return 0.0;
    }

    size_t oldestIndex = (currentIndex + 1) % historySize;
    size_t newestIndex = (currentIndex + historySize - 1) % historySize;

    float deltaValue = dataHistory[newestIndex] - dataHistory[oldestIndex];
    float deltaTime = static_cast<float>(currentSize - 1); // Assume 1 unit time step
    return deltaValue / deltaTime;
}

bool DataProcessor::detectOutlier(float value) const {
    if (currentSize == 0) {
        return false;
    }

    float mean = calculateSmoothedValue();
    float diff = std::abs(value - mean);
    return diff > outlierThreshold;
}
