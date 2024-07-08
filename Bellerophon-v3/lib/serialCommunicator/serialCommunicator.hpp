#ifndef SERIALCOMMUNICATOR_HPP
#define SERIALCOMMUNICATOR_HPP

#include <Arduino.h>
#include "configKeys.hpp"
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
    SerialCommunicator(uint32_t baudRate, const char prefix, const char suffix);

    ~SerialCommunicator(); // Destructor declaration

    /**
     * @brief Initializes serial communication.
     */
    void begin();

    /**
     * @brief Sends a formatted message over serial.
     * @param message The message to send.
     */
    void sendSerialMessage(const char* message);

    /**
     * @brief Waits for a specific message with a timeout.
     * @param expectedMessage The message to wait for.
     * @param timeout The timeout period in milliseconds.
     * @return True if the expected message is received within the timeout, false otherwise.
     */
    bool waitForMessage(const char* expectedMessage, uint32_t timeout);

    /**
     * @brief Reads a message from the serial buffer, trims whitespace, and returns the trimmed message.
     * @return A pointer to the trimmed C-style string. The caller is responsible for freeing the memory.
     */
    char* readSerialMessage();

    /**
     * @brief Trims leading and trailing whitespace from a C-style string.
     * @param input The input C-style string to be trimmed.
     * @return A pointer to the trimmed string.
     */
    char* trimWhitespace(char* input);

    /**
     * @brief Checks if the given message contains any whitespace characters.
     * @param message The message to check.
     * @return True if the message contains whitespace, false otherwise.
     */
    static bool containsWhiteSpace(char* message);

    /**
     * @brief Utility function to check if a message is null or empty.
     * @param message The message to check.
     * @return True if the message is null or empty; otherwise false.
     */
    static bool isNullOrEmpty(const char* message);

private:
    
    /*
        ||| PRIVATE MEMBERS |||
    */
    uint32_t baudRate;   ///< The baud rate for serial communication.
    const char prefix;   ///< The prefix for messages.
    const char suffix;   ///< The suffix for messages.
    const char* buffer;   ///< Buffer for storing incomplete messages.
    int index;
    bool prefixFound;
    char* input;
    int bufferSize = 100;


    /*
        ||| PRIVATE METHODS |||
    */

    /**
     * @brief Reads a message from the serial input with specified prefix and suffix.
     * @param buffer The buffer to store the read message.
     * @param bufferSize The size of the buffer.
     * @return True if a valid message is found, false otherwise.
     */
    bool readMessageWithPrefixSuffix(char* buffer, int bufferSize);

};

#endif // SERIALCOMMUNICATOR_HPP
