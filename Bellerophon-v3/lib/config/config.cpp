#include "config.hpp"

bool DEBUG = true;


// Mode configurations
// 0 - standby
// 1 - read data
// 2 - delete data
// 3 - log data
int mode = STANDBY_MODE;

const char* logFilePrefix = "log_";
const char* logFileSuffix = ".txt";
const char* dataFilePrefix = "data_";
const char* dataFileSuffix = ".csv";

bool dualDeploy = false;
float drogueDelay = 0;
float mainDelay = 0;
float mainAltitude = 300;
