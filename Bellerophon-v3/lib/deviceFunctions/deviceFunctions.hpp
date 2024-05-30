#ifndef DEVICE_FUNCTIONS_HPP
#define DEVICE_FUNCTIONS_HPP

#include <Arduino.h>
#include "pinAssn.hpp"
#include "config.hpp"

void buzzerSuccess();
void buzzerFailure();
void startUp();
void peripheralInitialize();
void LEDBlink(uint8_t LED_PIN, uint32_t blinkTime);
void LEDBlinkRED();
void buzzerModeSelect(int mode);

#endif //DEVICE_FUNCTIONS_HPP
