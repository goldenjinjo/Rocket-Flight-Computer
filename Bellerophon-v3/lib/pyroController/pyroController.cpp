#include "pyroController.hpp"

// Constructor to initialize the PyroController with a specific pin and delay
PyroController::PyroController(uint8_t pin, uint32_t triggerDelay)
    : _pinControl(pin), _triggerDelay(triggerDelay), _holdDuration(1000), _isTriggered(false), _startTime(0) {
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
        _startTime = millis();
        _isTriggered = true;
    }
    return handleTriggerSequence();
}

// Method to handle the trigger sequence logic
bool PyroController::handleTriggerSequence() {
    uint32_t currentTime = millis();
    
    if (_isTriggered) {
        if (currentTime - _startTime >= _triggerDelay && currentTime - _startTime < _triggerDelay + _holdDuration) {
            // After the initial delay, set the pin HIGH and hold it for the hold duration
            _pinControl.digitalWrite(HIGH);
        } else if (currentTime - _startTime >= _triggerDelay + _holdDuration) {
            // After the hold duration, set the pin LOW and reset the trigger state
            _pinControl.digitalWrite(LOW);
            _isTriggered = false;
            return true; // Trigger sequence is completed
        }
    }
    return false; // Trigger sequence is not yet completed
}
