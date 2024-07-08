#include "BuzzerController.hpp"

// Constructor to initialize the BuzzerController with a specific pin
BuzzerController::BuzzerController(uint8_t pin) : _pin(pin), _isPowered(false) {
    initialize();
}

// Destructor to ensure the buzzer is turned off
BuzzerController::~BuzzerController() {
    turnOff();
}

// Initializes the BuzzerController
void BuzzerController::initialize() {
    pinMode(_pin, OUTPUT);
    turnOff();
}

void BuzzerController::turnOn(uint16_t frequency) {
    tone(_pin, frequency);
    _isPowered = true;
}

// Starts the buzzer beeping with a specified duration and frequency
void BuzzerController::beep(uint32_t beepDuration, uint16_t frequency) {
    // starts the beep timer when the method first runs or after timer reset
    
    if(!_beepTimer.hasElapsed()) {
        _beepTimer.start(beepDuration);
        return;
    }
    
    if (_beepTimer.hasElapsed() && !_isPowered) {
        _beepTimer.reset();
        _beepTimer.start(beepDuration);
        turnOn(frequency);
        return;
    }
    // Once timer elapses, turn the buzzer off
    if (_beepTimer.hasElapsed() && _isPowered) {
        turnOff();
        _beepTimer.reset();
        _beepTimer.start(beepDuration);
        return;
    }
}

// Checks if the buzzer is currently on
bool BuzzerController::isOn() const {
    return _isPowered;
}

// Turns the buzzer off
void BuzzerController::turnOff() {
    noTone(_pin);
    _isPowered = false;
}

// Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
void BuzzerController::update() {
    if (_beepTimer.hasElapsed() && _isPowered) {
        turnOff();
        _beepTimer.reset();
    }
}
