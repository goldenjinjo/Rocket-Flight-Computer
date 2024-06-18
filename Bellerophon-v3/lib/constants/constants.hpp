#ifndef CONSTANTS_H
#define CONSTANTS_H

// for declaring constants and measured values

#define HANDSHAKE_MESSAGE "START_TRANSFER"
#define ACK_MESSAGE "TRANSFER_ACK"
#define END_OF_TRANSMISSION_MESSAGE "END_OF_TRANSMISSION"
#define END_OF_TRANSMISSION_ACK "END_OF_TRANSMISSION_ACK"
#define FILE_COPY_MESSAGE "FILE_ALREADY_RECEIVED"
#define ALL_FILES_SENT "ALL_FILES_SENT"
#define ALL_FILES_SENT_ACK "ALL_FILES_SENT_ACK"
#define REQUEST_FILE_DOWNLOAD "REQUEST_FILE_DOWNLOAD"
#define CANCEL_MSG_REQUEST "EXIT_PROGRAM"

// Serial message formatting
#define PREFIX ""
#define SUFFIX "\n"
#define BAUD_RATE 115200  




#define SPI_SPEED SD_SCK_MHZ(4)

// Standard deviation of the model, aka model error
// TODO: This must be tuned
#define SIGMA_M 10

// Standard deviations of altitude and acceleration measurements
// TODO: These must be determined experimentally
#define SIGMA_S 3
#define SIGMA_A 2

// Period to buffer initial altitude measurements
#define ALTITUDE_BUFFER_PERIOD 2000

//Atmospheric Pressure at Sea-Level in hPa
#define P_0 1013.25
#define L_B 0.0065
#define EXP 1/5.257
#define C_TO_K 273.15

// 1G offset for accelerometer, accelerometer will measure 0g when in unpowered flight
#define G_OFFSET 9.81

// Constant used for recursive filter on acceleration measurements.
#define RECURSIVE_CONSTANT 0.875

// Launch detect thresholds for velocity and acceleration
#define LAUNCH_VEL_THRESHOLD 15
#define LAUNCH_ACC_THRESHOLD 60

// Time the rocket must spend with a velocity estimate below 0 before apgoee is decided 
#define APOGEE_TIMER 100

// Landing velocity threshold (i.e. if velocity is lower than this consider the rocket landed)
#define LANDING_VEL_THERSHOLD 1

//Small epsilon for comparing values
#define EPSILON 0.01





#endif // CONSTANTS_H
