#include "pyroController.hpp"

// Constructor to initialize the PyroController with a specific pin and delay
PyroController::PyroController(uint8_t pin, uint32_t triggerDelay)
    : _pinControl(pin), _triggerDelay(triggerDelay), _holdDuration(2000), _isTriggered(false), _startTime(0) {
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

// Method to handle the trigger sequence logic
bool PyroController::handleTriggerSequence() {
    
    if(!_isTriggered) {
        return false;
    }

    if (_triggerTimer.hasElapsed() && !_holdTimer.hasElapsed()) {
        // After the initial delay, set the pin HIGH and start the hold timer
        _pinControl.digitalWrite(HIGH);
        _holdTimer.start(_holdDuration);
        
    } else if(_holdTimer.hasElapsed()){
        _pinControl.digitalWrite(LOW);
        _holdTimer.reset();
        _triggerTimer.reset();
        _isTriggered = false;
        return true;
    }
    return false; // Trigger sequence is not yet completed
}
