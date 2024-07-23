#ifndef BAROMETRIC_PROCESSOR_HPP
#define BAROMETRIC_PROCESSOR_HPP

#include "dataProcessor.hpp"
#include "PressureSensor.hpp"
#include "configKeys.hpp"

/**
 * @class BarometricProcessor
 * @brief Class for processing barometric pressure data and estimating altitude and vertical velocity.
 */
class BarometricProcessor : public DataProcessor {
public:
    /**
     * @brief Constructor with history size and outlier threshold.
     * 
     * @param pressureSensor Reference to the pressure sensor.
     * @param historySize The size of the history buffer for smoothing and differentiation.
     * @param outlierThreshold The threshold for detecting outliers.
     */
    BarometricProcessor(PressureSensor& pressureSensor, size_t historySize, float outlierThreshold = 10.0);

    /**
     * @brief Update the pressure sensor data and process it.
     */
    void update();

    /**
     * @brief Get the estimated altitude.
     * 
     * @return Estimated altitude.
     */
    float getAltitude() const;

    /**
     * @brief Get the estimated vertical velocity.
     * 
     * @return Estimated vertical velocity.
     */
    float getVerticalVelocity() const;

private:
    PressureSensor& pressureSensor_;
    float referencePressure_;
    bool firstUpdate_;

};

#endif // BAROMETRIC_PROCESSOR_HPP
