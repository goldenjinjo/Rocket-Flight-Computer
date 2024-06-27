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
    X(ALTITUDE_BUFFER_PERIOD, 2000.0) \
    X(G_OFFSET, 9.81) \
    X(LAUNCH_VEL_THRESHOLD, 15.0) \
    X(LAUNCH_ACC_THRESHOLD, 60.0) \
    X(APOGEE_TIMER, 100.0) \
    X(LANDING_VEL_THRESHOLD, 1.0) \
    X(BOOTUP_MODE, 0) \
    X(DUAL_DEPLOY, 1.0) \
    X(DROGUE_DELAY, 5.0) \
    X(MAIN_DELAY, 15.0) \
    X(MAIN_DEPLOYMENT_ALT, 300.0)

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
