#ifndef POSITIONAL_SERVO_HPP
#define POSITIONAL_SERVO_HPP

#include <Servo.h>
#include "pinAssn.hpp"
#include <array>
#include <unordered_map>
#include <Arduino.h>
#include "configKeys.hpp"
#include "deviceFunctions.hpp"

/**
 * @class PositionalServo
 * @brief Manages multiple servos, allowing activation, deactivation, movement to specified positions,
 *        and movement relative to a center position. Includes boundary checks to ensure servo positions
 *        remain within safe limits.
 */
class PositionalServo {
private:
    /*
        PRIVATE MEMBERS
    */
    
    /**
     * @struct ServoObject
     * @brief Represents a servo with its associated pin and center position.
     */
    struct ServoObject {
        Servo servo;     ///< The servo object
        int pin;         ///< The pin to which the servo is attached
        int centerPos;   ///< The center position for the servo
    };

    /**
     * @brief Map to hold Servo ID to ServoObject mapping
     */
    std::unordered_map<char, ServoObject> servoMap;

    /**
     * @brief Minimum position for the servos
     */
    const int minPos = 0;

    /**
     * @brief Maximum position for the servos
     */
    const int maxPos = 180;

    /**
     * @brief Maximum angle servo can deflect from an initial position
     */
    const int maxDeflectionAngle = 30;

    /*
        PRIVATE METHODS
    */

    /**
     * @brief Moves a servo to the specified position by its ID.
     * @param id The identifier of the servo to move.
     * @param position The position to move the servo to (0-180 degrees).
     */
    void moveServoByID(char id, int position);

    /**
     * @brief Finds the servo object by its ID.
     * @param id The identifier of the servo.
     * @return A pointer to the ServoObject if found, nullptr otherwise.
     */
    ServoObject* findServoByID(char id);

    /**
     * @brief Stops the specified servo by moving it to the center position.
     * @param servoObj The servo object to be stopped.
     */
    void stop(ServoObject& servoObj);

    /**
     * @brief Ensures the position is within the defined bounds.
     * @param position The position to check,
     * passed by reference to allow modification.
     */
    void maxSetAngleCheck(int& position);

    /**
     * @brief Ensures the position is within the defined bounds.
     * @param position The position to check.
     */
    void boundaryCheck(int& position);

    /**
     * @brief Ensures the relative position does not exceed maximum deflection angles.
     * @param position The relative position to check.
     */
    void maxDeflectionCheck(int& position);

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

public:
    /*
        PUBLIC METHODS
    */

    /**
     * @brief Constructor for the PositionalServo class.
     * Initializes the servos and locks them into position.
     */
    PositionalServo();

    /**
     * @brief Initializes the servo map with predefined pins and center positions.
     */
    void initialize();

    /**
     * @brief Moves the specified servo based on its relative center position.
     * Allows moving forward and backward from the center position.
     * @param id The identifier of the servo to move.
     * @param relativePosition The position relative to the center position.
     * Positive values move forward, negative values move backward.
     */
    void moveServoRelativeToCenter(char id, int& relativePosition);

    /**
     * @brief Stops the specified servo by its ID.
     * @param id The identifier of the servo to stop.
     */
    void stopByID(char id);

    /**
     * @brief Resets all servo positions to their center positions.
     */
    void centerAllServoPositions();

    /**
     * @brief Updates the center position of a servo with the given ID
     * and moves it to the new position.
     * @param id The ID of the servo.
     * @param position The new center position to set, 
     * passed by reference to allow modification.
     */
    void updateCenterPosition(char id, int& position);

    /**
     * @brief Checks if the provided servo ID is valid.
     *
     * This method validates whether the given servo ID exists in the servoMap.
     * It helps in ensuring that operations are performed only on valid servo IDs.
     *
     * @param id The character representing the servo ID to be validated.
     * @return true if the servo ID is valid and exists in the servoMap, false otherwise.
     */
    bool isValidServoID(char id);

};

#endif // POSITIONAL_SERVO_HPP

