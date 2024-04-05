// Import Libraries
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <Adafruit_Sensor.h>
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "config.hpp"
#include "Adafruit_BNO055.h"
#include "constants.hpp"

//Create an instance of the objects
MPL3115A2 baro;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

#define SPI_SPEED SD_SCK_MHZ(4)
// flash memory
SdFs sd;
FsFile file;

void initialisation() {
  // Initialisation function

    if (recordInitial == true) {
    
    // Measure start of program to record time intervals
    float pressureInitArray[ calliMeasure ];
    float altInitArray[ calliMeasure ];
    float accXInitArray[ calliMeasure ];
    float accYInitArray[ calliMeasure ];
    float accZInitArray[ calliMeasure ];
    float gyroXInitArray[ calliMeasure ];
    float gyroYInitArray[ calliMeasure ];
    float gyroZInitArray[ calliMeasure ];
    float magXInitArray[ calliMeasure ];
    float magYInitArray[ calliMeasure ];
    float magZInitArray[ calliMeasure ];
  
    for(int i = 0; i<calliMeasure ; i++) {
      pressureInitArray[i] = baro.readPressure();
      altInitArray[i] = baro.readAltitude();

      imu::Vector<3> accel = bno.getVector(bno.VECTOR_ACCELEROMETER);
      imu::Vector<3> gyro = bno.getVector(bno.VECTOR_GYROSCOPE);
      imu::Vector<3> mag = bno.getVector(bno.VECTOR_MAGNETOMETER);
      accXInitArray[i] = accel[0];
      accYInitArray[i] = accel[1];
      accZInitArray[i] = accel[2];
      gyroXInitArray[i] = gyro[0];
      gyroYInitArray[i] = gyro[1];
      gyroZInitArray[i] = gyro[2];
      magXInitArray[i] = mag[0];
      magYInitArray[i] = mag[1];
      magZInitArray[i] = mag[2];

      pressureInit += pressureInitArray[i];
      altInit += altInitArray[i];
      accXInit += accXInitArray[i];
      accYInit += accYInitArray[i];
      accZInit += accZInitArray[i];
      gyroXInit += gyroXInitArray[i];
      gyroYInit += gyroYInitArray[i];
      gyroZInit += gyroZInitArray[i];
      magXInit += magXInitArray[i];
      magYInit += magYInitArray[i];
      magZInit += magZInitArray[i];
      
      delay(calliDelay);  
    }

    pressureInit = pressureInit / calliMeasure;
    altInit = altInit / calliMeasure;
    accXInit = accXInit / calliMeasure;
    accYInit = accYInit / calliMeasure;
    accZInit = accZInit / calliMeasure;
    gyroXInit = gyroXInit / calliMeasure;
    gyroYInit = gyroYInit / calliMeasure;
    gyroZInit = gyroZInit / calliMeasure;
    magXInit = magXInit / calliMeasure;
    magYInit = magYInit / calliMeasure;
    magZInit = magZInit / calliMeasure;
  
    // Record time taken to complete initilisation
    float initTime = (millis() - startTime );

    file.println("-------------------------RECORD STATIONARY VALUES------------------------");
    // column titles for CSV
    file.println("Time (ms), Pressure (Pa), AccX, AccY, AccZ, Alt (m), rX', rY', rZ', magX, magY, magZ");
    
    // recording data in file
    char buffer[100];
    sprintf(buffer, "%.0f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f, %.2f, %.2f", initTime,pressureInit, \
    altInit,accXInit,accYInit,accZInit,gyroXInit,gyroYInit,gyroZInit, magXInit, magYInit, magZInit);

    file.println(buffer);


    file.println("--------------------------------------------------------------------");
    file.println(" WAITING FOR FLIGHT START DETECTION ");
    // print data from before flight take off
    file.println("Time (ms), Pressure (Pa), AccX, AccY, AccZ, Alt (m), rX', rY', rZ', magX, magY, magZ");

    
    // physically indicate end of initisation
    buzzerSuccess();
    digitalWrite(G_LED, HIGH);
    delay(500);
    digitalWrite(G_LED, LOW);
    delay(500);
    
    // red to indicate stationary flight state
    digitalWrite(R_LED, HIGH);
    delay(500);

    // prevents initial measurements from being taken again
    recordInitial = false; 
  }
}

