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
    Serial.begin(500000);

    imu.init();

    previousTime = 0;

    loopStartTime = -1;

    delay(2000);
    imu.setPollRate(7);

    float A_poll = imu.getAccelPollRate();
    float G_poll = imu.getGyroPollRate();

    Serial.print("Accel Poll Rate: ");
    Serial.println(A_poll);

    Serial.print("Gyro Poll Rate: ");
    Serial.println(G_poll);

    delay(1000);

    

}

// MAIN LOOP
void loop()
{
    Serial.println("loop");
    delay(2000);
    float pressure = baro.getPressure();
    float temp = baro.getTemperature();

    float* acc = imu.getAccelerometerData();
    float accX = acc[0];
    float accY = acc[1];
    float accZ = acc[2];

    float* gyro = imu.getGyroscopeData();
    float gyroX = gyro[0];
    float gyroY = gyro[1];
    float gyroZ = gyro[2];

    Serial.println(pressure);
    Serial.println(temp);
    Serial.println(accX);
    Serial.println(accY);
    Serial.println(accZ);
    Serial.println(gyroX);
    Serial.println(gyroY);
    Serial.println(gyroZ);


}




