#include <unity.h>
#include "BuzzerFunctions.hpp"
#include "pinAssn.hpp"

// Create an instance of BuzzerFunctions
BuzzerFunctions buzzerFunctions(BUZZER, 50);

// Setup function runs before each test
void setUp(void) {
    // Initialize the buzzer
    buzzerFunctions = BuzzerFunctions(BUZZER, 50);
}

// Teardown function runs after each test
void tearDown(void) {
    // Any cleanup code can go here
}

// Test case for the success sequence
void test_success_sequence(void) {
    buzzerFunctions.success();
    Serial.println("Running Success sequence");
    // Add assertions or checks to verify the expected behavior
    // For example, you might check the state of the buzzer after calling success()
}

// Test case for the failure sequence
void test_failure_sequence(void) {
    buzzerFunctions.failure();
    Serial.println("Running Failure sequence");
    // Add assertions or checks to verify the expected behavior
}

// Test case for the startup sequence
void test_startup_sequence(void) {
    buzzerFunctions.startUp();
    Serial.println("Running Startup sequence");
    // Add assertions or checks to verify the expected behavior
}

void test_silence(void) {
}

// Add more test cases for other methods as needed

void setup() {
    // Initialize the Arduino framework
    delay(5000); // Delay to wait for previous buzzer test to run through

    // Start Unity test framework
    UNITY_BEGIN();

    // Run the test cases
    RUN_TEST(test_success_sequence);
    delay(2000);
    RUN_TEST(test_failure_sequence);
    delay(2000);
    RUN_TEST(test_startup_sequence);

    // Finish Unity test framework
    UNITY_END();
}

void loop() {
    buzzerFunctions.update();
}
