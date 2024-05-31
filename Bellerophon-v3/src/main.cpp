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
// set oversample rate (lower, faster)
pressureSensor baro(1);
// Change address to low or high based on PCB design
IMUSensor imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000);
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
    imu.setPollRate(10);  

}

// keep track of previous tones
int previousMode = -1;  
// MAIN LOOP
void loop()
{
    // Play a tone to indicate mode of operation
    if (mode != previousMode) {
        buzzerModeSelect(mode);
        previousMode = mode;
    }

    switch (mode) {
        case STANDBY_MODE:

            // Cycle through all LEDS in 1 second flashes
            // cycleLEDS(500);
            break;
            
        case READING_MODE:
            // communicate with python serial to download flash data
            logger.serialFileTransfer();
            break;
            
        case PURGE_MODE:
            // delete all files from flash memory
            // TODO: add serial confirmation check
            logger.deleteAllFiles();
            break;
        
        case LOGGING_MODE:
            if(DEBUG){
                delay(1000);
            }
            // get IMU data
            float* acc = imu.getAccelerometerData();
            float* gyro = imu.getGyroscopeData();

            float* sensorArray = new float[9];
            
            // put all sensors into sensor array
            // TODO: Abstract this to dataLogger class
            // TODO: improve speed. Last check, 13ms between loops. Way too slow.
            sensorArray[0] = baro.getPressure();
            sensorArray[1] = baro.getTemperature();  // Resolved by commenting out TDR bit check in SparkFun Lib. 
            sensorArray[2] = acc[0];
            sensorArray[3] = acc[1];
            sensorArray[4] = acc[2];
            sensorArray[5] = acc[3];
            sensorArray[6] = gyro[1];
            sensorArray[7] = gyro[2];
            sensorArray[8] = gyro[3];
            // log sensor data
            logger.logData(sensorArray, 9);

            delete[] sensorArray; // Don't forget to free the allocated memory
            
            break;

        case FIN_CONTROL_MODE:
        // Move fins based on serial input
        controlFins.moveServosFromSerial();
        
        break;

        default:
            // Handle invalid mode
            break;
    }
}




