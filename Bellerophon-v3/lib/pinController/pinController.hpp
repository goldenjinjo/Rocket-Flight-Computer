#ifndef PINCONTROLLER_HPP
#define PINCONTROLLER_HPP

#include <Arduino.h>

/**
 * @class PinController
 * @brief Wrapper class for pin control operations.
 *
 * This class provides an abstraction for basic pin operations such as setting the mode,
 * writing to the pin, and reading from the pin. It uses the Arduino framework functions
 * internally but can be easily modified to use other frameworks if needed.
 */
class PinController {
public:
    /**
     * @brief Constructor to initialize the PinController with a specific pin.
     * @param pin The pin number to be controlled.
     */
    PinController(uint8_t pin);

    /**
     * @brief Sets the mode of the pin.
     * @param mode The mode to set (INPUT, OUTPUT, INPUT_PULLUP, etc.).
     */
    void setMode(uint8_t mode);

    /**
     * @brief Writes a digital value to the pin.
     * @param value The value to write (HIGH or LOW).
     */
    void digitalWrite(uint8_t value);

    /**
     * @brief Reads a digital value from the pin.
     * @return The value read from the pin (HIGH or LOW).
     */
    int digitalRead() const;

    /**
     * @brief Writes an analog value to the pin.
     * @param value The value to write (0 to 255).
     */
    void analogWrite(int value);

    /**
     * @brief Reads an analog value from the pin.
     * @return The value read from the pin (0 to 1023).
     */
    int analogRead() const;

private:
    uint8_t _pin; // The pin number
};

#endif // PINCONTROLLER_HPP
