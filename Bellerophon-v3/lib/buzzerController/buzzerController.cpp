#include "BuzzerController.hpp"

// Constructor to initialize the BuzzerController with a specific pin
BuzzerController::BuzzerController(uint8_t pin, size_t maxQueueSize) 
    : _pin(pin), _isPowered(false), _maxQueueSize(maxQueueSize) {
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

// Turns the buzzer on with a specific frequency
void BuzzerController::turnOn(uint16_t frequency) {
    tone(_pin, frequency);
    _isPowered = true;
}

// Starts the buzzer beeping with a specified duration and frequency
bool BuzzerController::beep(uint32_t beepDuration, uint16_t frequency) {
    if (_beepQueue.size() >= _maxQueueSize) {
        return false; // Queue is full, reject the command
    }
    BeepCommand command = {beepDuration, frequency};
    _beepQueue.push(command);
    return true;
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
    if (!_beepQueue.empty()) {
        if (!_isPowered) {
            BeepCommand command = _beepQueue.front();
            _beepQueue.pop();
            turnOn(command.frequency);
            _beepTimer.start(command.duration);
        } else if (_beepTimer.hasElapsed()) {
            turnOff();
            _beepTimer.reset();
        }
    } else if (_isPowered && _beepTimer.hasElapsed()) {
        turnOff();
        _beepTimer.reset();
    }
}

// Returns the current size of the beep queue
size_t BuzzerController::getQueueSize() const {
    return _beepQueue.size();
}
