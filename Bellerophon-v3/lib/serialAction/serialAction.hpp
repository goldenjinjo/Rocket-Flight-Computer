#ifndef SERIAL_ACTION_HPP
#define SERIAL_ACTION_HPP

#include "serialCommunicator.hpp"
#include "constants.hpp"
#include "configFileManager.hpp"
#include "positionalServo.hpp"
#include "dataLogger.hpp"
#include "buzzerFunctions.hpp"
#include <cstring>

/**
 * @class SerialAction
 * @brief Class to perform Serial Actions for communication across the serial platform.
 */
class SerialAction {
public:
    /**
     * @brief Constructor for SerialAction.
     * @param communicator A reference to an instance of SerialCommunicator to be used for serial actions.
     */
    SerialAction(SerialCommunicator& communicator, ConfigFileManager& config, DataLogger& logger, PositionalServo& servo, BuzzerFunctions& buzzer);

    /**
     * @brief Method to check and switch between serial modes based on the received message.
     */
    void checkSerialForMode();

     /**
     * @brief Method to process serial input and change configuration values based on the received message.
     * MESSAGE STRUCTURE: CONFIG_NAME:VALUE
     */
    void processAndChangeConfig();

    /**
     * @brief Moves the servos to specified positions based on serial input.
     * The expected input format is any combination of commands: "A90", "D30 B45", "A90 C120", etc.
     * where the letter represents the servo and the number represents the position.
     */
    void moveServosFromSerial();

    /**
     * @brief  Handles the serial file transfer process.
     */
    void serialFileTransfer();

    /**
     * @brief Purges all data based on a serial command confirmation.
     *        If the DELETE_FILE_MESSAGE is confirmed via serial communication, all files are deleted.
     *        If the confirmation fails, the operation is aborted.
     */
    void purgeDataFromSerial();

private:

    /**
     * @brief Method to handle serial commands and change configuration values.
     * @param command The received command to handle.
     * @return True if the command was successfully handled and changed a configuration value; otherwise false.
     */
    bool changeConfigValue(const char* command);

    /**
     * @brief Moves the servo to a new position and updates the configuration.
     *
     * This method moves the specified servo to a new position relative to its current center position.
     * It updates the configuration file with the new center position of the servo.
     *
     * @param servoID The character representing the servo ID.
     * @param position The new position to move the servo to, relative to its current center position.
     */
    void moveServoandUpdateConfig(char servoID, int position);
    
    /**
     * @brief Processes a command to move servos based on the input string.
     *
     * This method parses the input string to extract servo commands, validates the servo IDs, and moves the
     * corresponding servos to the specified positions. It handles multiple commands in a single input string.
     *
     * @param input The input string containing servo commands.
     */
    void processServoCommand(const char* input);


    /**
     * @brief Confirms a specific action by waiting for a designated serial message within a specified time period.
     * @param SERIAL_MESSAGE The expected message to confirm the action.
     * @return True if the expected message is received within the wait period, false otherwise.
     *         If the message is not received, the mode is set to 0 and a red LED blink is triggered.
     *         If the message is received, a green LED blink is triggered.
     */
    bool confirmAction(const char* SERIAL_MESSAGE);

    /**
     * @brief Checks if the input command is a cancel request.
     *
     * This method checks whether the provided input matches the predefined cancel message.
     * If the input is a cancel request, it performs necessary actions such as blinking the LED,
     * resetting the mode, and freeing the allocated memory for the input.
     *
     * @param input The input command to be checked.
     * @return true if the input is a cancel request, false otherwise.
 */
    bool checkForCancelRequest(const char* input);

    SerialCommunicator& communicator; ///< Reference to the SerialCommunicator instance
    ConfigFileManager& config; ///< Reference to the ConfigFileManager instance
    DataLogger& logger; //<Reference to DataLogger instance
    PositionalServo& servo; //<Reference to Servo instance
    BuzzerFunctions& buzzer; //<Reference to Buzzer instance to be used inside while loops

    // Time to wait for a message before cancelling a mode operation
    uint32_t modeActivationWaitPeriod = 1000 * 60 * 3; // 3 minutes
};

#endif // SERIAL_ACTION_HPP
