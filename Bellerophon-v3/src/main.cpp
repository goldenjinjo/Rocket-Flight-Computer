/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 * 
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
    
    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);
  
    pinMode(BUZZER, OUTPUT);

    // Set LEDS to low in case previous program left it HIGH
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, LOW);
  
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

    Serial.print("Accel Poll Rate: ");
    Serial.println(A_poll);

    Serial.print("Gyro Poll Rate: ");
    Serial.println(G_poll);
}

// MAIN LOOP
void loop()
{
    if(DEBUG){
        delay(2000);
    }
   
    // float pressure = baro.getPressure();
    // float temp = baro.getTemperature();

    float* acc = imu.getAccelerometerData();
    float* gyro = imu.getGyroscopeData();

    logger.logData(millis(),acc,3);
    logger.logData(millis(), gyro,3);
}




