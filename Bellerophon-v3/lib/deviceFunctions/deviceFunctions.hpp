#ifndef DEVICE_FUNCTIONS_HPP
#define DEVICE_FUNCTIONS_HPP

#include <Arduino.h>
#include "pinAssn.hpp"
#include "constants.hpp"

void buzzerSuccess();
void buzzerFailure();
void startUp();
void peripheralInitialize();
void LEDBlink(uint8_t LED_PIN, uint32_t blinkTime);
void LEDBlinkRED();
void buzzerModeSelect(int mode);
void cycleLEDS(uint32_t blinkTime);

#endif //DEVICE_FUNCTIONS_HPP
