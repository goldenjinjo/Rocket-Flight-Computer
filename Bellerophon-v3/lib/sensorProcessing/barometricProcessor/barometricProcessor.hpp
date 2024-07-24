#ifndef barometricProcessor_HPP
#define barometricProcessor_HPP

#include "dataProcessor.hpp"
#include "sensorProcessor.hpp"
#include "pressureSensor.hpp"
#include "configKeys.hpp"

/**
 * @class BarometricProcessor
 * @brief Class for processing barometric pressure data and estimating altitude and vertical velocity.
 */
class BarometricProcessor : public DataProcessor, public SensorProcessor {
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
    void update() override;

    /**
     * @brief Get the estimated altitude.
     * 
     * @return Estimated altitude.
     */
    float getAltitude() const override;

    /**
     * @brief Get the estimated vertical velocity.
     * 
     * @return Estimated vertical velocity.
     */
    float getVerticalVelocity() const override;

    /**
     * @brief Get the maximum recorded altitude.
     * 
     * @return Maximum recorded altitude.
     */
    float getMaxAltitude() const override;

    /**
     * @brief Get the maximum recorded vertical velocity.
     * 
     * @return Maximum recorded vertical velocity.
     */
    float getMaxVelocity() const override;

    /**
     * @brief Get the ground altitude recorded at end of stabilisation period
     * 
     * @return ground altitude
     */
    float getGroundAltitude() const override;

    /**
     * @brief Override of virtual method. Barometer not able to calculate acceleration.
     * 
     * @return 0
     */
    float getAcceleration() const override {
        return 0;
    }
    
    /**
     * @brief Override of virtual method. Barometer not able to calculate acceleration.
     * 
     * @return 0
     */
    float getMaxAcceleration() const override {
        return 0;
    }


private:
    PressureSensor& pressureSensor_;
    float maxAltitude_;
    float maxVelocity_;
    float groundAltitude_;

    float calculateAltitude(float pressure) const; // Helper function to calculate altitude from pressure
    void updateMaxAltitude();
    void updateMaxVelocity();
    void updateGroundAltitude();
};

#endif // barometricProcessor_HPP
