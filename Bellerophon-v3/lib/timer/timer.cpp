#include "timer.hpp"

// Constructor to initialize the Timer
Timer::Timer() : _startTime(0), _duration(0), _running(false) {}

bool Timer::startAndCheck(uint32_t duration) {
    start(duration);
    return hasElapsed();
}

// Starts the timer with a specific duration
void Timer::start(uint32_t duration) {
    // prevent timer from restarting if timer already running
    if(_running) {
        return;
    }
    _startTime = millis();
    _duration = duration;
    _running = true;
}

// Checks if the timer has completed
bool Timer::hasElapsed() const {
    if (!_running) {
        return false;
    }
    return (millis() - _startTime) >= _duration;
}

// Resets the timer
void Timer::reset() {
    _running = false;
}