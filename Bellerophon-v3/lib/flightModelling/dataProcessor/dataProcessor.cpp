#include "dataProcessor.hpp"

DataProcessor::DataProcessor(size_t historySize, float outlierThreshold)
    : currentIndex(0), currentSize(0), historySize(historySize), outlierThreshold(outlierThreshold),
      stabilizationPhase(true), stabilizationCount(0), stabilizationLimit(historySize) {
    values = new float[historySize]();
    timestamps = new unsigned long[historySize]();
}

DataProcessor::~DataProcessor() {
    delete[] values;
    delete[] timestamps;
}

void DataProcessor::updateBuffer(float value) {
    if (stabilizationPhase) {
        stabilize(value);
    } else {
        values[currentIndex] = value;
        timestamps[currentIndex] = Timer::currentTime();
        currentIndex = (currentIndex + 1) % historySize;
        if (currentSize < historySize) {
            ++currentSize;
        }
    }
}

float DataProcessor::getIntegratedValue() const {
    if (stabilizationPhase) {
        return 0.0;
    }
    return calculateIntegratedValue();
}

float DataProcessor::getDifferentiatedValue() const {
    if (stabilizationPhase) {
        return 0.0;
    }
    return calculateDifferentiatedValue();
}

float DataProcessor::getSmoothedValue() const {
    if (stabilizationPhase) {
        return 0.0;
    }
    return calculateSmoothedValue();
}

bool DataProcessor::isOutlier(float value) const {
    return detectOutlier(value);
}

bool DataProcessor::isStabilized() const {
    return !stabilizationPhase;
}

void DataProcessor::stabilize(float value) {
    // During the stabilization phase, we update the buffer without timestamps and check for stabilization
    values[currentIndex] = value;
    currentIndex = (currentIndex + 1) % historySize;
    if (currentSize < historySize) {
        ++currentSize;
    }
    
    if (++stabilizationCount >= stabilizationLimit) {
        stabilizationPhase = false; // Exit stabilization phase
    }
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
