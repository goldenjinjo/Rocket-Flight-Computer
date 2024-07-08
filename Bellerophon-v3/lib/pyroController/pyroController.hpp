#ifndef PYROCONTROLLER_HPP
#define PYROCONTROLLER_HPP

#include "pinController.hpp"
#include "timer.hpp"

/**
 * @class PyroController
 * @brief Handles the triggering of pyro charges using a specified pin and delays.
 *
 * This class provides functionality to control a pyro charge using a MOSFET or similar device.
 * It takes care of initializing the pin and triggering the pyro charge after a specified delay
 * and holding it high for a defined duration.
 */
class PyroController {
public:
    /**
     * @brief Constructor to initialize the PyroController with a specific pin and delay.
     * @param pin The pin connected to the pyro charge.
     * @param triggerDelay The delay (in milliseconds) before the pyro charge is triggered.
     */
    PyroController(uint8_t pin, uint32_t triggerDelay);

    /**
     * @brief Method to initiate the trigger sequence.
     *
     * This method will handle the entire triggering sequence, starting with the initial delay,
     * setting the pin high, waiting for the hold duration, and then setting the pin low.
     * @return bool True if the trigger sequence is completed, False otherwise.
     */
    bool trigger();

    /**
     * @brief Method to check if the pyro controller has ever triggered.
     * @return bool True if the pyro controller has ever triggered, False otherwise.
     */
    bool hasEverTriggered() const;

private:
    uint8_t _pin;              // Pin connected to the pyro charge
    uint32_t _triggerDelay;    // Delay before the pyro charge is triggered
    uint32_t _holdDuration;    // Duration for which the pyro charge stays high
    bool _isTriggered;         // Indicates if the trigger sequence is in progress
    bool _hasEverTriggered;    // Indicates if the pyro controller has ever triggered

    PinController _pinControl; // Wrapper for pin control operations
    Timer _triggerTimer; // Timer instance to handle trigger timing
    Timer _holdTimer; //  Timer instance to handle the holdDuration

    /**
     * @brief Initializes the pin and sets it to LOW.
     */
    void initialize();

    /**
     * @brief Method to handle the trigger sequence logic.
     * @return bool True if the trigger sequence is completed, False otherwise.
     */
    bool handleTriggerSequence();

    /**
     * @brief Resets the trigger and hold timers.
     */
    void resetTimers();
};

#endif // PYROCONTROLLER_HPP
