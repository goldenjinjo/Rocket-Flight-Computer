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
    char* input = communicator.readSerialMessage();
    if (SerialCommunicator::isNullOrEmpty(input)) {
        delete[] input;
        return;
    }

    if (!changeConfigValue(input)) {
        Serial.println("Failed to handle serial command.");
    }
    delete[] input; // Free the allocated memory
}

bool SerialAction::changeConfigValue(const char* command) {
    // Check if the command is nullptr
    if (command == nullptr) {
        return false;
    }
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
        Serial.print("Failed to set ");
        Serial.print(key);
        Serial.println(" value.");
        return false;
    }

    return true;
}


void SerialAction::moveServosFromSerial() {

    // servo instance
    PositionalServo servo;

    bool serialServoControlState = false;
    
    
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        if (input == "start") {
            serialServoControlState = true;
            LEDBlink(G_LED, 500);
        }
    }

    while (serialServoControlState) {
        if (Serial.available()) {
            String input = Serial.readStringUntil('\n');
            input.trim(); // Remove any leading/trailing whitespace
            if (input == "end") {
                serialServoControlState = false;
                LEDBlink(R_LED, 1000);
                break;
            }

            int len = input.length();
            int i = 0;

            while (i < len) {
                char servoID = input[i];
                int positionStart = ++i;

                while (i < len && isDigit(input[i])) {
                    i++;
                }

                int position = input.substring(positionStart, i).toInt();

                if (DEBUG) {
                    Serial.print("Servo ");
                    Serial.print(servoID);
                    Serial.print(": Moving to position ");
                    Serial.println(position);
                }

                servo.moveServoByID(servoID, position);

                while (i < len && input[i] == ' ') {
                    i++;
                }
            }
        }
    }
}