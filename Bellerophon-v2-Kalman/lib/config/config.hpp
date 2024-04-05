// file configuration
const char* logFileName = "flight_log.txt";
const char* dataFileName = "flight_data.csv";
bool deleteFile = false;

// -----Deployment Settings-----
// Whether computer should use dual or single deploy mode
bool dualDeploy = false;

// Delays before deploying drogue or main parachutes when in dual deploy mode.
// Note: drogueDelay is used when in single deploy mode.
float drogueDelay = 0;
float mainDelay = 0;

// Altitude to deploy main parachutes when in dual deploy mode.
float mainAltitude = 300;