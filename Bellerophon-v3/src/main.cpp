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
#include "serialCommunicator.hpp"
#include "fileManager.hpp"
#include "configFileManager.hpp"
#include "configKeys.hpp"
#include "constants.hpp"
#include "serialAction.hpp"
#include "pyroController.hpp"
#include "LEDController.hpp"
#include "buzzerFunctions.hpp"
#include "LEDManager.hpp"
#include "flightStateMachine.hpp"

size_t buzzerQueueLimit = 20;
// Class Declarations
BuzzerController buzzer(BUZZER, buzzerQueueLimit);
BuzzerFunctions buzzerFunc(buzzer);
LEDManager LED;

SerialCommunicator serialComm(BAUD_RATE, PREFIX, SUFFIX, buzzerFunc);
FileManager fm;
PositionalServo controlFins;
DataLogger logger(serialComm, fm);
ConfigFileManager config(fm);
SerialAction serialAction(serialComm, config, logger, controlFins, buzzerFunc, LED);

Timer testTimer;
FlightStateMachine flightState(buzzerFunc, logger);

void setup() {

    Wire.begin(); // Join i2c bus
    serialComm.begin();
    // initilize classes
    fm.initialize();
    // Config must be initalised first after FileManager, 
    //as it declares all external variables, including debug
    config.initialize();
    logger.initialize();
    controlFins.initialize();
    // play start up sequence
    LED.startUp();
    buzzerFunc.startUp();

}
// keep track of previous tones
int previousMode = -1;  
int time1 = 0;
int time2 = 0;

// MAIN LOOP

void loop()
{
    // Read serial monitor and change mode if input is given as:
    // mode:MODE_NUM
    serialAction.checkSerialForMode();
    // Play a tone to indicate mode of opera tion
    if (mode != previousMode) {
        buzzerFunc.modeSelect(mode);
        previousMode = mode;
    }

    buzzerFunc.update();
    LED.updateAllLEDS();
   
    flightState.update();

    switch (mode) {
        
        case STANDBY_MODE: {
            // do nothing
            LED.cycleLEDs(5000);
            //controlFins.continuousDeflect(250, 15);
            break;
        }    
        case READING_MODE: {
            // communicate with python serial to download flash data
            serialAction.serialFileTransfer();
            break;
        } 
        case PURGE_MODE: {
            // delete all files from flash memory
            serialAction.purgeDataFromSerial();
            break;
        }
        case LOGGING_MODE: {
            // log data to data file
            flightState.logSensorData(1000);
            break;
        }
        case FIN_CONTROL_MODE: {
            serialAction.moveServosFromSerial();
            break;
        }
        case CONFIG_MODE: {
            serialAction.processAndChangeConfig();
            break;
        }
    }
}