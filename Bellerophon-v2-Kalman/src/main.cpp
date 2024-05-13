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
#include "dataLogger.hpp"

// declare sensors
pressureSensor baro(1);
IMUSensor imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000);

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
    //startUp();


    Wire.begin(); // Join i2c bus
    Serial.begin(9600);

    imu.init();

    previousTime = 0;

    loopStartTime = -1;


    delay(1000);
}

// MAIN LOOP
void loop()
{
    delay(500);
  
    float pressure = baro.getPressure();
    float temp = baro.getTemperature();

    float* acc = imu.getAccelerometerData();
    float accX = acc[0];

    Serial.println(pressure);
    Serial.println(temp);
    
    // TODO: convert units to m/s
    Serial.println(accX);


}




