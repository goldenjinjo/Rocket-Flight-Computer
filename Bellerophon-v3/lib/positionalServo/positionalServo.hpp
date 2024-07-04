#ifndef POSITIONAL_SERVO_HPP
#define POSITIONAL_SERVO_HPP

#include <Servo.h>
#include "pinAssn.hpp"
#include <array>
#include <unordered_map>
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
    // Map to hold Servo ID to ServoObject mapping
    std::unordered_map<char, ServoObject> servoMap;
    // Default position for servo
    uint8_t servoZeroValue = 0;
    // bool to determine while loop lock for servo control
    bool serialServoControlState = false;

public:
    /**
     * @brief Constructor for the PositionalServo class.
     * Initializes the servos and locks them into position.
     */
    PositionalServo();

     /**
     * @brief Destructor for the PositionalServo class.
     * Releases servos from active control
     */
    ~PositionalServo();

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
     * @brief Moves a servo to the specified position by its ID.
     * @param id The identifier of the servo to move.
     * @param position The position to move the servo to (0-180 degrees).
     */
    void moveServoByID(char id, int position);
};



#endif // POSITIONAL_SERVO_HPP



