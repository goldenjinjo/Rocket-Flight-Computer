// file configuration
const char* fileName = "flight data";
bool deleteFile = false;

// Shortens some functions and adds debug, set to false for flight application
bool testMode = false;

// setting states, should not be changed (unless testing)
bool recordInitial = true;
bool endFlight = false;
bool startFlight = false;
bool apoogeeReach = false;


// values for callibration
const int calliMeasure = 500; // number of measurements to take total
const float calliDelay = 10; // delay in ms to use between measurements
int stationaryCount = 0;

// measure
int meanCount = 100;
const float offSetGyroX = 0.3;

// barometer precison - not using this for now
// set delay for how often barometer takes measurements
const float baroDelay = 10; // milliseconds
const int IMUDuration = 5; // milliseconds
int baroCount = 5;

// starting conditions - for the the pre-lanch phase of flight
const float accThreshold = 35;
const float pressureChangeThreshold = 300; // Pa. 300 Pa = approx 100m alt change at sea level
const float landingPressureThreshold = 50; // Pa - deltaP for detecting landing
float preFlightDelay = 30; // Delay in ms to take before flight take-off
// ending conditions

// End of Flight can not be triggered unless altLimitReach is switched to true
// *** CHANGE THIS TO DETECTING DESCENT PHASE ONCE CODE IS READY ***
bool altLimitReach = false;

// Altitude that must be reached for altLimitReach to switch to true
const float maxAltThreshold = 100; // metres

// Altitude must be measured to be below this threshold before it will record end of flight
const float minAltThreshold = 5; // metres

// number of loop iterations device must measure below minAltThreshold before it will trigger End of Flight

float endTime = 60; // (s) - desired duration of time for computer to take measurements on ground. Change this value.
float sampleRate = 10; // (ms) - time for a single recording of values. Experimentally derived.

float totalSampleTime = meanCount*sampleRate; // (ms) - time taken to complete a full loop
const int endThreshold = endTime*1000 / totalSampleTime; // ~ 30 seconds at 100 hz

