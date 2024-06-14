#include "config.hpp"

bool DEBUG = true;

// baudrate



// Mode configurations
// 0 - standby
// 1 - read data
// 2 - delete data
// 3 - log data
// 4 - manual fin control
int mode = STANDBY_MODE;


// data file management
const char* logFilePrefix = "log_";
const char* logFileSuffix = ".txt";
const char* dataFilePrefix = "data_";
const char* dataFileSuffix = ".csv";
const char* debugPrefix = "debug_";

// number of 0s to use in file name formatting
// e.g. for 3: example_000.example
const uint8_t zeroPadding = 6;

bool dualDeploy = false;
float drogueDelay = 0;
float mainDelay = 0;
float mainAltitude = 300;
