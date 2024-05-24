/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 */

// Import Libraries
#include "pinAssn.hpp"
#include "pressureSensor.hpp"
#include "IMUSensor.hpp"
#include "dataLogger.hpp"

#include <BasicLinearAlgebra.h>
#include "SparkFunMPL3115A2.h"


// Class Declarations
// set oversample rate (lower, faster)
pressureSensor baro(1);
// Change address to low or high based on PCB design
IMUSensor imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000);
DataLogger logger;


void setup() {

    Wire.begin(); // Join i2c bus
    Serial.begin(500000);
    
    if(DEBUG){
        delay(1000);
    }

    // Set pin types and configure LEDs
    peripheralInitialize();
  
    // play start up sequence
    startUp();

    // initilize classes
    logger.initialize();

    delay(2000);
    imu.setPollRate(10);    
}

// MAIN LOOP
void loop()
{
    switch (mode) {
           case STANDBY_MODE:
                // Standby mode     
                while (true) {
                    delay(1000);
                }
        
        case READING_MODE:
        // TODOS:
        // create struct for unique file types (log and data files for right now)
        // on python side organise system to differeniate between data and log files (with .txt and .csv)
        // on both sides set it up so it can handle multiple file transfers (upload both logs and flight data)
        // give file name to python, python will then check and if it has already been uploaded, it will not upload it again
            logger.scanFiles();
            logger.readDataFromFile(logger.logFileName);
            logger.readDataFromFile(logger.dataFileName);
            while (true) {
                // Infinite loop to prevent further execution
                delay(1000);
            }
            break;

        case PURGE_MODE:
            // delete all files from flash memory
            // TODO: add serial confirmation check
            logger.deleteAllFiles();
            while (true) {
                // Infinite loop to prevent further execution
                delay(1000);
            }
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
        default:
            // Handle invalid mode
            break;
    }
}




