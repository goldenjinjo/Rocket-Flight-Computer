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
    BuzzerFunctions(BuzzerController& buzzer);

    /**
     * @brief Selects the buzzer mode.
     * @param mode The mode to set the buzzer to.
     */
    void modeSelect(int mode);

    /**
     * @brief Plays the success sequence on the buzzer.
     */
    void success();

    /**
     * @brief Plays the failure sequence on the buzzer.
     */
    void failure();

    /**
     * @brief Plays the startup sequence on the buzzer,
     * variable based on debug mode
     */
    void startUp();

    /**
     * @brief Plays the landing tone. Designed to be played on repeat.
     * 
     */
    void landingTone();

    /**
     * @brief Updates the buzzer state. This should be called in the main loop 
     * to ensure non-blocking behavior.
     */
    void update();

private:
    BuzzerController& _buzzer;

    size_t _maxQueueSize = 20;

    /**
     * @brief Plays the standby mode sequence on the buzzer.
     */
    void standbyMode();

    /**
     * @brief Plays the reading mode sequence on the buzzer.
     */
    void readingMode();

    /**
     * @brief Plays the purge mode sequence on the buzzer.
     */
    void purgeMode();

    /**
     * @brief Plays the logging mode sequence on the buzzer.
     */
    void loggingMode();

    /**
     * @brief Plays the fin control mode sequence on the buzzer.
     */
    void finControlMode();

    /**
     * @brief Plays the config mode sequence on the buzzer.
     */
    void configMode();

    /**
     * @brief Plays the baro-only flight mode sequence on the buzzer.
     */
    void baroOnlyFlightMode();

    /**
     * @brief Plays the debug startup sequence on the buzzer.
     */
    void debugStartUp();

    /**
     * @brief Helper func to check if there is enough room left in queue
     * for an additional queueNum beep queues. Used to prevent tone 
     * sequences from playing when there is not enough space.
     * @return 
     */
    bool isSpaceInQueue(size_t queueNum);
};

#endif // BUZZER_FUNCTIONS_HPP
