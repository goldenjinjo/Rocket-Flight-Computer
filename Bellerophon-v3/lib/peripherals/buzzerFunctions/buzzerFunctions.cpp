#include "BuzzerFunctions.hpp"

// Constructor to initialize the BuzzerFunctions with a specific pin
BuzzerFunctions::BuzzerFunctions(BuzzerController& buzzer)
    : _buzzer(buzzer) {}

// Selects the buzzer mode
void BuzzerFunctions::modeSelect(int mode) {

    if(!isSpaceInQueue(9)){
    return;
    }

    switch (mode) {
        case STANDBY_MODE:
            standbyMode();
            break;

        case READING_MODE:
            readingMode();
            break;

        case PURGE_MODE:
            purgeMode();
            break;

        case LOGGING_MODE:
            loggingMode();
            break;

        case FIN_CONTROL_MODE:
            finControlMode();
            break;

        case CONFIG_MODE:
            configMode();
            break;

        default:
            // Handle unknown mode
            break;
    }
}

// Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
void BuzzerFunctions::update() {
    _buzzer.update();
}

// Standby mode: Single long beep
void BuzzerFunctions::standbyMode() {
    _buzzer.beep(1500, 1000); // 1kHz tone for 1.5 seconds
    _buzzer.silent(500);
}

// Reading mode: Beep - Beep - Beep (3 short beeps)
void BuzzerFunctions::readingMode() {
    for (int i = 0; i < 3; i++) {
        _buzzer.beep(500, 900 + (30 * i)); // Start with 900Hz tone
        _buzzer.silent(167); // Short pause between beeps
    }
    _buzzer.silent(500);
}

// Purge mode: Beeeep - Beeeeep - Beeeeep (3 long beeps)
void BuzzerFunctions::purgeMode() {
    for (int i = 0; i < 3; i++) {
        _buzzer.beep(1000, 1000); // 1kHz tone for 1 second
        _buzzer.silent(500); // Pause between beeps
    }
    _buzzer.silent(500);
}

// Logging mode: Beep-beep - Beep-beep (2 double beeps)
void BuzzerFunctions::loggingMode() {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            _buzzer.beep(250, 1500); // 1.5kHz tone for 0.25 seconds
            _buzzer.silent(250); // Short pause between beeps
        }
        _buzzer.silent(500); // Pause between double beeps
    }
    _buzzer.silent(500);
}

// Fin control mode: Rapid short beeps
void BuzzerFunctions::finControlMode() {
    for (int i = 0; i < 5; i++) {
        _buzzer.beep(100, 2000); // 2kHz tone for 0.1 seconds
        _buzzer.silent(100); // Short pause between beeps
    }
    _buzzer.silent(500);
}

// Config mode: Beep - Beep-beep - Beep (1 short beep, 1 double beep, 1 short beep)
void BuzzerFunctions::configMode() {
    _buzzer.beep(500, 1000); // 1kHz tone for 0.5 seconds
    _buzzer.silent(250); // Pause between beeps
    for (int i = 0; i < 2; i++) {
        _buzzer.beep(250, 1000); // 1kHz tone for 0.25 seconds
        _buzzer.silent(250); // Short pause between beeps
    }
    _buzzer.beep(500, 1000); // 1kHz tone for 0.5 seconds

    _buzzer.silent(500);
}

// Baro-only flight mode: Long beep - Short beep - Long beep
void BuzzerFunctions::baroOnlyFlightMode() {
    _buzzer.beep(1000, 1200); // 1.2kHz tone for 1 second
    _buzzer.silent(500); // Pause between beeps
    _buzzer.beep(500, 1200); // 1.2kHz tone for 0.5 seconds
    _buzzer.silent(500); // Pause between beeps
    _buzzer.beep(1000, 1200); // 1.2kHz tone for 1 second

    _buzzer.silent(500);
}

// Success sequence: High tone - Higher tone - High tone - Higher tone
void BuzzerFunctions::success() {
    if(!isSpaceInQueue(8)){
    return;
    }
    _buzzer.beep(200, 900);  // High tone
    _buzzer.silent(200); 
    _buzzer.beep(200, 1300); // Higher tone
    _buzzer.silent(200); 
    _buzzer.beep(200, 900);  // High tone
    _buzzer.silent(200); 
    _buzzer.beep(200, 1300); // Higher tone

    _buzzer.silent(500);
}

// Failure sequence: Low tones with pauses
void BuzzerFunctions::failure() {

    if(!isSpaceInQueue(8)){
    return;
    }
    _buzzer.beep(500, 100);  // Low tone
    _buzzer.silent(500);
    _buzzer.beep(500, 200);  // Slightly higher low tone
    _buzzer.silent(500);
    _buzzer.beep(500, 100);  // Low tone
    _buzzer.silent(500);
    _buzzer.beep(500, 400);  // Slightly higher low tone

    _buzzer.silent(500);
}

void BuzzerFunctions::startUp() {
  
  if(!isSpaceInQueue(6)){
    return;
  }

  if(DEBUG){
    debugStartUp();
    return;
  }
  // Queue buzzer tones
  _buzzer.beep(500,700);
  _buzzer.beep(1000, 600);
  _buzzer.beep(500, 700);
  _buzzer.beep(1250, 800);
  // give a 2 second pause in case another function is queued
  _buzzer.silent(2000);
}

void BuzzerFunctions::debugStartUp() {
    _buzzer.beep(500, 700);
    _buzzer.silent(500);
    _buzzer.beep(500, 700);
    // give a 2 second pause in case another function is queued
    _buzzer.silent(2000);
}

void BuzzerFunctions::landingTone() {
    // Designed to be run on loop
    if(!isSpaceInQueue(2)){
        return;
  }
  _buzzer.beep(2000, 1000);
  _buzzer.silent(2000);

}

// Pre-launch mode: A cheerful beep pattern to save power consumption
void BuzzerFunctions::preLaunchTone() {
    if (!isSpaceInQueue(6)) {
        return;
    }

    // A cheerful sequence: Beep - Higher beep - Lower beep - Pause
    _buzzer.beep(500, 1000);   // 1kHz tone for 0.5 seconds
    _buzzer.silent(200);       // Short pause
    _buzzer.beep(500, 1200);   // 1.2kHz tone for 0.5 seconds
    _buzzer.silent(200);       // Short pause
    _buzzer.beep(500, 800);    // 0.8kHz tone for 0.5 seconds
    _buzzer.silent(10000);      // 10 seconds of silence
}

bool BuzzerFunctions::isSpaceInQueue(size_t queueNum) {
    return (_buzzer.getQueueSize() < (_buzzer.getMaxQueueSize() - queueNum));
}