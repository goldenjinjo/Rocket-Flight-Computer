#ifndef CONFIG_KEYS_HPP
#define CONFIG_KEYS_HPP

#include <cstddef>
#include <stdint.h>


extern float ALTITUDE_BUFFER_PERIOD;
extern float G_OFFSET;
extern float LAUNCH_VEL_THRESHOLD;
extern float LAUNCH_ACC_THRESHOLD;
extern float APOGEE_TIMER;
extern float LANDING_VEL_THRESHOLD;
extern float BOOTUP_MODE;
extern float DUAL_DEPLOY;
extern float DROGUE_DELAY;
extern float MAIN_DELAY;
extern float MAIN_DEPLOYMENT_ALT;

struct ConfigKey {
    uint8_t key;
    const char* name;
    float defaultValue;
    float* variable; // Pointer to the corresponding variable
};

// Define all configuration keys here
// ConfigKey CONFIG_KEYS[] = {
//     {0x00, "ALTITUDE_BUFFER_PERIOD", 2000.0, &ALTITUDE_BUFFER_PERIOD},
//     {0x01, "G_OFFSET", 9.81, &G_OFFSET},
//     {0x02, "LAUNCH_VEL_THRESHOLD", 15.0, &LAUNCH_VEL_THRESHOLD},
//     {0x03, "LAUNCH_ACC_THRESHOLD", 60.0, &LAUNCH_ACC_THRESHOLD},
//     {0x04, "APOGEE_TIMER", 100.0, &APOGEE_TIMER},
//     {0x05, "LANDING_VEL_THRESHOLD", 1.0, &LANDING_VEL_THRESHOLD},
//     {0x06, "BOOTUP_MODE", 1.0, &BOOTUP_MODE},
//     {0x07, "DUAL_DEPLOY", 1.0, &DUAL_DEPLOY},
//     {0x08, "DROGUE_DELAY", 5.0, &DROGUE_DELAY},
//     {0x09, "MAIN_DELAY", 15.0, &MAIN_DELAY},
//     {0x0A, "MAIN_DEPLOYMENT_ALT", 300.0, &MAIN_DEPLOYMENT_ALT}
// };

extern ConfigKey CONFIG_KEYS[];

// Number of configuration keys
// extern const size_t NUM_CONFIG_KEYS;
extern const std::size_t NUM_CONFIG_KEYS;
#endif // CONFIG_KEYS_HPP

