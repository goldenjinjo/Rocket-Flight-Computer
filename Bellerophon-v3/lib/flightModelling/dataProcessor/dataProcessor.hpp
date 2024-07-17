#ifndef DATAPROCESSOR_HPP
#define DATAPROCESSOR_HPP

#include <cstddef> // Include this header for size_t
#include "timer.hpp"

class DataProcessor {
public:
    DataProcessor(size_t historySize, float outlierThreshold = 10.0);
    ~DataProcessor(); // Add the destructor
    void update(float value);
    float getIntegratedValue() const;
    float getDifferentiatedValue() const;
    float getSmoothedValue() const;
    bool isOutlier(float value) const;

private:
    float* values;
    unsigned long* timestamps;
    size_t currentIndex;
    size_t currentSize;
    const size_t historySize;
    const float outlierThreshold;

    float calculateSmoothedValue() const;
    float calculateIntegratedValue() const;
    float calculateDifferentiatedValue() const;
    bool detectOutlier(float value) const;
};

#endif // DATAPROCESSOR_HPP

