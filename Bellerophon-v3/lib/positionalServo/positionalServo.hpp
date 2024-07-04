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
        int centerPos;  // Center position for the servo
    };

    // Members

    // Map to hold Servo ID to ServoObject mapping
    std::unordered_map<char, ServoObject> servoMap;

    // Minimum and maximum positions for the servos
    const int minPos = 0;
    const int maxPos = 180;

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
     * @brief Stops the specified servo by moving it to the center position.
     * @param servoObj The servo object to be stopped.
     */
    void stop(ServoObject& servoObj);

    /**
     * @brief Finds the servo object by its ID.
     * @param id The identifier of the servo.
     * @return A pointer to the ServoObject if found, nullptr otherwise.
     */
    ServoObject* findServoByID(char id);

    /**
     * @brief Moves a servo to the specified position by its ID.
     * @param id The identifier of the servo to move.
     * @param position The position to move the servo to (0-180 degrees).
     */
    void moveServoByID(char id, int position);

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
     * @brief Moves the specified servo based on its relative center position.
     * Allows moving forward and backward from the center position.
     * @param id The identifier of the servo to move.
     * @param relativePosition The position relative to the center position.
     * Positive values move forward, negative values move backward.
     */
    void moveServoRelativeToCenter(char id, int relativePosition);

    /**
     * @brief Stops the specified servo by its ID.
     * @param id The identifier of the servo to stop.
     */
    void stopByID(char id);


    /**
     * @brief Ensures the position is within the defined bounds.
     * @param position The position to check.
     * @return The adjusted position within bounds.
     */
    int boundaryCheck(int position);

};

#endif // POSITIONAL_SERVO_HPP




