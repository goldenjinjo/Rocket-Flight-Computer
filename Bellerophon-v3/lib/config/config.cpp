#include "config.hpp"

bool DEBUG = true;

const char* logFileName = "flight_log.txt";
const char* dataFileName = "flight_data.csv";

// Mode configurations
// 0 - standby
// 1 - read data
// 2 - log data
int mode = READING_MODE;

bool dualDeploy = false;
float drogueDelay = 0;
float mainDelay = 0;
float mainAltitude = 300;
