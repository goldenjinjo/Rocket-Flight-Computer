#include "deviceFunctions.hpp"
#include "pinAssn.hpp"
#include "Arduino.h"


// buzzer patterns to indicate success or failure
void buzzerSuccess() {
  tone(BUZZER, 600);
  delay(250);
  noTone(BUZZER);
  delay(250);
  tone(BUZZER, 600);
  delay(250);
  noTone(BUZZER);
}

void buzzerFailure() {
  tone(BUZZER, 1500);
  delay(500);
  noTone(BUZZER);
  delay(250);
  tone(BUZZER, 500);
  delay(3000);
  noTone(BUZZER); 
}

void startUp() {
  // Activate Buzzers to Confirm Code started
  
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
}