#ifndef DATAPROCESSOR_HPP
#define DATAPROCESSOR_HPP

#include <vector>
#include <cstddef>

class DataProcessor {
public:
    DataProcessor(size_t historySize, float outlierThreshold = 10.0);
    void update(float value);
    float getIntegratedValue() const;
    float getDifferentiatedValue() const;
    float getSmoothedValue() const;
    bool isOutlier(float value) const;

private:
    std::vector<float> dataHistory;
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
