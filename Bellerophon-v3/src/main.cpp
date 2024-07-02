/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 */

// Import Libraries
#include "pinAssn.hpp"
#include "pressureSensor.hpp"
#include "IMUSensor.hpp"
#include "dataLogger.hpp"
#include "positionalServo.hpp"
#include <BasicLinearAlgebra.h>
#include "serialCommunicator.hpp"
#include "fileManager.hpp"
#include "configFileManager.hpp"
#include "configKeys.hpp"
#include "constants.hpp"

// Class Declarations

SerialCommunicator serialComm(BAUD_RATE, PREFIX, SUFFIX);
FileManager fm;
PositionalServo controlFins;
DataLogger logger(serialComm, fm);
ConfigFileManager config(fm);

void handleSerialCommand(const char* command);

void setup() {

    Wire.begin(); // Join i2c bus
    serialComm.begin();

    // Set pin types and configure LEDs
    peripheralInitialize();
    // initilize classes
    fm.initialize();
    // Config must be initalised first after FileManager, as it declares all external variables, including debug
    config.initialize();
    logger.initialize();
    // play start up sequence
    startUp();

    delay(1000);
   
    printConfigKeysToSerial();

}
// keep track of previous tones
int previousMode = -1;  
// MAIN LOOP
void loop()
{
    // Read serial monitor and change mode if input is given as:
    // mode:MODE_NUM
    /// TODO: create universal serial interface in python. There are timing issues with this configuration
    serialComm.checkSerialForMode();

    // Play a tone to indicate mode of operation
    if (mode != previousMode) {
        buzzerModeSelect(mode);
        previousMode = mode;
    }

    switch (mode) {
        case STANDBY_MODE: {
            break;
        }    
        case READING_MODE: {
            // communicate with python serial to download flash data
            logger.serialFileTransfer();
            break;
        } 
        case PURGE_MODE: {
            // delete all files from flash memory
            /// TODO: add serial confirmation check
            /// TODO: have this do nothing if there are already zero files, or maybe move to standby mode
            /// TODO: create new new public method for serial based deletion, either individual files or all files and make this one private
            logger.deleteAllFiles();
            break;
        }
        case LOGGING_MODE: {
            // log data to data file
            logger.logData();
            break;
        }
        case FIN_CONTROL_MODE: {
            // Move fins based on serial input
            // type "start" to enter mode, otherwise does nothing
            // type "end" to exit
            controlFins.moveServosFromSerial();
            break;
        }

        case CONFIG_MODE: {
            if (Serial.available()) {
                const int bufferSize = 100; // Define a buffer size large enough for your input
                char input[bufferSize];

                // Read the input from Serial
                int len = Serial.readBytesUntil('\n', input, bufferSize - 1);
                input[len] = '\0'; // Null-terminate the C-style string

                // Remove any leading/trailing whitespace (optional)
                char* trimmedInput = input;
                while (isspace(*trimmedInput)) trimmedInput++; // Trim leading whitespace
                for (char* end = trimmedInput + strlen(trimmedInput) - 1; end >= trimmedInput && isspace(*end); end--) {
                    *end = '\0'; // Trim trailing whitespace
                }

                handleSerialCommand(trimmedInput);
            }
            break;
        }
    }
}

void handleSerialCommand(const char* command) {
    // Find the colon character to separate key and value
    const char* colonPos = strchr(command, ':');
    if (colonPos == nullptr) {
        Serial.println("Invalid command format");
        return;
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
    }
}

