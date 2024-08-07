#include "pinController.hpp"

// Constructor to initialize the PinController with a specific pin
PinController::PinController(uint8_t pin) : _pin(pin) {}

// Sets the mode of the pin
void PinController::setMode(uint8_t mode) {
    pinMode(_pin, mode);
}

// Writes a digital value to the pin
void PinController::digitalWrite(uint8_t value) {
    ::digitalWrite(_pin, value);
}

// Reads a digital value from the pin
int PinController::digitalRead() const {
    return ::digitalRead(_pin);
}

// Writes an analog value to the pin
void PinController::analogWrite(int value) {
    ::analogWrite(_pin, value);
}

// Reads an analog value from the pin
int PinController::analogRead() const {
    return ::analogRead(_pin);
}
