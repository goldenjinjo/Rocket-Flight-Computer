#ifndef SERIAL_ACTION_HPP
#define SERIAL_ACTION_HPP

#include "serialCommunicator.hpp"
#include "constants.hpp"
#include "configFileManager.hpp"
#include "positionalServo.hpp"
#include "dataLogger.hpp"
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
    SerialAction(SerialCommunicator& communicator, ConfigFileManager& config, DataLogger& logger);

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
     * @brief Confirms a specific action by waiting for a designated serial message within a specified time period.
     * @param SERIAL_MESSAGE The expected message to confirm the action.
     * @return True if the expected message is received within the wait period, false otherwise.
     *         If the message is not received, the mode is set to 0 and a red LED blink is triggered.
     *         If the message is received, a green LED blink is triggered.
     */
    bool confirmAction(const char* SERIAL_MESSAGE);


    SerialCommunicator& communicator; ///< Reference to the SerialCommunicator instance
    ConfigFileManager& config; ///< Reference to the ConfigFileManager instance
    DataLogger& logger; //<Reference to DataLogger instance

    // Time to wait for a message before cancelling a mode operation
    uint32_t modeActivationWaitPeriod = 1000 * 60 * 3; // 3 minutes
};

#endif // SERIAL_ACTION_HPP
