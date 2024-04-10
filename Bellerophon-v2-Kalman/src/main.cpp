/*
 * Main File to be uploaded to Bellerophon Flight Computer
 * 
 * 
 * 
 */

// Import Libraries
#include <Wire.h>
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <Adafruit_Sensor.h>
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "config.hpp"
#include <BasicLinearAlgebra.h>
#include <LSM6DSLSensor.h>
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "pressureSensor.hpp"

// declare sensors
pressureSensor baro(1);



//Create an instance of the objects

LSM6DSLSensor lsm(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW);
Adafruit_ICM20948 icm;

// flash memory
SdFs sd;
FsFile file;

int launch_time;

unsigned long loopStartTime;

unsigned long previousTime = 0;



void logFlightEvent(const char* message, const unsigned long time);
void logFlightData(const unsigned long time);

void setup() {
    //Currently Unused
    //startUpTime = millis();
    
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

    // track number of successful activations
    int successCounter = 0;

    if (!sd.begin(CHIP_SELECT, SPI_SPEED)) {
        buzzerFailure();
    }

    // Delete old versions of file if deleteFile condition is true
    if (deleteFile) {
        if (sd.exists(logFileName)) {
            sd.remove(logFileName);
        } 

        if (sd.exists(dataFileName)) {
            sd.remove(dataFileName);
        }
    }
    // create data file and start writing
    file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);

    // Format file to distingish between new data sets
    file.println("");
    file.println("-----FLIGHT LOG-----");
    file.println("");
    file.println("-----BEGIN INITIALIZATION SEQUENCE-----");
    file.println("");
  
    file.println("Bellerophon v3 Online!");
    file.println("Flash Chip Found...");
    successCounter++; 


    if (icm_enable == true){
        if(!icm.begin_I2C()){
            buzzerFailure();
            
        } else {
            
            Serial.println("ICM Found..");
            icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
        }
    }
    // Try to initialize!
    if (lsm.begin() == LSM6DSLStatusTypeDef::LSM6DSL_STATUS_OK) {
        Serial.println("LSM Found");
        lsm.Enable_X();
        lsm.Enable_G();
        lsm.Set_X_FS(16); // set to 16G
        lsm.Set_G_FS(1000);

        successCounter++;
    } else {    
        Serial.println("LSM NOT Found");
    }

  

 

    file.close();
    
    file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    file.println("time, temp, ax,ay,az,mag_x,mag_y,mag_z,gx,gy,gz");
    file.close();
    
    
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




// void logFlightEvent(const char* message, const unsigned long time) {
//     file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
//     file.print(time);
//     file.print(": ");
//     file.print(message);
//     file.println("");
//     file.close();
// }

// void logFlightData(const unsigned long time) {
        
//     file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    
//     file.print(time);
//     file.print(",");

//    // Get LSM IMU Sensor Data
//     int32_t lsm_acc[3];
//     int32_t lsm_gyro[3];
//     lsm.Get_X_Axes(lsm_acc);
//     lsm.Get_G_Axes(lsm_gyro);

//     file.print(lsm_acc[0]);
//     file.print(",");
//     file.print(lsm_acc[1]);
//     file.print(",");
//     file.print(lsm_acc[2]);
//     file.print(",");

//     file.print(lsm_gyro[0]);
//     file.print(",");
//     file.print(lsm_gyro[1]);
//     file.print(",");
//     file.print(lsm_gyro[2]);
//     file.print(",");

    
//     //Barometer Data
//     file.print(baro.readPressure());
//     file.print(",");
//     file.print(baro.readTemp());
    
    


//     if(icm_enable == true){
//         sensors_event_t accel;
//         sensors_event_t gyro;
//         sensors_event_t mag;
//         sensors_event_t temp;
//         icm.getEvent(&accel, &gyro, &temp, &mag);

//         float a_z = accel.acceleration.z;

//         if(abs(a_z) > LAUNCH_ACC_THRESHOLD){
//             Serial.println("launch detect");
//             int launch_time = millis();

//         }


//         file.print(",");
//         file.print(temp.temperature);
//         file.print(",");
//         file.print(accel.acceleration.x);
//         file.print(",");
//         file.print(accel.acceleration.z);
//         file.print(",");
//         file.print(accel.acceleration.z);
//         file.print(",");
//         file.print(mag.magnetic.x);
//         file.print(",");
//         file.print(mag.magnetic.y);
//         file.print(",");
//         file.print(mag.magnetic.z);
//         file.print(",");
//         file.print(gyro.gyro.x);
//         file.print(",");
//         file.print(gyro.gyro.y);
//         file.print(",");
//         file.print(gyro.gyro.z);
//         file.print(",");
//         file.println();
//     } else {
//         file.println("");
//     }

//     file.close();
// }