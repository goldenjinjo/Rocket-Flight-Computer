#ifndef DATAPROCESSOR_HPP
#define DATAPROCESSOR_HPP

#include <cstddef> // Include this header for size_t
#include "timer.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>
#include <deque>   // Include for the sliding window

/**
 * @class DataProcessor
 * @brief Base class for processing sensor data.
 *
 * This class provides methods for smoothing, integrating, and differentiating sensor data.
 * It also includes functionality for detecting outliers and a stabilization phase.
 */
class DataProcessor {
public:
    /**
     * @brief Constructor for DataProcessor.
     * 
     * @param historySize The size of the history buffer.
     * @param outlierThreshold The threshold for detecting outliers.
     */
    DataProcessor(size_t historySize, float outlierThreshold = 10.0);

    /**
     * @brief Virtual destructor for DataProcessor.
     */
    virtual ~DataProcessor();

    /**
     * @brief Pure virtual function for updating sensor data.
     *
     * This method should be overridden by subclasses to fetch data from specific sensors
     * and update the internal data buffer. The subclasses will provide the implementation
     * specific to the sensor they handle.
     */
    virtual void update() = 0;

    /**
     * @brief Get the integrated value of the sensor data.
     * 
     * @return The integrated value.
     */
    float getIntegratedValue() const;

    /**
     * @brief Get the differentiated value of the sensor data.
     * 
     * @return The differentiated value.
     */
    float getDifferentiatedValue() const;

    /**
     * @brief Get the smoothed value of the sensor data.
     * 
     * @return The smoothed value.
     */
    float getSmoothedValue() const;

    /**
     * @brief Check if a value is an outlier.
     * 
     * @param value The value to check.
     * @return True if the value is an outlier, false otherwise.
     */
    bool isOutlier(float value) const;

    /**
     * @brief Check if the processor has stabilized.
     * 
     * @return True if the processor has stabilized, false otherwise.
     */
    bool isStabilized() const;

    /**
     * @brief Get the number of detected outliers.
     * 
     * @return The number of outliers detected.
     */
    size_t getOutlierCount() const;

protected:
    float* values;              ///< Array to store sensor values.
    unsigned long* timestamps;  ///< Array to store timestamps of the sensor values.
    size_t currentIndex;        ///< Current index in the history buffer.
    size_t currentSize;         ///< Current size of the history buffer.
    const size_t historySize;   ///< Maximum size of the history buffer.
    const float outlierThreshold; ///< Threshold for detecting outliers.
    bool stabilizationPhase;    ///< Flag indicating if the processor is in the stabilization phase.
    size_t stabilizationCount;  ///< Counter for the stabilization phase.
    const size_t stabilizationLimit; ///< Limit for the stabilization phase.
    std::deque<float> slidingWindow; ///< Sliding window for robust outlier detection
    std::deque<float> rateOfChangeWindow; ///< Sliding window for rate of change
    size_t outlierCount; ///< Counter for the number of detected outliers

    /**
     * @brief Update the internal buffer with new data and record the current timestamp.
     * 
     * @param value The new sensor data value.
     */
    void updateBuffer(float value);

    /**
     * @brief Calculate the smoothed value of the sensor data.
     * 
     * @return The smoothed value.
     */
    virtual float calculateSmoothedValue() const;

    /**
     * @brief Calculate the integrated value of the sensor data.
     * 
     * @return The integrated value.
     */
    virtual float calculateIntegratedValue() const;

    /**
     * @brief Calculate the differentiated value of the sensor data.
     * 
     * @return The differentiated value.
     */
    virtual float calculateDifferentiatedValue() const;

    /**
     * @brief Detect if a value is an outlier.
     * 
     * This method determines if a given value is an outlier based on the rate of change compared
     * to the last value in the sliding window.
     *
     * @param value The value to check.
     * @return True if the value is an outlier, false otherwise.
     */
    virtual bool detectOutlier(float value) const;

    /**
     * @brief Validate the data during the stabilization phase.
     * 
     * @param value The new sensor data value.
     */
    void stabilize(float value);

private:
    /**
     * @brief Update the sliding window with a new value.
     * 
     * This method adds a new value to the sliding window, ensuring that the window size does not
     * exceed the history size. It also updates the rate of change window accordingly.
     *
     * @param value The new sensor data value to add to the sliding window.
     */
    void updateSlidingWindow(float value);
};

#endif // DATAPROCESSOR_HPP