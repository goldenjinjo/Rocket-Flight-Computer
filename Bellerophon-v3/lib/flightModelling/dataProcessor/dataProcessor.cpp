#include "dataProcessor.hpp"

DataProcessor::DataProcessor(size_t historySize, float outlierThreshold)
    : currentIndex(0), currentSize(0), historySize(historySize), outlierThreshold(outlierThreshold) {
    values = new float[historySize]();
    timestamps = new unsigned long[historySize]();
}

DataProcessor::~DataProcessor() {
    delete[] values;
    delete[] timestamps;
}

void DataProcessor::updateBuffer(float value) {
    values[currentIndex] = value;
    timestamps[currentIndex] = Timer::currentTime();
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

    float sum = 0.0;
    for (size_t i = 0; i < currentSize; ++i) {
        sum += values[i];
    }
    return sum / static_cast<float>(currentSize);
}

float DataProcessor::calculateIntegratedValue() const {
    if (currentSize < 2) {
        return 0.0;
    }

    float sum = 0.0;
    for (size_t i = 1; i < currentSize; ++i) {
        size_t currentIndex = (this->currentIndex + historySize - i) % historySize;
        size_t previousIndex = (currentIndex + historySize - 1) % historySize;

        float deltaValue = values[currentIndex] - values[previousIndex];
        float deltaTime = (timestamps[currentIndex] - timestamps[previousIndex]) / 1000.0; // Convert to seconds

        sum += deltaValue * deltaTime;
    }

    return sum;
}

float DataProcessor::calculateDifferentiatedValue() const {
    if (currentSize < 2) {
        return 0.0;
    }

    float sumDeltaValue = 0.0;
    float sumDeltaTime = 0.0;
    for (size_t i = 1; i < currentSize; ++i) {
        size_t currentIndex = (this->currentIndex + historySize - i) % historySize;
        size_t previousIndex = (currentIndex + historySize - 1) % historySize;

        float deltaValue = values[currentIndex] - values[previousIndex];
        float deltaTime = (timestamps[currentIndex] - timestamps[previousIndex]) / 1000.0; // Convert to seconds

        sumDeltaValue += deltaValue;
        sumDeltaTime += deltaTime;
    }

    // Avoid division by zero
    if (sumDeltaTime == 0.0) {
        return 0.0;
    }

    return sumDeltaValue / sumDeltaTime;
}

bool DataProcessor::detectOutlier(float value) const {
    if (currentSize == 0) {
        return false;
    }

    float mean = calculateSmoothedValue();
    float diff = std::abs(value - mean);
    return diff > outlierThreshold;
}
