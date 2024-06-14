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

// Reads messages from the serial buffer, checking for the prefix and suffix
String SerialCommunicator::readSerialMessage() {
    while (Serial.available()) {
        // Read all available characters
        char c = (char)Serial.read();
        buffer += c;

        // Check if the buffer contains a complete message
        int suffixIndex = buffer.indexOf(suffix);
        if (suffixIndex != -1) {
            // Extract the complete message
            String message = buffer.substring(0, suffixIndex + suffix.length());

            // Remove the complete message from the buffer
            buffer = buffer.substring(suffixIndex + suffix.length());

            // Check if the message starts with the prefix and ends with the suffix
            if (message.startsWith(prefix) && message.endsWith(suffix)) {
                // Remove the prefix and suffix
                return message.substring(prefix.length(), message.length() - suffix.length());
            }
        }
    }
    // Return an empty string if no valid message is received
    return "";
}

// Waits for a specific message with a timeout
bool SerialCommunicator::waitForMessage(const String& expectedMessage, uint32_t timeout) {
    uint32_t startTime = millis();
   
    // Continue checking until the timeout period has elapsed
    while (millis() - startTime < timeout) {
        String message = readSerialMessage();
        // Return true if the expected message is received
        if (message == expectedMessage) {
            return true;
        }
    }
    // Return false if the expected message is not received within the timeout
    return false;
}
