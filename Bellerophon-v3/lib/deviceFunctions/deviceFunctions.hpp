#ifndef DEVICE_FUNCTIONS_HPP
#define DEVICE_FUNCTIONS_HPP

#include <Arduino.h>
#include "pinAssn.hpp"
#include "config.hpp"

void buzzerSuccess();
void buzzerFailure();
void startUp();
void peripheralInitialize();
void LEDBlink();
void LEDBlinkRED();

#endif //DEVICE_FUNCTIONS_HPP
