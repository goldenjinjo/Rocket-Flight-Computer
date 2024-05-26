#include "deviceFunctions.hpp"

// TODO: Format this into a peripheral class
// TODO: Make multi-threaded using internal/external timers


// buzzer patterns to indicate success or failure

void peripheralInitialize() {
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Set LEDS to low in case previous program left it HIGH
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
}

/// TODO: set this for different colors and delays
/// TODO: make multithreaded
void LEDBlink() {
  digitalWrite(B_LED, HIGH);
  delay(500);
  digitalWrite(B_LED, LOW);
}

void LEDBlinkRED() {
  digitalWrite(R_LED, HIGH);
  delay(2000);
  digitalWrite(R_LED, LOW);
}


void buzzerSuccess() {
  tone(BUZZER, 900, 200); // High tone
  delay(200);
  tone(BUZZER, 1300, 200); // Higher tone
  delay(200);
  tone(BUZZER, 900, 200); // High tone
  delay(200);
  tone(BUZZER, 1300, 200); // Higher tone
}

void buzzerFailure() {
  tone(BUZZER, 100, 500);
  delay(500);
  tone(BUZZER, 200, 500);
  delay(500);
  tone(BUZZER, 100, 500);
  delay(500);
  tone(BUZZER, 400, 500);
}

void startUp() {
  // Activate Buzzers to Confirm Code started

  // play normal start up sounds if debug is not enabled 
  if(!DEBUG){
  tone(BUZZER, 700, 500);
  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, HIGH);
  delay(250);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  delay(250);
  tone(BUZZER, 600, 1000);
  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, HIGH);
  delay(500);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  delay(500);
  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, HIGH);
  tone(BUZZER, 700, 500);
  delay(500);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);;
  tone(BUZZER, 800, 1250);
  delay(1250);
  } else {
    digitalWrite(R_LED, HIGH);
    tone(BUZZER, 700, 500);
    delay(500);
    digitalWrite(R_LED, LOW);
    delay(500);
    digitalWrite(R_LED, HIGH);
    tone(BUZZER, 700, 500);
    delay(500);
    digitalWrite(R_LED,LOW);
  }
}