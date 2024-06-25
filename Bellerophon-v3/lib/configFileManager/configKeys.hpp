#ifndef CONFIG_KEYS_HPP
#define CONFIG_KEYS_HPP

#include <stdint.h>

struct ConfigKey {
    uint8_t key;
    const char* name;
    float defaultValue;
};

// Define all configuration keys here
const ConfigKey CONFIG_KEYS[] = {
    {0x00, "ALTITUDE_BUFFER_PERIOD", 2000.0},
    {0x01, "G_OFFSET", 9.81},
    {0x02, "LAUNCH_VEL_THRESHOLD", 15.0},
    {0x03, "LAUNCH_ACC_THRESHOLD", 60.0},
    {0x04, "APOGEE_TIMER", 100.0},
    {0x05, "LANDING_VEL_THRESHOLD", 1.0},
    {0x06, "BOOTUP_MODE", 1.0},
    {0x07, "DUAL_DEPLOY", 1.0},
    {0x08, "DROGUE_DELAY", 5.0},
    {0x09, "MAIN_DELAY", 15.0},
    {0x0A, "MAIN_DEPLOYMENT_ALT", 300.0}
};

// Number of configuration keys
const size_t NUM_CONFIG_KEYS = sizeof(CONFIG_KEYS) / sizeof(CONFIG_KEYS[0]);

#endif // CONFIG_KEYS_HPP

