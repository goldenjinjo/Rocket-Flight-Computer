/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 */

// Import Libraries
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "pressureSensor.hpp"
#include "IMUSensor.hpp"
#include "dataLogger.hpp"

#include <BasicLinearAlgebra.h>

// Class Declarations
pressureSensor baro(1);
IMUSensor imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000);
DataLogger logger(logFileName, dataFileName);

void setup() {
    
    // Set pin types and configure LEDs
    peripheralInitialize();
  
    // play start up sequence
    startUp();

    Wire.begin(); // Join i2c bus
    Serial.begin(500000);

    // initilize classes
    imu.init();
    logger.initialize();

    delay(2000);
    imu.setPollRate(7);

    float A_poll = imu.getAccelPollRate();
    float G_poll = imu.getGyroPollRate();
}

// MAIN LOOP
void loop()
{
    if(DEBUG){
        delay(2000);
    }

    float* sensorArray = new float[9];

    // get IMU data
    float* acc = imu.getAccelerometerData();
    float* gyro = imu.getGyroscopeData();

    // put all sensors into sensor array
    // TODO: Abstract this to dataLogger class
    sensorArray[0] = baro.getPressure();
    sensorArray[1] = baro.getTemperature(); // DEBUG why this is not working
    sensorArray[2] = acc[0];
    sensorArray[3] = acc[1];
    sensorArray[4] = acc[2];
    sensorArray[5] = acc[3];
    sensorArray[6] = gyro[1];
    sensorArray[7] = gyro[2];
    sensorArray[8] = gyro[3];

    // log sensor data
    logger.logData(millis(),sensorArray,9);
}




