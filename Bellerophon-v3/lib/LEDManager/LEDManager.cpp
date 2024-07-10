#include "LEDManager.hpp"
#include "pinAssn.hpp" // Include your pin assignments

// Constructor to initialize the LEDManager with predefined LEDs
LEDManager::LEDManager() {
    initializeLEDs();
}
    
// Initializes the predefined LEDs
void LEDManager::initializeLEDs() {
    _leds.emplace(R_LED, LEDController(R_LED));
    _leds.emplace(G_LED, LEDController(G_LED));
    _leds.emplace(B_LED, LEDController(B_LED));
    _leds.emplace(PRESSURE_LED, LEDController(PRESSURE_LED));
    _leds.emplace(IMU_LED, LEDController(IMU_LED));
    _leds.emplace(FLASH_LED, LEDController(FLASH_LED));
}

// Blinks the specified LED for the given duration
void LEDManager::blink(uint8_t pin, uint32_t blinkDuration) {
    auto it = _leds.find(pin);
    if (it != _leds.end()) {
        it->second.blink(blinkDuration);
    }
}

// Updates all LEDs managed by the LEDManager. This should be called in the main loop to ensure non-blocking behavior.
void LEDManager::updateAllLEDS() {
    for (auto& pair : _leds) {
        pair.second.updateBlinkState();
    }
}

// Checks if the given pin is a valid LED
bool LEDManager::isValidLED(uint8_t pin) const {
    return _leds.find(pin) != _leds.end();
}
