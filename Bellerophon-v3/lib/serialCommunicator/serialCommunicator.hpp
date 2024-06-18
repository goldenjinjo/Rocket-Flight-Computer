#ifndef SERIALCOMMUNICATOR_HPP
#define SERIALCOMMUNICATOR_HPP

#include <Arduino.h>
#include "config.hpp"
#include "constants.hpp"

/**
 * @class SerialCommunicator
 * @brief Handles serial communication with formatted messages using prefix and suffix.
 */
class SerialCommunicator {
public:
    /**
     * @brief Constructor for SerialCommunicator.
     * @param baudRate The baud rate for serial communication.
     * @param prefix The prefix for messages.
     * @param suffix The suffix for messages.
     */
    SerialCommunicator(uint32_t baudRate, const String& prefix, const String& suffix);

    /**
     * @brief Initializes serial communication.
     */
    void begin();

    /**
     * @brief Sends a formatted message over serial.
     * @param message The message to send.
     */
    void sendSerialMessage(const String& message);

    /**
     * @brief Waits for a specific message with a timeout.
     * @param expectedMessage The message to wait for.
     * @param timeout The timeout period in milliseconds.
     * @return True if the expected message is received within the timeout, false otherwise.
     */
    bool waitForMessage(const String& expectedMessage, uint32_t timeout);


    void checkSerialForMode();

private:
    uint32_t baudRate;   ///< The baud rate for serial communication.
    String prefix;   ///< The prefix for messages.
    String suffix;   ///< The suffix for messages.
    String buffer;   ///< Buffer for storing incomplete messages.

    /**
     * @brief Reads messages from serial, checking for the prefix and suffix.
     * @return The cleaned message without prefix and suffix, or an empty string if invalid.
     */
    String readSerialMessage();
};

#endif // SERIALCOMMUNICATOR_HPP
