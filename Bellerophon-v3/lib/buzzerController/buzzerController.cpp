#include "BuzzerController.hpp"

// Constructor to initialize the BuzzerController with a specific pin
BuzzerController::BuzzerController(uint8_t pin, size_t maxQueueSize) 
    : _pin(pin), _isPowered(false), _resetting(false), _maxQueueSize(maxQueueSize) {
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

// Starts the buzzer beeping with a specified duration and frequency
bool BuzzerController::beep(uint32_t beepDuration, uint16_t frequency) {
    return pushCommand(beepDuration, frequency);
}

// Keeps the buzzer silent for a specified duration
bool BuzzerController::silent(uint32_t duration) {
    return pushCommand(duration, 0);
}


bool BuzzerController::pushCommand(uint32_t beepDuration, uint16_t frequency) {
    if (isQueueFull()) {
        return false; // Queue is full, reject the command
    }
    if(_resetting) {
        // do not push new commands while system is resetting
        return false;   
    }
    BeepCommand command = {beepDuration, frequency};
    _beepQueue.push(command);
    // Sends buzzer command straight to the update sequence
    update();
    return true;
}

// Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
void BuzzerController::update() {
    // If we are in the process of resetting, clear one element from the queue
    if (_resetting) {
        clearQueue();
        return;  // Skip other processing while resetting
    }

    // If the buzzer is powered, handle the powered state
    if (_isPowered) {
        handlePoweredState();
    } else {
        // If the buzzer is not powered, process the next command in the queue
        processNextCommand();
    }
}

// Sets the reset flag to start clearing the queue incrementally
void BuzzerController::reset() {
    _resetting = true;
    turnOff();
}

// Checks if the buzzer is currently on
bool BuzzerController::isOn() const {
    return _isPowered;
}

// Returns the current size of the beep queue
size_t BuzzerController::getQueueSize() const {
    return _beepQueue.size();
}

size_t BuzzerController::getMaxQueueSize() const {
    return _maxQueueSize;
}

// Checks if the queue is full
bool BuzzerController::isQueueFull() const {
    return getQueueSize() >= _maxQueueSize;
}

void BuzzerController::clearQueue() {
    if (!_beepQueue.empty()) {
            _beepQueue.pop();
        } else {
            _resetting = false;  // Reset complete
        }
}

// Processes the next command in the beep queue
void BuzzerController::processNextCommand() {
    if (_beepQueue.empty()) {
        return;
    }

    BeepCommand command = _beepQueue.front();
    _beepQueue.pop();
    turnOn(command.frequency);
    _beepTimer.start(command.duration);
}

// Handles the buzzer state when it is powered
void BuzzerController::handlePoweredState() {
    if (_beepTimer.hasElapsed()) {
        turnOff();
        _beepTimer.reset();
    }
}

// Turns the buzzer on with a specific frequency
void BuzzerController::turnOn(uint16_t frequency) {
    
    _isPowered = true;
    
    if(frequency <= 0) {
        noTone(_pin);
        return;
    }
    tone(_pin, frequency);
}

// Turns the buzzer off
void BuzzerController::turnOff() {
    noTone(_pin);
    _isPowered = false;
}

