#ifndef CONFIG_HPP
#define CONFIG_HPP

// DEBUG setting. Set to false for flight!!
// Primary purpose is to print logfile messages to serial
extern bool DEBUG;

// file configuration
extern const char* logFileName;
extern const char* dataFileName;

// -----Deployment Settings-----
// Whether computer should use dual or single deploy mode
extern bool dualDeploy;

// Delays before deploying drogue or main parachutes when in dual deploy mode.
// Note: drogueDelay is used when in single deploy mode.
extern float drogueDelay;
extern float mainDelay;

// Altitude to deploy main parachutes when in dual deploy mode.
extern float mainAltitude;


#endif // CONFIG_HPP
