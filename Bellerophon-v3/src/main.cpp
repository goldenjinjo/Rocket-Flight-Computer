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


// Class Declarations

DataLogger logger;
PositionalServo controlFins;



void setup() {

    Wire.begin(); // Join i2c bus
    Serial.begin(2000000);

    // Set pin types and configure LEDs
    peripheralInitialize();
  
    // play start up sequence
    startUp();

    // initilize classes
    logger.initialize();
     

}
// keep track of previous tones
int previousMode = -1;  
// MAIN LOOP
void loop()
{
    // Read serial monitor and change mode if input is given as:
    // mode:MODE_NUM
    /// TODO: create universal serial interface in python. There are timing issues with this configuration
    checkSerialforMode();

    // Play a tone to indicate mode of operation
    if (mode != previousMode) {
        buzzerModeSelect(mode);
        previousMode = mode;
    }

    switch (mode) {
        case STANDBY_MODE: {
            // Cycle through all LEDS in 1 second flashes
            // cycleLEDS(500);
            delay(500);
            break;
        }    
        case READING_MODE: {
            // communicate with python serial to download flash data
            delay(100);
            logger.serialFileTransfer();
            break;
        } 
        case PURGE_MODE: {
            // delete all files from flash memory
            /// TODO: add serial confirmation check
            /// TODO: have this do nothing if there are already zero files, or maybe move to standby mode
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




