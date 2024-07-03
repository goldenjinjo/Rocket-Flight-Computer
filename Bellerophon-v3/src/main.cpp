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

// Function to handle serial commands
void processSerialInput() {
    char* input = serialComm.readSerialMessage();
    if (!input || input[0] == '\0') {
        delete[] input;
        return;
    }

    handleSerialCommand(input);
    delete[] input; // Free the allocated memory
}

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

    mode = 5;
   
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
    // Play a tone to indicate mode of opera tion
    if (mode != previousMode) {
        buzzerModeSelect(mode);
        previousMode = mode;
    }

    switch (mode) {
        
        case STANDBY_MODE: {
            char* input = serialComm.readSerialMessage();
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
            ///TODO: fix this as it currently breaks when coupled with checkSerialForMode
            // it literally crashes...
            //char* input = serialComm.readSerialMessage(100);
            //char* test = serialComm.readSerialMessage(bufferSize);
          

            break;

            //serialComm.readMessageWithPrefixSuffix(message, bufferSize);

            // if(!strcmp(input, "") == 0) {
            //     Serial.println("test: ");
            //     Serial.println(input);
            //     delete[] input;
            //     delay(1000);
            // } 
                // if (strcmp(input, "CANCEL") == 0){
                //     cancel_flag = true;
                //     mode = 0;
                // }
            
            //handleSerialCommand(input);
        }
    }
}

void handleSerialCommand(const char* command) {
    // Check if the command is nullptr
    if (command == nullptr) {
        return;
    }
    // Find the colon character to separate key and value
    const char* colonPos = strchr(command, ':');
    if (colonPos == nullptr) {
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

