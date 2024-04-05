/*
 * Main File to be uploaded to the UNSW Rocketry Flight Computer
 * 
 * 
 * 
 */

// Import Libraries
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <Adafruit_Sensor.h>
#include "magnificence.hpp"
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "Adafruit_BNO055.h"
#include "initialisation.cpp"


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


  Wire1.begin(); // Join i2c bus
  Serial.begin(9600);

  // track number of successful activations
  int successCounter = 0;

  if (!sd.begin(CHIP_SELECT, SPI_SPEED)) {
    buzzerFailure();
  }

  // Delete old versions of file if deleteFile condition is true
  if (sd.exists(fileName) && deleteFile == true) {
    sd.remove(fileName);
  } 

    // create data file and start writing
    file.open(fileName, O_RDWR | O_CREAT | O_AT_END);

    // Format file to distingish between new data sets
    file.println("");
    file.println("-------------------------------------------------------------------------------------------------------");
    file.println("-------------------------START OF FLIGHT DATA FILE--------------------------");
    file.println("-------------------------------------------------------------------------------------------------------");
    file.println(" ");
    file.println("--------------------------BEGIN INITIALIZATION SEQUENCE-------------------------");
    file.println(" ");
    
    file.println("Bellerophon Online!");
    file.println("Flash Chip Found...");
    successCounter += 1;
  

 

  // Try to initialize!
  if(!bno.begin()) {
    file.println("BNO055 Not Dectected.");
    buzzerFailure();
  } else {
    file.println("BNO055 Found...");
    successCounter += 1;
  }

  if(!baro.begin(Wire1)) {
    file.println("MPL3115A2 Not Dectected.");
    buzzerFailure();
  } else {
    file.println("ICM20948 Found...");
    successCounter += 1;
    
  }

  // Communicate all sensors have activated
  if (successCounter == 3){
    buzzerSuccess();
    digitalWrite(B_LED, HIGH);
    delay(500);
    digitalWrite(B_LED, LOW);
    file.println("All Sensors Responding Nominally.");
  }

  //Configure the sensors
  //baro.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  baro.setOversampleRate(7); // Set Oversample to the recommended 128
  baro.enableEventFlags(); // Enable all three pressure and temp event flags 
  
  bno.setExtCrystalUse(true);
  //bno.setAxisRemap(0x05);
  //bno.write8(bno.BNO055_PAGE_ID_ADDR, 0x01);
  //bno.write8(bno.BNO055_ACCEL_DATA_X_LSB_ADDR, 00011110b);
  bno.set16GRange();

}

/// MAIN LOOP
void loop()
{

  // open file
  file.open(fileName, O_RDWR | O_CREAT | O_AT_END);

  // initalize arrays for iteration
  float pressureArray[meanCount];
  float altArray[meanCount];
  float accXArray[meanCount];
  float accYArray[meanCount];
  float accZArray[meanCount];
  float gyroXArray[meanCount];
  float gyroYArray[meanCount];
  float gyroZArray[meanCount];
  float magXArray[meanCount];
  float magYArray[meanCount];
  float magZArray[meanCount];
  float tempArray[meanCount];
  float gravityXArray[ meanCount];
  float gravityYArray[ meanCount];
  float gravityZArray[ meanCount];
  float eulerXArray[ meanCount];
  float eulerYArray[ meanCount];
  float eulerZArray[ meanCount];
  float linearAccelXArray[ meanCount];
  float linearAccelYArray[ meanCount];
  float linearAccelZArray[ meanCount];

  // record initial values
  initialisation();

  // record accelerometer and pressure data from before take-off
  while (startFlight == false){

      loopTime = (millis() - startTime);
      
      deltaTime = millis();
      // set a delay, only for pre flight, to save memory
      //delay(preFlightDelay);
    
      imu::Vector<3> accelPreFlight = bno.getVector(bno.VECTOR_ACCELEROMETER);
      float pressurePreFlight = baro.readPressure();
     
      /*
      // read data
      char buffer[100];
      sprintf(buffer,"%.0f , %.0f, %.2f , %.2f , %.2f, %.2f",loopTime, millis()-deltaTime, \
       pressurePreFlight, accelPreFlight[0], accelPreFlight[1], accelPreFlight[2]);
      file.println(buffer);
      */

      // check  barometer and accelerometer for sensor indication of flight start
      if( abs(accelPreFlight[0]) > accThreshold || (pressureInit - pressurePreFlight) > pressureChangeThreshold) {
        // stops while loop from triggering again
        startFlight = true;
        digitalWrite(R_LED, LOW);
        digitalWrite(G_LED, HIGH);
        file.println("-------------------------------------------------------");
        file.println("------------------- LAUNCH DETECTED -------------------");
        file.println("-------------------------------------------------------");
        // new title for new section of CSV file
        file.print("Time (ms), deltaT (ms), Pressure (Pa), AccX, AccY, AccZ, Temp (C), rX', rY', rZ, magX, magY, magZ");
        file.println(", gravityX, gravityY, gravityZ EulerX, EulerY, EulerZ, LinearAccelX, LinearAccelY, LinearAccelZ");
        // stop while loop and move on to powered ascent stage of flight
        break;
      }
    }

  if (startFlight == true && endFlight == false) {
    
    for(int i = 0; i<meanCount ; i++) {

      deltaTime = millis();
      imu::Vector<3> gyro = bno.getVector(bno.VECTOR_GYROSCOPE);
      imu::Vector<3> mag = bno.getVector(bno.VECTOR_MAGNETOMETER);
      imu::Vector<3> accel = bno.getVector(bno.VECTOR_ACCELEROMETER);
      imu::Vector<3> gravity = bno.getVector(bno.VECTOR_GRAVITY);
      imu::Vector<3> euler = bno.getVector(bno.VECTOR_EULER);
      imu::Vector<3> linearAccel = bno.getVector(bno.VECTOR_LINEARACCEL);

      pressureArray[i] = baro.readPressure();
      //altArray[i] = baro.readAltitude() - altInit;
      
      accXArray[i] = accel[0];
      accYArray[i] = accel[1];
      accZArray[i] = accel[2];
      gyroXArray[i] = gyro[0];
      gyroYArray[i] = gyro[1];
      gyroZArray[i] = gyro[2];
      magXArray[i] = mag[0];
      magYArray[i] = mag[1];
      magZArray[i] = mag[2];
      tempArray[i] = bno.getTemp();
      gravityXArray[i] = gravity[0];
      gravityYArray[i] = gravity[1];
      gravityZArray[i] = gravity[2];
      eulerXArray[i] = euler[0];
      eulerYArray[i] = euler[1];
      eulerZArray[i] = euler[2];
      linearAccelXArray[i] = linearAccel[0];
      linearAccelYArray[i] = linearAccel[1];
      linearAccelZArray[i] = linearAccel[2];

      pressure += pressureArray[i];
      //alt += altArray[i];
      
      /*
      gyroX += gyroXArray[i];
      gyroY += gyroYArray[i];
      gyroZ += gyroZArray[i];
      magX += magXArray[i];
      magY += magYArray[i];
      magZ += magZArray[i];

      if (apoogeeReach == false && altArray[0] > altArray[meanCount]){
        // logic
        digitalWrite(PYRO_DROGUE, HIGH);
        int avgApogee = (altArray[0] + altArray[meanCount])/2;          
        file.print("APOGEE DETECTED");
        file.println(avgApogee);
        apoogeeReach = true;
      } 
      */

      // recording data in file
      loopTime = (millis() - startTime);
      char buffer[150];
      sprintf(buffer, "%.0f , %.0f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f, %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f , %.2f", \
      loopTime, millis()-deltaTime, pressureArray[i], accXArray[i],accYArray[i],accZArray[i], tempArray[i], \
      gyroXArray[i],gyroYArray[i], gyroZArray[i], magXArray[i], magYArray[i], magZArray[i], gravityXArray[i], \
      gravityYArray[i], gravityZArray[i], eulerXArray[i], eulerYArray[i], eulerZArray[i], linearAccelXArray[i], \
      linearAccelYArray[i], linearAccelZArray[i]);
      file.println(buffer);
      
    }
  
    pressure = pressure / (meanCount);
    //alt = alt / meanCount;
    /*
    gyroX = gyroX / meanCount + offSetGyroX;
    gyroY = gyroY / meanCount;
    gyroZ = gyroZ / meanCount;
    magX = magX / meanCount;
    magY = magY / meanCount;
    magZ = magZ / meanCount;
    accY = accY / meanCount;
    accX = accX / meanCount;
    accZ = accZ / meanCount;
    */

     // should be pressureChangeThreshold
    if((pressureInit - pressure) > pressureChangeThreshold && altLimitReach == false){
      altLimitReach = true;
      file.println("------------------------------------------------------------------------------");
      file.println("THRESHOLD ALTITUDE REACHED = 100 M");
      file.println("------------------------------------------------------------------------------");
    }



    if (apoogeeReach == false && pressureArray[0] < pressureArray[meanCount-1] && altLimitReach == true){
      // logic
      
      // PYROS deactivated in this version
      //digitalWrite(PYRO_DROGUE, HIGH);
      auto avgApogee = (pressureArray[0] + pressureArray[meanCount-1])/2;          
      file.println("------------------------------------------------------------------------------");
      file.print("APOGEE DETECTED: ");
      file.print(avgApogee);
      file.println(" Pa");
      file.println("------------------------------------------------------------------------------");
      apoogeeReach = true;
    }
    
    //// END OF FLIGHT LOGIC ///////////
    // Checks that the rocket has flown above maxAltThreshold before it will allow end of flight
    
   

    // starts iterating a count when rocket is measured to be below minAltThreshold
    // changing to pressure Change for now
    if( abs(pressureInit - pressure) < landingPressureThreshold && altLimitReach == true){
      stationaryCount +=1;
    }

    // Triggers end of flight of count is above the int endThreshold
  
    if( stationaryCount > endThreshold && endFlight == false){
      endFlight = true;
      digitalWrite(G_LED, LOW);
      file.println("------------------------------------------------------");
      file.println(" END OF FLIGHT DETECTED ");
      file.println("------------------------------------------------------");
    }

    pressure = 0;

  }
  
  if (endFlight == true){
    tone(BUZZER, 500);
    delay(1000);
    noTone(BUZZER);
  }

  // close file at end of every loop to save data
  file.close();
}