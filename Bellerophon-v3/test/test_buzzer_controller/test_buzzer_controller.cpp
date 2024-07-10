#include <Arduino.h>
#include <unity.h>
#include "BuzzerController.hpp"
#include "Timer.hpp"
#include "pinAssn.hpp"


size_t buzzerQueueLimit = 20;
// Create an instance of BuzzerController
BuzzerController buzzer(BUZZER, buzzerQueueLimit);

// Create an instance of the Timer
Timer testTimer;

// Setup function runs before each test
void setUp(void) {
    // Initialize the buzzer
    buzzer = BuzzerController(BUZZER, buzzerQueueLimit);
    testTimer.reset();
}

// Teardown function runs after each test
void tearDown(void) {
    // Any cleanup code can go here
}

// Test case for the queue sequence
void test_buzzer_queue_sequence(void) {
    testTimer.start(4000);

    if (testTimer.hasElapsed()) {
        Serial.println("Clearing Queue for the test");
        buzzer.reset();
        testTimer.reset();
    }

    // Perform queue sequence only if buzzer has enough space for it
    if (buzzer.getQueueSize() > 1) {
        TEST_FAIL_MESSAGE("Buzzer queue does not have enough space");
    }

    // Queue up multiple buzzes
    bool beepResult = buzzer.beep(1000, 1100);
    if (beepResult) {
        Serial.println("Queued beep at 1 kHz");
    } else {
        TEST_FAIL_MESSAGE("Failed to queue beep at 1 kHz");
    }

    beepResult = buzzer.beep(300, 1500);
    if (beepResult) {
        Serial.println("Queued beep at 1.5 kHz");
    } else {
        TEST_FAIL_MESSAGE("Failed to queue beep at 1.5 kHz");
    }

    bool silentResult = buzzer.silent(2000);
    if (silentResult) {
        Serial.println("Queued 2 seconds of silence");
    } else {
        TEST_FAIL_MESSAGE("Failed to queue 2 seconds of silence");
    }

    beepResult = buzzer.beep(700, 1800);
    if (beepResult) {
        Serial.println("Queued beep at 1.8 kHz");
    } else {
        TEST_FAIL_MESSAGE("Failed to queue beep at 1.8 kHz");
    }
}

void setup() {
    // Initialize the Arduino framework
    delay(2000); // Delay to wait for the serial monitor to open

    // Start Unity test framework
    UNITY_BEGIN();

    // Run the test cases
    RUN_TEST(test_buzzer_queue_sequence);

    // Finish Unity test framework
    UNITY_END();
}

void loop() { buzzer.update(); }
