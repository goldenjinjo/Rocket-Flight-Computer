#include "deviceFunctions.hpp"

// TODO: Format this into a peripheral class
// TODO: Make multi-threaded using internal/external timers

void startUp() {
  // Activate Buzzers to Confirm Code started

  // play normal start up sounds if debug is not enabled 
  if(!DEBUG){
  tone(BUZZER, 700, 500);
  delay(500);
  tone(BUZZER, 600, 1000);
  delay(500);
  delay(500);
  tone(BUZZER, 700, 500);
  delay(500);
  tone(BUZZER, 800, 1250);
  } else {
    tone(BUZZER, 700, 500);
    delay(500);
    delay(500);
    tone(BUZZER, 700, 500);
  }
}

