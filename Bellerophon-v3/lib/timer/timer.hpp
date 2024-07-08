#ifndef TIMER_HPP
#define TIMER_HPP

#include <Arduino.h>

/**
 * @class Timer
 * @brief A simple class for handling non-blocking delays.
 *
 * This class provides functionality to handle non-blocking delays using the Arduino `millis()` function.
 */
class Timer {
public:
    /**
     * @brief Constructor to initialize the Timer.
     */
    Timer();

    /**
     * @brief Starts the timer with a specific duration.
     * @param duration The duration for which the timer should run (in milliseconds).
     */
    void start(uint32_t duration);

    /**
     * @brief Checks if the timer has completed.
     * @return True if the timer has completed, false otherwise.
     */
    bool hasElapsed() const;

    /**
     * @brief Resets the timer.
     */
    void reset();

private:
    uint32_t _startTime; // The start time of the timer
    uint32_t _duration;  // The duration for which the timer should run
    bool _running;       // Indicates if the timer is running
};

#endif // TIMER_HPP
