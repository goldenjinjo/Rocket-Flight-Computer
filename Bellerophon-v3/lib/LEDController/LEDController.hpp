#ifndef LEDCONTROLLER_HPP
#define LEDCONTROLLER_HPP

#include "PinController.hpp"
#include "Timer.hpp"

/**
 * @class LEDController
 * @brief A class to control an LED with non-blocking blink functionality.
 *
 * This class provides methods to turn an LED on, off, and blink it with a specified duration
 * using non-blocking delays.
 */
class LEDController {
public:
    /**
     * @brief Constructor to initialize the LEDController with a specific pin.
     * @param pin The pin connected to the LED.
     */
    LEDController(uint8_t pin);

    /**
     * @brief Destructor to ensure the LED is turned off.
     */
    ~LEDController();

    /**
     * @brief Blinks LED for equal non-blocking time on and off.
     * @param blinkDuration The time to blink for. e.g. 1000: 1 second on, one second off.
     */
    void blink(uint32_t blinkDuration);

    /**
     * @brief Checks if the LED is currently on.
     * @return True if the LED is on, false otherwise.
     */
    bool isPowered() const;

    /**
     * @brief Turns the LED on and sets the _isPowered flag to true.
     */
    void turnOn();

    /**
     * @brief Turns the LED off _isPowered flag to false.
     */
    void turnOff();

private:
    PinController _pinControl; // Wrapper for pin control operations
    Timer _blinkTimer;         // Timer for the blink duration
    bool _isPowered;          // Indicates if the LED is currently blinking

    /**
     * @brief Sets pin mode to output and digitial voltage state to low.
     */
    void initialize();
};

#endif // LEDCONTROLLER_HPP
