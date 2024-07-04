#include "serialAction.hpp"


SerialAction::SerialAction(SerialCommunicator& communicator, ConfigFileManager& config) : communicator(communicator), config(config) {}

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
    if (newMode >= '0' && newMode <= '5') {
        mode = newMode - '0';  // Convert char to int
        Serial.print("Mode changed to: ");
        Serial.println(mode);
    } else {
        Serial.println("Invalid mode.");
    }

    delete[] message; // Free the allocated message buffer
}

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

void SerialAction::processAndChangeConfig() {
    // Wait for unique message to confirm config mode
   if(!confirmAction(CHANGE_SETTINGS)){
        return;
   }

    while(true) {

        char* input = communicator.readSerialMessage();

        if(strcmp(input, CANCEL_MSG_REQUEST) == 0) {
            delete[] input; // Free the memory allocated for the message
            mode = 0;
            LEDBlink(B_LED, 1000);
            return;
        }
        
        if (!SerialCommunicator::isNullOrEmpty(input)) {
            if (!changeConfigValue(input)) {
                Serial.println("Failed to handle serial command.");
                LEDBlink(R_LED, 1000);
                buzzerFailure();
            } else {
                LEDBlink(G_LED, 1000);
                buzzerSuccess();
            }
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


void SerialAction::moveServosFromSerial() {

    // servo instance
    PositionalServo servo;

    bool serialServoControlState = false;
    
    // Use communicator to wait for the start message
    if (communicator.waitForMessage("start", 1000 * 60)) {
        serialServoControlState = true;
        LEDBlink(G_LED, 500);
    } else {
        mode = 0;
        return;
    }

    while (serialServoControlState) {
        // Use communicator to read the serial message
        char* input = communicator.readSerialMessage();  // Assuming a buffer size of 100
        
        // If input is null or empty, continue to the next iteration
        if (communicator.isNullOrEmpty(input)) {
            delete[] input;
            continue;
        }
        // Trim whitespace from input
        char* trimmedInput = communicator.trimWhitespace(input);

        // If the input command is "end", set the mode and return
        if (strcmp(trimmedInput, "end") == 0) {
            LEDBlink(R_LED, 1000);
            mode = 0;
            delete[] input;  // Free the allocated memory
            return;
        }

        int len = strlen(trimmedInput);
        int i = 0;

        // Loop through the input string
        while (i < len) {
            char servoID = trimmedInput[i];
            int positionStart = ++i;

            // Find the end of the position number
            while (i < len && isdigit(trimmedInput[i])) {
                i++;
            }

            // Extract and convert the position substring to an integer
            char positionStr[10];  // Assuming the position will not be longer than 9 digits
            strncpy(positionStr, &trimmedInput[positionStart], i - positionStart);
            positionStr[i - positionStart] = '\0';
            int position = atoi(positionStr);

            if (DEBUG) {
                Serial.print("Servo ");
                Serial.print(servoID);
                Serial.print(": Moving to position ");
                Serial.println(position);
            }

            // Move the corresponding servo to the specified position
            servo.moveServoByID(servoID, position);

            // Skip any spaces between commands
            while (i < len && isspace(trimmedInput[i])) {
                i++;
            }
        }

        delete[] input;  // Free the allocated memory
    }
}
