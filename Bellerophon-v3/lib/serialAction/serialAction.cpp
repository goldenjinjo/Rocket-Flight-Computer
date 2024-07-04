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

        if(strcmp(input, CANCEL_MSG_REQUEST) == 0) {
            delete[] input; // Free the memory allocated for the message
            mode = 0;
            LEDBlink(B_LED, 1000);
            return;
        }

        if(strcmp(input, REQUEST_SETTINGS_INFO_MESSAGE) == 0) {
            printConfigKeysToSerial();
        }
        
        if (!changeConfigValue(input)) {
            Serial.println("Failed to handle serial command.");
            LEDBlink(R_LED, 1000);
            buzzerFailure();
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

void SerialAction::moveServosFromSerial() {
    // servo instance
    PositionalServo servo;

    if(!confirmAction(MANUAL_SERVO_CONTROL_MESSAGE)){
        return;
    }

    while (true) {
        // Use communicator to read the serial message
        char* input = communicator.readSerialMessage();  // Assuming a buffer size of 100
        
        // If input is null or empty, continue to the next iteration
        if (communicator.isNullOrEmpty(input)) {
            delete[] input;
            continue;
        }

        // cancel out of servo control and return to standby
        if (strcmp(input, CANCEL_MSG_REQUEST) == 0) {
            LEDBlink(R_LED, 1000);
            mode = 0;
            delete[] input;  // Free the allocated memory
            return;
        }

        int len = strlen(input);
        int i = 0;

        // Loop through the input string
        while (i < len) {
            char servoID = input[i];
            int positionStart = ++i;

            // Find the end of the position number
            while (i < len && isdigit(input[i])) {
                i++;
            }

            // Extract and convert the position substring to an integer
            char positionStr[10];  // Assuming the position will not be longer than 9 digits
            strncpy(positionStr, &input[positionStart], i - positionStart);
            positionStr[i - positionStart] = '\0';
            int position = atoi(positionStr);

            if (DEBUG) {
                Serial.print("Servo ");
                Serial.print(servoID);
                Serial.print(": Moving to position ");
                Serial.println(position);
            }

            // Move the corresponding servo to the specified position
            servo.moveServoRelativeToCenter(servoID, position);

            // Skip any spaces between commands
            while (i < len && isspace(input[i])) {
                i++;
            }
        }

        delete[] input;  // Free the allocated memory
    }
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
