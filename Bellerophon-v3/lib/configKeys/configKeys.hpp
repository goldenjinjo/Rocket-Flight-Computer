/**
 * @file ConfigKeys.hpp
 * @brief Defines and initializes configuration keys and their associated variables.
 *
 * This file provides a single source of truth for all configuration variables used in the system.
 * The configuration variables, their default values, and pointers are managed using a macro
 * to ensure consistency and reduce redundancy.
 */

#ifndef CONFIG_KEYS_HPP
#define CONFIG_KEYS_HPP

#include <cstddef> // Include this header for size_t
#include <stdint.h>
#include <Arduino.h>

/**
 * @struct ConfigKey
 * @brief Struct representing a configuration key.
 *
 * Each configuration key is associated with a unique key, name, default value, and a pointer
 * to the corresponding global variable.
 */
struct ConfigKey {
    uint8_t key;            ///< Unique key for the configuration variable
    const char* name;       ///< Name of the configuration variable
    const float defaultValue;     ///< Default value of the configuration variable
    float* variable;        ///< Pointer to the corresponding global variable
};

/**
 * @def CONFIG_VARIABLES
 * @brief Macro defining all configuration variables, their default values, and pointers.
 *
 * This macro is used to declare, define, and initialize configuration variables in a consistent manner.
 */
#define CONFIG_VARIABLES \
   X(ALTITUDE_BUFFER_PERIOD, 2000.0) /* Period to buffer initial altitude measurements (Unit TBD) */ \
    X(G_OFFSET, 9.81) /* 1G offset for accelerometer, accelerometer will measure 0g when in unpowered flight (unit TBD) */ \
    X(LAUNCH_VEL_THRESHOLD, 15.0) /* Launch Detect Threshold for Velocity (Unit TBD) */ \
    X(LAUNCH_ACC_THRESHOLD, 60.0) /* Launch Detect Threshold for Acceleration (Unit TBD) */ \
    X(APOGEE_TIMER, 100.0) /* Time the rocket must spend with a velocity estimate below 0 before apogee is decided (seconds) */ \
    X(LANDING_VEL_THRESHOLD, 1.0) /* Landing velocity threshold (i.e. if velocity is lower than this consider the rocket landed) (Unit TBD) */ \
    X(BOOTUP_MODE, 0) /* Mode that activates when computer resets */ \
    X(DUAL_DEPLOY, 1.0) /* Flag for triggering dual deploy (may be replaced by a mode) (0: No dual deploy, 1: dual deploy) */ \
    X(DROGUE_DELAY, 5.0) /* Delay to deploy drogue parachute (ms) */ \
    X(MAIN_DELAY, 15.0) /* Delay to deploy main parachute (ms) */ \
    X(MAIN_DEPLOYMENT_ALT, 300.0) /* Altitude to deploy main parachute (ft) */ \
    X(DEBUG, 0.0) /* Flag for enabling debug (0: disabled, 1: enabled ) */ \
    X(SERVO_A_CENTER_POSITION, 90.0) /* 0 Deflection Angle for Servo A Based on Fin Alignment */ \
    X(SERVO_B_CENTER_POSITION, 90.0) /* 0 Deflection Angle for Servo B Based on Fin Alignment */ \
    X(SERVO_C_CENTER_POSITION, 90.0) /* 0 Deflection Angle for Servo C Based on Fin Alignment */ \
    X(SERVO_D_CENTER_POSITION, 90.0) /* 0 Deflection Angle for Servo D Based on Fin Alignment */

// Declare the global variables
#define X(name, defaultValue) extern float name;
CONFIG_VARIABLES
#undef X

// Declare the configuration keys
extern ConfigKey CONFIG_KEYS[];

// Number of configuration keys
extern const std::size_t NUM_CONFIG_KEYS;

/**
 * @brief Initializes the pointers in the CONFIG_KEYS array to point to the corresponding global variables.
 *
 * This function ensures that each configuration key's pointer is correctly set to the global variable.
 */
void initializeConfigKeys();

/**
 * @brief Prints all data in the configKeys struct using Serial.println() for the purpose of debugging
 */
void printConfigKeysToSerial();

#endif // CONFIG_KEYS_HPP
