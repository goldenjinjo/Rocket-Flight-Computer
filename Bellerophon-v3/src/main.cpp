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
#include "serialAction.hpp"
#include "pyroController.hpp"
#include "LEDController.hpp"
#include "buzzerFunctions.hpp"

size_t buzzerQueueLimit = 20;
// Class Declarations
BuzzerFunctions buzzerFunc(BUZZER, buzzerQueueLimit);
SerialCommunicator serialComm(BAUD_RATE, PREFIX, SUFFIX, buzzerFunc);
FileManager fm;
PositionalServo controlFins;
DataLogger logger(serialComm, fm);
ConfigFileManager config(fm);
SerialAction serialAction(serialComm, config, logger, controlFins, buzzerFunc);

// test instance of pyro class for drogue
PyroController drogue(PYRO_DROGUE, 2000);

LEDController greenLED(G_LED);
LEDController flashLED(FLASH_LED);
LEDController redLED(R_LED);


BuzzerController buzzer(BUZZER, buzzerQueueLimit);



Timer testTimer;

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

    ///TODO: set mode manager with getters and setters
   mode = BOOTUP_MODE;


}
// keep track of previous tones
int previousMode = -1;  
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

    greenLED.updateBlinkState();
    redLED.updateBlinkState();
    flashLED.updateBlinkState();
    buzzerFunc.update();

    switch (mode) {
        
        case STANDBY_MODE: {
            // do nothing
            greenLED.blink(1000);
            flashLED.blink(1500);


           
            // testTimer.start(4000);

            // if (testTimer.hasElapsed()) {
            //     Serial.println("Clearing Queue for the test");
            //     buzzer.reset();
            //     testTimer.reset();
            // }
            


            // // // only perform queue sequence if buzzer has enough space for it
            // if(buzzer.getQueueSize() > (1)) {
            //     break;
            // }
            // // // Queue up multiple buzzes
            // if (buzzer.beep(1000, 1100)) {
            //     Serial.println("Queued beep at 1 kHz");
            // }

            
            // if (buzzer.beep(300, 1500)) {
            //     Serial.println("Queued beep at 1.5 kHz");
            // }

            // if (buzzer.silent(2000)) {
            //     Serial.println("Queued 2 second of silences");
            // }
            // if (buzzer.beep(700, 1800)) {
            //     Serial.println("Queued beep at 1.5 kHz");
            // }
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
            logger.logData();
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
        case BARO_ONLY_FLIGHT_MODE: {
            // Testing pyro systems
            if(drogue.hasEverTriggered()){
                LEDBlink(B_LED, 500);
                delay(1000);
                break;
            }
            if(drogue.trigger()){
                LEDBlink(PRESSURE_LED, 500);
                delay(1000);
            }

            drogue.cancelTrigger();

            break;
        }
    }
}