#include <Servo.h>
#include "pinAssn.hpp"
#include <array>
#include <Arduino.h>
#include "configKeys.hpp"
#include "deviceFunctions.hpp"

class PositionalServo {
private:
    // Methods
    void initialize();

    struct ServoObject {
        Servo servo;
        int pin;
    };

    // Members
    // Array to hold Servo Objects
    std::array<ServoObject, 4> servos;
    // Default position for servo
    uint8_t servoZeroValue = 0;
    // bool to determine while loop lock for servo control
    bool serialServoControlState = false;

public:
    /**
     * @brief Constructor for the PositionalServo class.
     * Initializes the servos.
     */
    PositionalServo();

    /**
     * @brief Activates the specified servo by attaching it to its pin.
     * @param servoObj The servo object to be activated.
     */
    void activate(ServoObject& servoObj);

    /**
     * @brief Deactivates the specified servo by detaching it from its pin.
     * @param servoObj The servo object to be deactivated.
     */
    void deactivate(ServoObject& servoObj);

    /**
     * @brief Activates all servos by attaching them to their respective pins.
     */
    void activateAll();

    /**
     * @brief Deactivates all servos by detaching them from their respective pins.
     */
    void deactivateAll();

    /**
     * @brief Moves the specified servo to a given position.
     * @param servoObj The servo object to be moved.
     * @param position The position to move the servo to (0-180 degrees).
     */
    void move(ServoObject& servoObj, uint8_t position);

    /**
     * @brief Stops the specified servo by moving it to the zero position.
     * @param servoObj The servo object to be stopped.
     */
    void stop(ServoObject& servoObj);

    /**
     * @brief Moves the servos to specified positions based on serial input.
     * The expected input format is any combination of commands: "A90", "D30 B45", "A90 C120", etc.
     * where the letter represents the servo and the number represents the position.
     */
    void moveServosFromSerial();
};



