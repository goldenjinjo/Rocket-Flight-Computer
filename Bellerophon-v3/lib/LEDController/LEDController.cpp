#include "LEDController.hpp"

// Constructor to initialize the LEDController with a specific pin
LEDController::LEDController(uint8_t pin) : _pinControl(pin), _isPowered(false), _lastBlinkDuration(0) {
    initialize();
}

// Destructor to ensure the LED is turned off
LEDController::~LEDController() {
    turnOff();
}

void LEDController::initialize() {
    _pinControl.setMode(OUTPUT);
    turnOff();
}

void LEDController::blink(uint32_t blinkDuration) {
    // starts the blink timer when the method first runs 
    // or after timer reset
    if(!_blinkTimer.hasElapsed()) {
        _blinkTimer.start(blinkDuration);
        _lastBlinkDuration = blinkDuration;
        return;
    }
    updateBlinkState();
}

void LEDController::updateBlinkState() {
     // Once timer elapses, turn the LED on, if it was off previously
    if(_blinkTimer.hasElapsed() && !_isPowered) {
        turnOn();
        _blinkTimer.reset();
        // restarts timer to allow for one last blink, even if this method
        // was called in isolation
        _blinkTimer.start(_lastBlinkDuration);
        return;
    }
    // Once timer elapses, turn the LED off, if it was on previously
    if(_blinkTimer.hasElapsed() && _isPowered) {
        turnOff();
        _blinkTimer.reset();
        return;
    }
}

// Checks if the LED is currently on
bool LEDController::isPowered() const {
    return _isPowered;
}

// Turns the LED on
void LEDController::turnOn() {
    _pinControl.digitalWrite(HIGH);
    _isPowered = true;
}

// Turns the LED off
void LEDController::turnOff() {
    _pinControl.digitalWrite(LOW);
    _isPowered = false;
}