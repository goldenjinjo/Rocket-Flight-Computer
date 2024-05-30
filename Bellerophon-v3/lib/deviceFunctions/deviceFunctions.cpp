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
void LEDBlink(uint8_t LED_PIN, uint32_t blinkTime) {
  digitalWrite(B_LED, HIGH);
  delay(blinkTime);
  digitalWrite(B_LED, LOW);
}

void LEDBlinkRED() {
  digitalWrite(R_LED, HIGH);
  delay(2000);
  digitalWrite(R_LED, LOW);
}

// Unique tone sounds for the different modes
void buzzerModeSelect(int mode) {

  switch (mode) {
    case STANDBY_MODE:
      // Standby mode: Single long beep
      tone(BUZZER, 1000, 2000); // 1kHz tone for 2 seconds
      delay(2000);
      noTone(BUZZER);
      break;

    case READING_MODE:
      // Reading mode: Beep - Beep - Beep (3 short beeps)
      for (int i = 0; i < 3; i++) {
        tone(BUZZER, 900+(30*i)); // Start 1kHz tone
        delay(500);        // Wait for 0.5 seconds
        noTone(BUZZER);    // Stop the tone
        delay(167);        // Short pause between beeps
      }
      break;

    case PURGE_MODE:
      // Purge mode: Beeeep - Beeeep (2 long beeps)
      for (int i = 0; i < 2; i++) {
        tone(BUZZER, 500, 1000); // 500Hz tone for 1 second
        delay(1000);
        noTone(BUZZER);
        delay(500); // Pause between beeps
      }
      break;

    case LOGGING_MODE:
      // Logging mode: Beep-beep - Beep-beep (2 double beeps)
      for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
          tone(BUZZER, 1500, 250); // 1.5kHz tone for 0.25 seconds
          delay(250);
          noTone(BUZZER);
          delay(250); // Short pause between beeps
        }
        delay(500); // Pause between double beeps
      }
      break;

    default:
      // No mode selected or invalid mode
      noTone(BUZZER);
      break;
  }
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