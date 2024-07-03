#include "SerialCommunicator.hpp"

SerialCommunicator::SerialCommunicator(uint32_t baudRate, const char prefix, const char suffix)
    : baudRate(baudRate), prefix(prefix), suffix(suffix) {

    // Allocate memory for the input buffer
    input = new char[bufferSize];
    if (input == nullptr) {
        // Handle memory allocation failure
        Serial.println("Memory allocation failed");
    }
    }

// Initializes serial communication with the specified baud rate
void SerialCommunicator::begin() {
    Serial.begin(baudRate);
}

SerialCommunicator::~SerialCommunicator() {
    // Free the allocated memory
    delete[] input;
}

// Sends a formatted message over serial by adding the prefix and suffix
void SerialCommunicator::sendSerialMessage(const char* message) {
    size_t messageLen = strlen(message);
    size_t totalLen = 1 + messageLen + 1 + 1; // 1 for prefix, 1 for suffix, +1 for null terminator

    char* formattedMessage = new char[totalLen];
    if (formattedMessage == nullptr) {
        return; // Return if memory allocation fails
    }

    // Construct the formatted message
    formattedMessage[0] = prefix; // Add prefix
    strcpy(&formattedMessage[1], message); // Add the main message
    formattedMessage[1 + messageLen] = suffix; // Add suffix
    formattedMessage[1 + messageLen + 1] = '\0'; // Null-terminate the message

    Serial.println(formattedMessage);

    delete[] formattedMessage; // Free the allocated memory
}

char* SerialCommunicator::readSerialMessage() {
    if (!readMessageWithPrefixSuffix(input, bufferSize)) {
        return strdup(""); // Return empty string if no valid message is found yet
    }

    char* result = new char[strlen(input) + 1];
    if (result == nullptr) {
        return strdup(""); // Return empty string if memory allocation fails
    }

    strcpy(result, input);
    return result;
}

bool SerialCommunicator::waitForMessage(const char* expectedMessage, uint32_t timeout) {
    // Record the start time to track the timeout period
    uint32_t startTime = millis();

    // Loop until the timeout period elapses
    while (millis() - startTime < timeout) {
        // Read the serial message, which trims whitespace and returns the message
        char* message = readSerialMessage();

        // Compare the read message with the expected message
        if (strcmp(message, expectedMessage) == 0) {
            delete[] message; // Free the memory allocated for the message
            return true; // Return true if the expected message is received
        } 
        // Check if the received message is a cancel request
        else if (strcmp(message, CANCEL_MSG_REQUEST) == 0) {
            delete[] message; // Free the memory allocated for the message
            return false; // Return false if a cancel request is received
        }
        // Free the memory allocated for the message if it is not the expected message or a cancel request
        delete[] message;
        
    }
    // Return false if the expected message is not received within the timeout period
    return false;
}

// create functionality for switching between serial modes
void SerialCommunicator::checkSerialForMode() {
    // Call readSerialMessage to get the message
    char* message = readSerialMessage();
    
    if (!message || message[0] == '\0') { // Ensure message is not null and not empty
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

/*
UTILS
*/

bool SerialCommunicator::readMessageWithPrefixSuffix(char* buffer, int bufferSize) {
    if (Serial.available() > 0) {
        char c = Serial.read();
        // Serial.print("Read char: ");
        // Serial.println(c);

        if (!prefixFound) {
            if (c == prefix) {
                prefixFound = true;
                index = 0;  // Reset the index for the new message
            }
        } else {
            if (c == suffix) {
                buffer[index] = '\0';  // Null-terminate the message
                Serial.println(buffer);
                prefixFound = false;  // Reset for the next message
                return true;  // Valid message found
            } else {
                if (index < bufferSize - 1) {
                    buffer[index++] = c;
                    // Serial.print("Buffer: ");
                    // Serial.println(buffer);
                } else {
                    // Buffer overflow, reset the state
                    Serial.println("Buffer overflow");
                    prefixFound = false;
                    index = 0;
                }
            }
        }
    }
    return false;  // Valid message not yet found
}

char* SerialCommunicator::trimWhitespace(char* input) {
    if (input == nullptr) {
        return input; // Return nullptr if input is nullptr
    }

    // Trim leading whitespace
    char* trimmedInput = input;
    while (isspace(*trimmedInput)) {
        trimmedInput++;
    }

    // Trim trailing whitespace
    char* end = trimmedInput + strlen(trimmedInput) - 1;
    while (end >= trimmedInput && isspace(*end)) {
        *end = '\0';
        end--;
    }

    return trimmedInput;
}

bool SerialCommunicator::containsWhiteSpace(char* message) {
    while (*message) {
        if (isspace(*message)) {
            return true;
        }
        message++;
    }
    return false;
}
