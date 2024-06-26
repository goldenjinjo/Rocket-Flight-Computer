#include "configKeys.hpp"

float ALTITUDE_BUFFER_PERIOD;
float G_OFFSET;
float LAUNCH_VEL_THRESHOLD;
float LAUNCH_ACC_THRESHOLD;
float APOGEE_TIMER;
float LANDING_VEL_THRESHOLD;
float BOOTUP_MODE;
float DUAL_DEPLOY;
float DROGUE_DELAY;
float MAIN_DELAY;
float MAIN_DEPLOYMENT_ALT;


// Define all configuration keys here
ConfigKey CONFIG_KEYS[] = {
    {0x00, "ALTITUDE_BUFFER_PERIOD", 2000.0, nullptr},
    {0x01, "G_OFFSET", 9.81, nullptr},
    {0x02, "LAUNCH_VEL_THRESHOLD", 15.0, nullptr},
    {0x03, "LAUNCH_ACC_THRESHOLD", 60.0, nullptr},
    {0x04, "APOGEE_TIMER", 100.0, nullptr},
    {0x05, "LANDING_VEL_THRESHOLD", 1.0, nullptr},
    {0x06, "BOOTUP_MODE", 1.0, nullptr},
    {0x07, "DUAL_DEPLOY", 1.0, nullptr},
    {0x08, "DROGUE_DELAY", 5.0, nullptr},
    {0x09, "MAIN_DELAY", 15.0, nullptr},
    {0x0A, "MAIN_DEPLOYMENT_ALT", 300.0, nullptr}
};

// Number of configuration keys
// const size_t NUM_CONFIG_KEYS = sizeof(CONFIG_KEYS) / sizeof(CONFIG_KEYS[0]);
const std::size_t NUM_CONFIG_KEYS = sizeof(CONFIG_KEYS) / sizeof(CONFIG_KEYS[0]);

void initializeConfigKeys() {
    CONFIG_KEYS[0].variable = &ALTITUDE_BUFFER_PERIOD;
    CONFIG_KEYS[1].variable = &G_OFFSET;
    CONFIG_KEYS[2].variable = &LAUNCH_VEL_THRESHOLD;
    CONFIG_KEYS[3].variable = &LAUNCH_ACC_THRESHOLD;
    CONFIG_KEYS[4].variable = &APOGEE_TIMER;
    CONFIG_KEYS[5].variable = &LANDING_VEL_THRESHOLD;
    CONFIG_KEYS[6].variable = &BOOTUP_MODE;
    CONFIG_KEYS[7].variable = &DUAL_DEPLOY;
    CONFIG_KEYS[8].variable = &DROGUE_DELAY;
    CONFIG_KEYS[9].variable = &MAIN_DELAY;
    CONFIG_KEYS[10].variable = &MAIN_DEPLOYMENT_ALT;
}