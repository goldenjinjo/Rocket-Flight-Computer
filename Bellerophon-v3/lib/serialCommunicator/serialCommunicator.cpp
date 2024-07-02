#include "SerialCommunicator.hpp"

SerialCommunicator::SerialCommunicator(uint32_t baudRate, const char prefix, const char suffix)
    : baudRate(baudRate), prefix(prefix), suffix(suffix) {}

// Initializes serial communication with the specified baud rate
void SerialCommunicator::begin() {
    Serial.begin(baudRate);
}

// Sends a formatted message over serial by adding the prefix and suffix
void SerialCommunicator::sendSerialMessage(const char* message) {
    size_t prefixLen = strlen(prefix);
    size_t messageLen = strlen(message);
    size_t suffixLen = strlen(suffix);
    size_t totalLen = prefixLen + messageLen + suffixLen + 1; // +1 for null terminator

    char* formattedMessage = new char[totalLen];
    if (formattedMessage == nullptr) {
        return; // Return if memory allocation fails
    }

    // Construct the formatted message
    strcpy(formattedMessage, prefix);
    strcat(formattedMessage, message);
    strcat(formattedMessage, suffix);

    Serial.println(formattedMessage);

    delete[] formattedMessage; // Free the allocated memory
}

char* SerialCommunicator::readSerialMessage(int bufferSize) {
    // Allocate memory for the input buffer
    char* input = new char[bufferSize];
    if (input == nullptr) {
        return nullptr; // Return nullptr if memory allocation fails
    }

    // Read the input from Serial
    int len = Serial.readBytesUntil('\n', input, bufferSize - 1);
    input[len] = '\0'; // Null-terminate the C-style string

    // Trim leading/trailing whitespace
    char* trimmedInput = trimWhitespace(input);

    // Allocate memory for the trimmed input
    char* result = new char[strlen(trimmedInput) + 1];
    if (result == nullptr) {
        delete[] input;
        return nullptr; // Return nullptr if memory allocation fails
    }

    strcpy(result, trimmedInput);
    delete[] input; // Free the original input buffer

    Serial.println("Received Input: ");
    Serial.println(result);
    return result;
}


bool SerialCommunicator::waitForMessage(const char* expectedMessage, uint32_t timeout) {
    // Record the start time to track the timeout period
    uint32_t startTime = millis();

    // Loop until the timeout period elapses
    while (millis() - startTime < timeout) {
        // Check if there is data available on the Serial port
        if (Serial.available()) {
            const int bufferSize = 100; // Define a buffer size large enough for your input
            // Read the serial message, which trims whitespace and returns the message
            char* message = readSerialMessage(bufferSize);

            // Ensure the message is not null
            if (message != nullptr) {
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
        }
    }

    // Return false if the expected message is not received within the timeout period
    return false;
}


// create functionality for switching between serial modes
void SerialCommunicator::checkSerialForMode() {
  // Check for mode change command from serial input
  if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim(); // Remove any leading/trailing whitespace

      /// TODO: fix magic numbers, iterate over a mode array instead
      if (input.startsWith("mode:")) {
          char newMode = input.charAt(5); // Get the mode character
          if (newMode >= '0' && newMode <= '5') {
              mode = newMode - '0';  // Convert char to int
          }
      }
  }
}

/*
UTILS
*/

bool SerialCommunicator::readMessageWithPrefixSuffix(char* buffer, int bufferSize) {
    if (Serial.available() > 0) {
        char c = Serial.read();
        Serial.print("Read char: ");
        Serial.println(c);

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
                    Serial.print("Buffer: ");
                    Serial.println(buffer);
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