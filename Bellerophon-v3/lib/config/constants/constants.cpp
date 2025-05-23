#include "constants.hpp"

// for declaring constants and measured values

int mode = BOOTUP_MODE;

const char* HANDSHAKE_MESSAGE = "START_TRANSFER";
const char* ACK_MESSAGE = "TRANSFER_ACK";
const char* END_OF_TRANSMISSION_MESSAGE = "END_OF_TRANSMISSION";
const char* END_OF_TRANSMISSION_ACK = "END_OF_TRANSMISSION_ACK";
const char* FILE_COPY_MESSAGE = "FILE_ALREADY_RECEIVED";
const char* ALL_FILES_SENT = "ALL_FILES_SENT";
const char* ALL_FILES_SENT_ACK = "ALL_FILES_SENT_ACK";
const char* REQUEST_FILE_DOWNLOAD = "REQUEST_FILE_DOWNLOAD";
const char* CHANGE_SETTINGS_MESSAGE = "CHANGE_SETTINGS";
const char* MANUAL_SERVO_CONTROL_MESSAGE = "MANUAL_CONTROL";
const char* CANCEL_MSG_REQUEST = "EXIT_PROGRAM";
const char* REQUEST_SETTINGS_INFO_MESSAGE = "SETTINGS_INFO";
const char* DELETE_FILE_MESSAGE = "PURGE_TIME";
const char* RESET_CONFIG_MESSAGE = "RESET_SETTINGS";

// Serial message formatting
/// RULES: 
//  - CAN NOT USE NULL TERMINATOR: '\0', 
//  - PREFIX and SUFFIX must be different chars
const char PREFIX = '$';
const char SUFFIX = '!';
const int BAUD_RATE = 115200;

// Standard deviation of the model, aka model error
const int SIGMA_M = 10;

// Standard deviations of altitude and acceleration measurements
const int SIGMA_S = 3;
const int SIGMA_A = 2;

// File naming configuration
const char* logFilePrefix = "log_";
const char* logFileSuffix = ".txt";
const char* dataFilePrefix = "data_";
const char* dataFileSuffix = ".csv";
const char* debugPrefix = "debug_";

// number of 0s to use in file name formatting
// e.g. for 3: example_000.example
const uint8_t zeroPadding = 6;