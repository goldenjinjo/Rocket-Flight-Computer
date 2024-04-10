/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 * 
 * 
 */

// Import Libraries
#include <SPI.h>
#include "sdios.h"
#include <Adafruit_Sensor.h>
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "config.hpp"
#include <BasicLinearAlgebra.h>
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"

#include "pressureSensor.hpp"
#include "IMUSensor.hpp"

// declare sensors
pressureSensor baro(1);
IMUSensor imuSensor(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000);

//Create an instance of the objects
Adafruit_ICM20948 icm;



int launch_time;

unsigned long loopStartTime;

unsigned long previousTime = 0;


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
    Serial.begin(9600);

   
    if (icm_enable == true){
        if(!icm.begin_I2C()){
            buzzerFailure();
            
        } else {
            
            Serial.println("ICM Found..");
            icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
        }
    }
    previousTime = 0;

    loopStartTime = -1;


    delay(10000);
}

// MAIN LOOP
void loop()
{
    unsigned long currentTime = millis();
    if (loopStartTime == -1) {
        loopStartTime = currentTime;
    }

    float pressure = baro.getPressure();
   
    //logFlightData(currentTime - loopStartTime);
    previousTime = currentTime;

}




