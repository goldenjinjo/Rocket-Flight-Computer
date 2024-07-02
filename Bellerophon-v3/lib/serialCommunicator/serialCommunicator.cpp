#include "SerialCommunicator.hpp"

SerialCommunicator::SerialCommunicator(uint32_t baudRate, const String& prefix, const String& suffix)
    : baudRate(baudRate), prefix(prefix), suffix(suffix), buffer("") {}

// Initializes serial communication with the specified baud rate
void SerialCommunicator::begin() {
    Serial.begin(baudRate);
}

// Sends a formatted message over serial by adding the prefix and suffix
void SerialCommunicator::sendSerialMessage(const String& message) {
    String formattedMessage = prefix + message + suffix;
    Serial.println(formattedMessage);
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

// Waits for a specific message with a timeout
bool SerialCommunicator::waitForMessage(const String& expectedMessage, uint32_t timeout) {
    uint32_t startTime = millis();
   
    // Continue checking until the timeout period has elapsed

    // NEW METHOD
    // while (millis() - startTime < timeout) {
    //     char* message = readSerialMessage();
    //     // Return true if the expected message is received
    //     if (message == expectedMessage) {
    //         return true;
    //     }
    // }

   // OLD METHOD
    while (millis() - startTime < timeout) {
        if (Serial.available()) {
            String message = Serial.readStringUntil('\n');
            if (message == expectedMessage) {
                return true;
            } else if (message == CANCEL_MSG_REQUEST)
                // return method early if told to cancel
                return false;
        }
    }
    // Return false if the expected message is not received within the timeout
    return false;
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