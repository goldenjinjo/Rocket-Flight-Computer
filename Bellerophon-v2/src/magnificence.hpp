#ifndef MAGNIFICENCE
#define MAGNIFICENCE

//arrays are pointers weirdness here, true 1511 moment
//directly modifying input array rather than creaitng new one bc don't want to deal with garbage collection
void updateArray(float* arr, float newVal);

//input derivative values in ascending chronological order, final value is the current value to be updated
float simpsons4(float der0, float der1, float der2, float derCurr, float valCurr);

//updates accelereations
//changes accelerometer readings to cartesian using euler angles
//I have notes on how this is done, will upload when git is sorted
//may be wrong
void updateAccs();

//uses simpsons to update the crafts cartesian velocity and position
void updatePosition();

//update rocket rotation according to gyro readings
void updateRotation();

#endif