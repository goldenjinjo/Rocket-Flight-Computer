#include "dataProcessor.hpp"

DataProcessor::DataProcessor(size_t historySize, float outlierThreshold)
    : currentIndex(0), currentSize(0), historySize(historySize), outlierThreshold(outlierThreshold),
      stabilizationPhase(true), stabilizationCount(0), stabilizationLimit(historySize), outlierCount(0) {
    values = new float[historySize]();
    timestamps = new unsigned long[historySize]();
}

DataProcessor::~DataProcessor() {
    delete[] values;
    delete[] timestamps;
}

void DataProcessor::updateBuffer(float value) {
    if (!isStabilized()) {
        stabilize(value);
        return;
    }
    
    if (isOutlier(value)) {
        outlierCount++;
        return; // Skip adding this value if it's an outlier
    }
 
    values[currentIndex] = value;
    timestamps[currentIndex] = Timer::currentTime();
    currentIndex = (currentIndex + 1) % historySize;
    
    if (currentSize < historySize) {
        ++currentSize;
    }

    updateSlidingWindow(value);
    
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

size_t DataProcessor::getOutlierCount() const {
    return outlierCount;
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
    // If the sliding window has fewer than 2 elements, there isn't enough data to compare,
    // so we cannot determine if the value is an outlier. In this case, return false.
    if (slidingWindow.size() < 2) {
        return false;
    }

    // Get the last value in the sliding window, which is the most recent value added.
    float lastValue = slidingWindow.back();

    // Calculate the rate of change between the new value and the last value in the sliding window.
    // The rate of change is the absolute difference between these two values.
    float rateOfChange = std::abs(value - lastValue);

    // Determine if the rate of change exceeds the outlier threshold. If it does, the value is
    // considered an outlier, so return true. Otherwise, return false.
    return rateOfChange > outlierThreshold;
}

void DataProcessor::updateSlidingWindow(float value) {
    // If the sliding window has reached its maximum size (historySize), remove the oldest value
    // from the front to make room for the new value.
    if (slidingWindow.size() >= historySize) {
        slidingWindow.pop_front();
    }

    // Add the new value to the back of the sliding window.
    slidingWindow.push_back(value);

    // If the rate of change window has reached its maximum size (historySize), remove the oldest
    // rate of change value from the front to maintain a consistent window size.
    if (rateOfChangeWindow.size() >= historySize) {
        rateOfChangeWindow.pop_front();
    }

    // Calculate the rate of change if there is more than one value in the sliding window.
    if (slidingWindow.size() > 1) {
        // The rate of change is the absolute difference between the most recent value and the
        // second most recent value in the sliding window.
        rateOfChangeWindow.push_back(std::abs(slidingWindow.back() - slidingWindow[slidingWindow.size() - 2]));
    }
}