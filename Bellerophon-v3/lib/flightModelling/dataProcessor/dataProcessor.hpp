#ifndef DATAPROCESSOR_HPP
#define DATAPROCESSOR_HPP

#include <cstddef> // Include this header for size_t
#include "timer.hpp"
#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>

/**
 * @class DataProcessor
 * @brief Base class for processing sensor data.
 *
 * This class provides methods for smoothing, integrating, and differentiating sensor data.
 * It also includes functionality for detecting outliers.
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

protected:
    float* values;              ///< Array to store sensor values.
    unsigned long* timestamps;  ///< Array to store timestamps of the sensor values.
    size_t currentIndex;        ///< Current index in the history buffer.
    size_t currentSize;         ///< Current size of the history buffer.
    const size_t historySize;   ///< Maximum size of the history buffer.
    const float outlierThreshold; ///< Threshold for detecting outliers.

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
     * @param value The value to check.
     * @return True if the value is an outlier, false otherwise.
     */
    virtual bool detectOutlier(float value) const;
};

#endif // DATAPROCESSOR_HPP
