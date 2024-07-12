/**
 * @file ConfigKeys.cpp
 * @brief Implements the initialization and management of configuration keys and their associated variables.
 *
 * This file includes the definitions and initializations of the configuration keys using the CONFIG_VARIABLES macro.
 * It ensures that the configuration keys are correctly mapped to their global variables.
 */

#include "configKeys.hpp"

// Define the global variables without initializing them
#define X(name, defaultValue) float name;
CONFIG_VARIABLES
#undef X

// Define all configuration keys here
#define X(name, defaultValue) {__COUNTER__, #name, defaultValue, nullptr},
ConfigKey CONFIG_KEYS[] = {
    CONFIG_VARIABLES
};
#undef X

// Number of configuration keys
const std::size_t NUM_CONFIG_KEYS = sizeof(CONFIG_KEYS) / sizeof(CONFIG_KEYS[0]);

/**
 * @brief Initializes the pointers in the CONFIG_KEYS array to point to the corresponding global variables.
 *
 * This function ensures that each configuration key's pointer is correctly set to the global variable.
 */
// void initializeConfigKeys() {
//     #define X(name, defaultValue) CONFIG_KEYS[__COUNTER__].variable = &name;
//     CONFIG_VARIABLES
//     #undef X
// }


void initializeConfigKeys() {
    Serial.println("Initializing Config Keys");

    std::size_t index = 0;
    #define X(name, defaultValue) CONFIG_KEYS[index++].variable = &name;
    CONFIG_VARIABLES
    #undef X

    // Set the key values explicitly
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        CONFIG_KEYS[i].key = i;
    }
}


void printConfigKeysToSerial() {
    
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        Serial.print("Key: ");
        Serial.print(CONFIG_KEYS[i].key, HEX);
        Serial.print(", Name: ");
        Serial.print(CONFIG_KEYS[i].name);
        Serial.print(", Default Value: ");
        Serial.print(CONFIG_KEYS[i].defaultValue);
        Serial.print(", Variable Pointer: ");
        Serial.print(reinterpret_cast<uintptr_t>(CONFIG_KEYS[i].variable), HEX);
        Serial.print(", Variable Value: ");
        if (CONFIG_KEYS[i].variable) {
            Serial.println(*CONFIG_KEYS[i].variable);
        } else {
            Serial.println("nullptr");
        }
    }
}