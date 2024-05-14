#include "deviceFunctions.hpp"

// buzzer patterns to indicate success or failure
void buzzerSuccess() {
  tone(BUZZER, 300, 500);
  delay(500);
  tone(BUZZER, 1100, 500);
  delay(500);
  tone(BUZZER, 300, 500);
  delay(500);
  tone(BUZZER, 1100, 500);
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