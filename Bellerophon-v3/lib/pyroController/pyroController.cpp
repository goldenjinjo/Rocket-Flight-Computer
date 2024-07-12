#include "pyroController.hpp"

// Constructor to initialize the PyroController with a specific pin and delay
PyroController::PyroController(uint8_t pin, uint32_t triggerDelay)
    : _pinControl(pin), _triggerDelay(triggerDelay), _holdDuration(2000), _isTriggered(false), _hasEverTriggered(false) {
    initialize();
}

// Initializes the pin and sets it to LOW
void PyroController::initialize() {
    _pinControl.setMode(OUTPUT);
    _pinControl.digitalWrite(LOW);
}

// Method to initiate the trigger sequence
bool PyroController::trigger() {
    if (!_isTriggered) {
        _triggerTimer.start(_triggerDelay);
        _isTriggered = true;
    }
    return handleTriggerSequence();
}

// Method to cancel the trigger sequence
bool PyroController::cancelTrigger() {
    // return false if there is no trigger to cancel
    if(!_isTriggered) {
        return false;
    }

    _pinControl.digitalWrite(LOW);
    resetTimers();
    _isTriggered = false;
    // return true if trigger successfully cancelled
    return true;
}

// Check if the pyro controller has ever triggered
bool PyroController::hasEverTriggered() const {
    return _hasEverTriggered;
}

// Method to handle the trigger sequence logic
bool PyroController::handleTriggerSequence() {
    // Does nothing and returns false if there is no trigger or trigger was cancelled
    if(!_isTriggered) {
        return false;
    }

    if (_triggerTimer.hasElapsed() && !_holdTimer.hasElapsed()) {
        // After the initial delay, set the pin HIGH and start the hold timer
        _pinControl.digitalWrite(HIGH);
        // Holds the trigger high for the _holdDuration time
        _holdTimer.start(_holdDuration);

    } else if(_holdTimer.hasElapsed()){
        // releases pyro trigger
        cancelTrigger();
        // set flag to true, will now remain true for duration of class instance
        _hasEverTriggered = true;
        return true;
    }
    return false; // Trigger sequence is not yet completed
}

// Resets the timers
void PyroController::resetTimers() {
    _triggerTimer.reset();
    _holdTimer.reset();
}