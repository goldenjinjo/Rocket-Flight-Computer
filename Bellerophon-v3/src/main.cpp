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

FileManager fm;
PositionalServo controlFins;
SerialCommunicator serialComm(BAUD_RATE, PREFIX, SUFFIX);
DataLogger logger(serialComm, fm);
ConfigFileManager config(fm);




void setup() {

    Wire.begin(); // Join i2c bus
    serialComm.begin();

    // Set pin types and configure LEDs
    peripheralInitialize();
    // initilize classes
    logger.initialize();
    config.initialize();

    // play start up sequence
    startUp();

    delay(1000);
    Serial.println("---");
    Serial.println(ALTITUDE_BUFFER_PERIOD);


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
        default: {
            // Handle invalid mode
            break;
        }
    }
}




