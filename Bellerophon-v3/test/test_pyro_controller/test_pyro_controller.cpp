#include <unity.h>
#include "BuzzerController.hpp"
#include "pinAssn.hpp"
#include "pyroController.hpp"
#include "LEDManager.hpp"

// Create an instance of the pyro
uint16_t triggerDelay = 2000;
PyroController pyro(PYRO_DROGUE, triggerDelay);

// Instance of LEDs
LEDManager LED;

// Create an instance of the Timer
Timer testTimer;

// Setup function runs before each test
void setUp(void) {
    // Initialize the pyro
    pyro = PyroController(PYRO_DROGUE, 2000);
    testTimer.reset();
}

// Teardown function runs after each test
void tearDown(void) {
    // Any cleanup code can go here
}

// Test case for the pyro trigger sequence
void test_pyro_trigger(void) {
    bool triggered = false;
    unsigned long startTime = millis();

    while (!pyro.trigger()) {
        LED.blink(B_LED, 1000);
        // If it does not trigger after triggerDelay*2, it should fail
        if (millis() - startTime > triggerDelay * 2) {
            triggered = false;
            break;
        }
    }

    if (millis() - startTime <= triggerDelay * 2) {
        triggered = true;
    }

    TEST_ASSERT_TRUE_MESSAGE(triggered, "Pyro trigger failed within the expected delay.");

    delay(500);
    LED.blink(R_LED, 1000);
    delay(1000);

    // Assert that the while loop passed and the pyro was triggered
    TEST_ASSERT_TRUE_MESSAGE(pyro.hasEverTriggered(), "Pyro was not triggered.");
}

// Test case for the pyro cancel trigger sequence
void test_pyro_cancel_trigger(void) {
    pyro.trigger();
    pyro.cancelTrigger();

    delay(3000);

    // Assert that the pyro has not been triggered after cancellation
    TEST_ASSERT_FALSE_MESSAGE(pyro.hasEverTriggered(), "Pyro was triggered after cancellation.");
}

void setup() {
    // Initialize the Arduino framework
    delay(2000); // Delay to wait for the serial monitor to open

    // Start Unity test framework
    UNITY_BEGIN();

    // Run the test cases
    RUN_TEST(test_pyro_trigger);
    RUN_TEST(test_pyro_cancel_trigger);

    // Finish Unity test framework
    UNITY_END();
}

void loop() {
    LED.updateAllLEDS();
}
