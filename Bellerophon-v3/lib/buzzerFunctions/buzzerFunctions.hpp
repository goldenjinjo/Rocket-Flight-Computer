#ifndef BUZZER_FUNCTIONS_HPP
#define BUZZER_FUNCTIONS_HPP

#include "BuzzerController.hpp"
#include "pinAssn.hpp"
#include "constants.hpp"

/**
 * @class BuzzerFunctions
 * @brief Manages different buzzer modes and operations.
 */
class BuzzerFunctions {
public:
    /**
     * @brief Constructor to initialize the BuzzerFunctions with a specific pin.
     * @param pin The pin number the buzzer is connected to.
     * @param maxQueueSize The maximum size of the command queue (default is 10).
     */
    BuzzerFunctions(uint8_t pin, size_t maxQueueSize = 10);

    /**
     * @brief Selects the buzzer mode.
     * @param mode The mode to set the buzzer to.
     */
    void modeSelect(int mode);

    void success();

    void failure();

    /**
     * @brief Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
     */
    void update();

private:
    BuzzerController _buzzer;

    // Mode functions
    void standbyMode();
    void readingMode();
    void purgeMode();
    void loggingMode();
    void finControlMode();
    void configMode();
    void baroOnlyFlightMode();
};

#endif // BUZZER_FUNCTIONS_HPP
