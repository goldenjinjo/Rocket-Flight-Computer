#ifndef LEDMANAGER_HPP
#define LEDMANAGER_HPP

#include <vector>
#include <map>
#include "LEDController.hpp"

class LEDManager {
public:
    /**
     * @brief Constructor to initialize the LEDManager with predefined LEDs.
     */
    LEDManager();

    /**
     * @brief Blinks the specified LED for the given duration.
     * @param pin The pin connected to the LED.
     * @param blinkDuration The duration (in milliseconds) for which the LED should blink.
     */
    void blink(uint8_t pin, uint32_t blinkDuration);

    /**
     * @brief Updates all LEDs managed by the LEDManager. This should be called in the main loop to ensure non-blocking behavior.
     */
    void updateAllLEDS();

    /**
     * @brief Checks if the given pin is a valid LED.
     * @param pin The pin to check.
     * @return True if the pin is a valid LED, false otherwise.
     */
    bool isValidLED(uint8_t pin) const;

private:
    std::map<uint8_t, LEDController> _leds; // Map of pin numbers to LEDController instances

    /**
     * @brief Initializes the predefined LEDs.
     */
    void initializeLEDs();
};

#endif // LEDMANAGER_HPP
