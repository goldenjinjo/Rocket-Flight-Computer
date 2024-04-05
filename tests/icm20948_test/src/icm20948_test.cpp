// Basic demo for accelerometer readings from Adafruit ICM20948
//demo code is from this URL, follow instructions on it to install requisete packages
//https://learn.adafruit.com/adafruit-tdk-invensense-icm-20948-9-dof-imu/arduino

#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "simpsons4.h"
#include <vector>
using namespace std;

Adafruit_ICM20948 icm;
uint16_t measurement_delay_us = 65535; // Delay between measurements for testing

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit ICM20948 test!");

  // Try to initialize!
  if (!icm.begin_I2C()) {
    // if (!icm.begin_SPI(ICM_CS)) {
    // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {

    Serial.println("Failed to find ICM20948 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("ICM20948 Found!");
  // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
  Serial.print("Accelerometer range set to: ");
  switch (icm.getAccelRange()) {
  case ICM20948_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case ICM20948_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case ICM20948_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case ICM20948_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  Serial.println("OK");

  // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
  Serial.print("Gyro range set to: ");
  switch (icm.getGyroRange()) {
  case ICM20948_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case ICM20948_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  }

  //  icm.setAccelRateDivisor(4095);
  uint16_t accel_divisor = icm.getAccelRateDivisor();
  float accel_rate = 1125 / (1.0 + accel_divisor);

  Serial.print("Accelerometer data rate divisor set to: ");
  Serial.println(accel_divisor);
  Serial.print("Accelerometer data rate (Hz) is approximately: ");
  Serial.println(accel_rate);

  //  icm.setGyroRateDivisor(255);
  uint8_t gyro_divisor = icm.getGyroRateDivisor();
  float gyro_rate = 1100 / (1.0 + gyro_divisor);

  Serial.print("Gyro data rate divisor set to: ");
  Serial.println(gyro_divisor);
  Serial.print("Gyro data rate (Hz) is approximately: ");
  Serial.println(gyro_rate);

}

float yaw = 0;
float pitch = 0;
float roll = 0;

//adds new value to start of array and shifts all other values left, end value of array is lost
template <typename T>
  
void updateVect(vector<T> vect, T newVal)
{
    vect.insert(vect.begin(),newVal);
    vect.pop_back();  
}

void loop() {

  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t mag;
  sensors_event_t temp;
  icm.getEvent(&accel, &gyro, &temp, &mag);

  //Serial.print("\t\tTemperature ");
  //Serial.print(temp.temperature);
  //Serial.println(" deg C");
  /*
  // Display the results (acceleration is measured in m/s^2)
  Serial.print("\t\tAccel X: ");
  Serial.print(accel.acceleration.x, 5);
  Serial.print(" \tY: ");
  Serial.print(accel.acceleration.y, 5);
  Serial.print(" \tZ: ");
  Serial.print(accel.acceleration.z, 5);
  Serial.println(" m/s^2 ");

  //Display the results (acceleration is measured in m/s^2)
  Serial.print("\t\tGyro X: ");
  Serial.print(gyro.gyro.x, 5);
  Serial.print(" \tY: ");
  Serial.print(gyro.gyro.y, 5);
  Serial.print(" \tZ: ");
  Serial.print(gyro.gyro.z, 5);
  Serial.println(" radians/s ");
  Serial.println();
  */
  Serial.print(accel.acceleration.x, 5);
  Serial.print(" ");
  Serial.print(accel.acceleration.y, 5);
  Serial.print(" ");
  Serial.print(accel.acceleration.z, 5);
  Serial.print(" ");
  Serial.print(gyro.gyro.x, 5);
  Serial.print(" ");
  Serial.print(gyro.gyro.y, 5);
  Serial.print(" ");
  Serial.println(gyro.gyro.z, 5);
  delay(100);
  /*
  int delay1 = 100;
  //updateVect<float>(pitchVel, gyro.gyro.z);
  //pitch = simpsons4(delay1, pitchVel[3], pitchVel[2], pitchVel[1], pitchVel[0], pitch);
  Serial.println("---------------------------------------------------------");
  Serial.print("Pitch: ");
  Serial.print(gyro.gyro.x, 6);
  Serial.print(" ");
  pitch += gyro.gyro.x/delay1;
  Serial.print(pitch, 6);

  Serial.print("\t| Yaw: ");
  Serial.print(gyro.gyro.z, 6);
  Serial.print(" ");
  yaw += gyro.gyro.z/delay1;
  Serial.print(yaw, 6);

  Serial.print("\t| Roll: ");
  Serial.print(gyro.gyro.y, 6);
  Serial.print(" ");
  roll += gyro.gyro.y/delay1;
  Serial.println(roll, 6);

  delay(delay1);
  */
}

float altitudeSum;
float initialAltitude;

float getAltitudeAvg(int meanCount, int delayTime) {

  
  float altitudeArray[ meanCount ] = { };
  sensors_event_t accel;
  
  int i = 0;
  while (i < meanCount) {

    altitudeArray[i] = accel.acceleration.z;
    
    
    altitudeSum += altitudeArray[i];
    
    i++;

    delay(delayTime);

    }

    initialAltitude = altitudeSum / meanCount;

    return initialAltitude;
}
