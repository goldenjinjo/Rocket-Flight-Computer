#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <string>

// DEBUG setting. Set to false for flight!!
// Primary purpose is to print logfile messages to serial
extern bool DEBUG;



// Mode configuration
#define STANDBY_MODE 0
#define READING_MODE 1
#define PURGE_MODE 2
#define LOGGING_MODE 3

extern int mode;

// File naming configuration
extern const char* logFilePrefix;
extern const char* logFileSuffix;
extern const char* dataFilePrefix;
extern const char* dataFileSuffix;

// Deployment Settings
// Whether computer should use dual or single deploy mode
extern bool dualDeploy;

// Delays before deploying drogue or main parachutes when in dual deploy mode.
// Note: drogueDelay is used when in single deploy mode.
extern float drogueDelay;
extern float mainDelay;

// Altitude to deploy main parachutes when in dual deploy mode.
extern float mainAltitude;



#endif // CONFIG_HPP
