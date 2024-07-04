#include "serialAction.hpp"


SerialAction::SerialAction(SerialCommunicator& communicator) : communicator(communicator) {}

void SerialAction::checkSerialForMode() {
    // Call readSerialMessage to get the message
    char* message = communicator.readSerialMessage();
    
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