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


int airbrakeWaitTime = 16000; // 16 seconds

int airbrakeDeployTime = 5000; // 5 seconds

int angleStep = 15; // 15 degrees

int stepSize = 4;

int maxAngle = 45;

int intAngleOffSet = 10;

bool icm_enable = true;

int Y_AXIS = 0;

