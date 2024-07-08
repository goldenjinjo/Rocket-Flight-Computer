#include "serialAction.hpp"


SerialAction::SerialAction(SerialCommunicator& communicator, ConfigFileManager& config, 
DataLogger& logger, PositionalServo& servo)
 : communicator(communicator), config(config), logger(logger), servo(servo)  {}

void SerialAction::checkSerialForMode() {
    // Call readSerialMessage to get the message
    char* message = communicator.readSerialMessage();
    
    if(SerialCommunicator::isNullOrEmpty(message)){
        delete[] message;
        return;
    }

    // Check for mode change command from serial input
    if (strncmp(message, "mode:", 5) != 0) {
        delete[] message;
        return;
    }

    char newMode = message[5]; // Get the mode character
    if (newMode >= '0' && newMode < '0' + NUM_MODES) {
        mode = newMode - '0';  // Convert char to int
        Serial.print("Mode changed to: ");
        Serial.println(mode);
    } else {
        Serial.println("Invalid mode.");
    }

    delete[] message; // Free the allocated message buffer
}


void SerialAction::moveServosFromSerial() {

    if (!confirmAction(MANUAL_SERVO_CONTROL_MESSAGE)) {
        return;
    }
    // indicate start of servo control mode
    LEDBlink(G_LED, 500);

    while (true) {
        // Use communicator to read the serial message
        char* input = communicator.readSerialMessage();

        // If input is null or empty, continue to the next iteration
        if (communicator.isNullOrEmpty(input)) {
            delete[] input;
            continue;
        }

        // Cancel out of servo control and return to standby
        if(checkForCancelRequest(input)){
            return;
        }

        processServoCommand(input);

        delete[] input;  // Free the allocated memory
    }
}

void SerialAction::moveServoandUpdateConfig(char servoID, int position) {
    // Update the config for the SERVO_CHAR_CENTER_POSITION
    char configKey[30];
    snprintf(configKey, sizeof(configKey), "SERVO_%c_CENTER_POSITION", servoID);

    // Retrieve the old center position
    int oldCenterPos = static_cast<int>(config.getConfigValue(configKey));

    int newCenterPos = oldCenterPos + position;

    // Move the corresponding servo to the specified position
    servo.updateCenterPosition(servoID, newCenterPos);

    // Write the new center position to the config
    config.writeConfigValueFromString(configKey, newCenterPos);

    Serial.print("Moving ");
    Serial.print(newCenterPos - oldCenterPos);
    Serial.println(" degrees");
}

void SerialAction::processServoCommand(const char* input) {
    int len = strlen(input);
    int i = 0;

    while (i < len) {
        char servoID = input[i];
        if (!servo.isValidServoID(servoID)) {
            i++;
            continue;
        }
        int positionStart = ++i;

        if (i < len && input[i] == '-') {
            i++;
        }

        while (i < len && isdigit(input[i])) {
            i++;
        }

        char positionStr[10];
        strncpy(positionStr, &input[positionStart], i - positionStart);
        positionStr[i - positionStart] = '\0';
        int position = atoi(positionStr);

        moveServoandUpdateConfig(servoID, position);

        while (i < len && isspace(input[i])) {
            i++;
        }
    }
}

void SerialAction::serialFileTransfer() {
    
    // Check for incoming serial message, return to standby if not receieved
    if(!confirmAction(REQUEST_FILE_DOWNLOAD)){
        return;
    }
    // Send all files if correct message receieved
    LEDBlink(G_LED, 500);
    logger.sendAllFiles();

    // Send the end-of-transmission acknowledgment
    Serial.println(ALL_FILES_SENT);
   
    // Wait for confirmation
    if (!communicator.waitForMessage(ALL_FILES_SENT_ACK, modeActivationWaitPeriod)) {
        // Handle timeout (optional)
        buzzerFailure();
    } else {
        buzzerSuccess();
        LEDBlink(G_LED, 1000);
    }

    // return to standby
    mode = 0;
}

/// TODO: have this do nothing if there are already zero files, or maybe move to standby mode
/// TODO: create new new public method for serial based deletion, either individual files or all files and make this one private
void SerialAction::purgeDataFromSerial() {

    if(!confirmAction(DELETE_FILE_MESSAGE)){
        return;
    }
    logger.deleteAllFiles();
}

void SerialAction::processAndChangeConfig() {
    // Wait for unique message to confirm config mode
   if(!confirmAction(CHANGE_SETTINGS_MESSAGE)){
        return;
   }

    while(true) {

        char* input = communicator.readSerialMessage();

        if(SerialCommunicator::isNullOrEmpty(input)) {
            delete[] input;
            continue;
        }

        if(checkForCancelRequest(input)){
            return;
        }

        if(strcmp(input, REQUEST_SETTINGS_INFO_MESSAGE) == 0) {
            printConfigKeysToSerial();
            continue;
        }

         if(strcmp(input, RESET_CONFIG_MESSAGE) == 0) {
            config.restoreDefaults();
            continue;
        }
    
        if (!changeConfigValue(input)) {
            LEDBlink(R_LED, 1000);
        } else {
            LEDBlink(G_LED, 1000);
            buzzerSuccess();
        }
        
        delete[] input; // Free the allocated memory
    }
}

bool SerialAction::changeConfigValue(const char* command) {
   
    // Find the colon character to separate key and value
    const char* colonPos = strchr(command, ':');
    if (colonPos == nullptr) {
        return false;
    }

    // Extract the key and value strings
    size_t keyLength = colonPos - command;
    char key[keyLength + 1];
    strncpy(key, command, keyLength);
    key[keyLength] = '\0'; // Null-terminate the key string

    float value = atof(colonPos + 1);

    if (config.writeConfigValueFromString(key, value)) {
        Serial.print("Successfully set ");
        Serial.print(key);
        Serial.print(" to ");
        Serial.println(value);
    } else {
        // return false if invalid key
        return false;
    }

    return true;
}




/*
UTILS
*/
bool SerialAction::confirmAction(const char* SERIAL_MESSAGE) {
     if(!communicator.waitForMessage(SERIAL_MESSAGE, modeActivationWaitPeriod)){
        // Return false and change mode to 0 if message if expected message not
        // received
        LEDBlink(R_LED, 1000);
        mode = 0;
        return false;
    }
    // else return true
    LEDBlink(G_LED, 1000);
    return true;
}

bool SerialAction::checkForCancelRequest(const char* input) {
    if (strcmp(input, CANCEL_MSG_REQUEST) == 0) {
            LEDBlink(R_LED, 1000);
            mode = 0;
            delete[] input;  // Free the allocated memory
            return true;
        }
    return false;
}