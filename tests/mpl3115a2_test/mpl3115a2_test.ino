#include <Wire.h>
#include "SparkFunMPL3115A2.h"

//Create an instance of the object
MPL3115A2 baro;


bool recordInitial = true;

float initialAltitude;
float callibration;

float startTime;

// values for callibration
int calliMeasure = 100;
float calliDelay = 50;
float calliTime = calliMeasure * calliDelay / 1000;

void setup()
{
  Wire1.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output

  baro.begin(Wire1); // Get sensor online

  //Configure the sensor
  baro.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  baro.setOversampleRate(7); // Set Oversample to the recommended 128
  baro.enableEventFlags(); // Enable all three pressure and temp event flags 
}

void loop()
{

    // Records initial values only during first iteration of the loop
  if (recordInitial == true) {

    Serial.println("---BEGIN INITIALIZATION---");

    startTime = millis();
    
    Serial.println("ZEROING MPL3115A2, by taking "+String(calliMeasure)+" measurements");

    Serial.println("Estimated completion time is "+String(calliTime)+" seconds");

    // Serial.println("Actual time taken was "+String((millis() - startTime) / 1000)+" seconds");
    
    callibration = getAltitudeAvg(100, 50);
  
    Serial.print("Initial Altitude = ");
    Serial.print(callibration);
    Serial.println(" meters");
  
    Serial.println("---END INITIALIZATION---");
    
    delay(10);

    // prevents initial measurements from being taken again
    recordInitial = false;
    return;
  
  }



float altitude = baro.readAltitude() - callibration;

Serial.print(" Altitude(m):");
Serial.print(altitude, 2);



Serial.println();

delay(100);
}


// FUNCTIOMS


float altitudeSum;

float getAltitudeAvg(int meanCount, int delayTime) {
  
  float altitudeArray[ meanCount ] = { };

  
  int i = 0;
  while (i < meanCount) {

    altitudeArray[i] = baro.readAltitude();
    

    altitudeSum += altitudeArray[i];
    
    i++;

    delay(delayTime);

    }

    initialAltitude = altitudeSum / meanCount;

    return initialAltitude;
}
