#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <stdint.h>
#include "configKeys.hpp"

// for declaring constants and measured values


/// TODO: find a file to define this in, not suitable for constants folder
extern int mode;

// Serial messaging
extern const char* HANDSHAKE_MESSAGE;
extern const char* ACK_MESSAGE;
extern const char* END_OF_TRANSMISSION_MESSAGE;
extern const char* END_OF_TRANSMISSION_ACK;
extern const char* FILE_COPY_MESSAGE;
extern const char* ALL_FILES_SENT;
extern const char* ALL_FILES_SENT_ACK;
extern const char* REQUEST_FILE_DOWNLOAD;
extern const char* CANCEL_MSG_REQUEST;


// Serial message formatting
extern const char PREFIX;
extern const char SUFFIX;
extern const int BAUD_RATE;


// File naming configuration
extern const char* logFilePrefix;
extern const char* logFileSuffix;
extern const char* dataFilePrefix;
extern const char* dataFileSuffix;
extern const char* debugPrefix;
extern const uint8_t zeroPadding;


// Standard deviation of the model, aka model error
extern const int SIGMA_M;

// Standard deviations of altitude and acceleration measurements
extern const int SIGMA_S;
extern const int SIGMA_A;

// Atmospheric Pressure at Sea-Level in hPa
extern const float P_0;
extern const float L_B;
extern const float EXP;
extern const float C_TO_K;



// Mode configurations
#define STANDBY_MODE 0 
#define  READING_MODE 1
#define  PURGE_MODE 2
#define LOGGING_MODE 3
#define FIN_CONTROL_MODE 4
#define CONFIG_MODE 5




#endif // CONSTANTS_HPP