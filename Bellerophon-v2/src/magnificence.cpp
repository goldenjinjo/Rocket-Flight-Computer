#include "magnificence.hpp"
#include <Arduino.h>
#include <Adafruit_Sensor.h>
// coordinate system is cartesian with launch site as origin
// integration will use simpsons method, hence arrays for accelleration
// arrays are in ascending chronological order, i.e. blah[0] is the least recent blah in the array
const byte arrSiz = 4; //size of storage arrays, using 4 bc using simpsons4
// accellerations in xyz
float xa[arrSiz];
float ya[arrSiz];
float za[arrSiz];
// velocities in xyz
float xv[arrSiz];
float yv[arrSiz];
float zv[arrSiz];
// position in xyz
float xp;
float yp;
float zp;

// angular velocity in pitch,roll,yaw
float rolv[arrSiz];
float pitv[arrSiz];
float yawv[arrSiz];
// angular position in pitch,roll,yaw. these will serve as euler angles
float rolp;
float pitp;
float yawp;

unsigned long fireTime;
unsigned long timeStep = 100; //time step between readings in milliseconds, essential for simpsons

/*
void setup() {
  // assume rocket starts pointing directly up, at rest (no acceleration or velocity)
  for(int i = 0; i < arrSiz; i++){
    //acc initialization
    xa[i] = ya[i] = za[i] = 0;
    //vel initialization
    xv[i] = yv[i] = zv[i] = pitv[i] = rolv[i] = yawv[i] = 0;
  }
  xp = yp = zp = pitp = rolp = yawp = 0; //pos init
  fireTime += timeStep;
}


void loop() {
  //delay is not good to use at the end due to uncertainty in time for code to process before it
  //use "fire rate" mechanism instead
  //millis() only loops after several decades so i think we're fine lol
  if(millis() < fireTime){
    return; // do nothing if it's not time to take another reading
  }
  fireTime = millis()+timeStep;
  updateRotation();
  updatePosition();
  
}
*/
//arrays are pointers weirdness here, true 1511 moment
//directly modifying input array rather than creaitng new one bc don't want to deal with garbage collection
void updateArray(float* arr, float newVal){
  //shifting all but last element back one i.e. arr[0] = arr[1]
  for(int i = 1; i < arrSiz; i++){
    arr[i-1] = arr[i];
  }
  
  arr[arrSiz-1] = newVal;
  
}

//input derivative values in ascending chronological order, final value is the current value to be updated
float simpsons4(float der0, float der1, float der2, float derCurr, float valCurr){
  float integratedVal = valCurr + timeStep/24*(55*derCurr-59*der2+37*der1-9*der0);
  return integratedVal;
}


//updates accelereations
//changes accelerometer readings to cartesian using euler angles
//I have notes on how this is done, will upload when git is sorted
//may be wrong
void updateAccs(){

  // method for acceleration
  sensors_event_t accel;
  
  float psi = rolp; //xy plane rotation
  float thet = pitp; //xz plane rotation
  float phi = yawp; //yz plane rotation
  
  //dummy variables for acceleration inputs in m*s^-2
  float sensX = accel.acceleration.x;
  float sensY = accel.acceleration.z;
  float sensZ = accel.acceleration.y;

  //cartesian accelerations
  float realX = sensX*cos(thet)*cos(psi)+sensY*(cos(psi)*sin(phi)*sin(thet)-cos(phi)*sin(psi))+sensZ*(sin(phi)*sin(psi)+cos(phi)*cos(psi)*sin(thet));
  float realY = sensX*cos(thet)*sin(psi)+sensY*(cos(phi)*cos(psi)+sin(phi)*sin(psi)*sin(thet))+sensZ*(cos(phi)*sin(psi)-cos(psi)*sin(phi));
  float realZ = sensX*(-sin(thet))+sensY*cos(thet)*sin(phi)+sensZ*cos(phi)*cos(thet);

  updateArray(xa, realX);
  updateArray(ya, realY);
  updateArray(za, realZ);
}

//uses simpsons to update the crafts cartesian velocity and position
void updatePosition(){
  updateAccs();

  updateArray(xv, simpsons4(xa[0], xa[1], xa[2], xa[3], xv[3]) );
  updateArray(yv, simpsons4(ya[0], ya[1], ya[2], ya[3], yv[3]) );
  updateArray(zv, simpsons4(za[0], za[1], za[2], za[3], zv[3]) );

  xp = simpsons4(xv[0], xv[1], xv[2], xv[3], xp);
  yp = simpsons4(yv[0], yv[1], yv[2], yv[3], yp);
  zp = simpsons4(zv[0], zv[1], zv[2], zv[3], zp);
}



//update rocket rotation according to gyro readings
void updateRotation(){
  //dummy variables for gyro acceleration inputs in rad/s
  float sensPit = 0;
  float sensRol = 0;
  float sensYaw = 0;

  //updating angular acceleration arrays with new readings
  updateArray(pitv, sensPit);
  updateArray(rolv, sensRol);
  updateArray(yawv, sensYaw);
  
  //updating position values with simpsons
  pitp = fmod(simpsons4(pitv[0], pitv[1], pitv[2], pitv[3], pitp), 2*PI);
  rolp = fmod(simpsons4(rolv[0], rolv[1], rolv[2], rolv[3], rolp), 2*PI);
  yawp = fmod(simpsons4(yawv[0], yawv[1], yawv[2], yawv[3], yawp), 2*PI);
}