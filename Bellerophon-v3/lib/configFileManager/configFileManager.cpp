#include "ConfigFileManager.hpp"
#include "configKeys.hpp"
#include <Arduino.h>


ConfigFileManager::ConfigFileManager(FileManager& fm) : fm(fm) {}

void ConfigFileManager::initialize() {
    // Initalize config struct 
    fm.initializeFileItem(fm.configFile, fm.configFileName);

    // initialize ConfigKey Struct
    initializeConfigKeys();

    // initalizes external variables with default values only if the config file does not exist
    initializeWithDefaults();

    // assign config values to external variables for run time
    loadConfigValues();

    return;
}

void ConfigFileManager::initializeWithDefaults() {
    if (!fm.createFile(fm.configFile)) {
        Serial.println("Config File Exists");
        return;
    }

    Serial.println("Time to init config");

    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        Serial.print("Writing initial value for ");
        Serial.print(CONFIG_KEYS[i].name);
        Serial.print(": ");
        Serial.println(CONFIG_KEYS[i].defaultValue);
        writeConfigValue(CONFIG_KEYS[i].key, CONFIG_KEYS[i].defaultValue); 
    }
}

void ConfigFileManager::restoreDefaults() {
    deleteConfigFile();
    initializeWithDefaults();
}

const char* ConfigFileManager::keyToString(uint8_t key) {
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        if (CONFIG_KEYS[i].key == key) {
            return CONFIG_KEYS[i].name;
        }
    }
    return "UNKNOWN_KEY";
}

bool ConfigFileManager::readConfigValue(uint8_t key, float& value) {
   
    uint32_t position = key * sizeof(float);
    if(!fm.readFloatFromFile(fm.configFile, position, value)) {
        Serial.println("error reading config file");
        return false;
    }

    if(DEBUG) {
        Serial.print("Read value for key ");
        Serial.print(keyToString(key));
        Serial.print(": ");
        Serial.println(value);
    }
    return true;
}

bool ConfigFileManager::writeConfigValue(uint8_t key, float value) {

    uint32_t position = key * sizeof(float);
    if(!fm.writeFloatToFile(fm.configFile, position, value)){
        return false;
    }

    if(DEBUG) {
        Serial.print("Wrote value ");
        Serial.print(value);
        Serial.print(" to key ");
        Serial.println(keyToString(key));
    }
    
    // Update pointer with value
    AssignConfigValue(key, value);

    return true;
}

bool ConfigFileManager::writeConfigValueFromString(const char* keyName, float value) {
     for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
          if (strcmp(keyName, CONFIG_KEYS[i].name) == 0) {
            return writeConfigValue(CONFIG_KEYS[i].key, value);
        }
    }
    Serial.print("Unknown config key: ");
    Serial.println(keyName);
    return false;
}

float ConfigFileManager::getConfigValue(const char* keyName) {
    uint8_t key = stringToKey(keyName);
    if (key == 0xFF) {
        return 0;  // Return 0 for unknown keys
    }

    float value;
    if (readConfigValue(key, value)) {
        return value;
    } else {
        for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
            if (CONFIG_KEYS[i].key == key) {
                return CONFIG_KEYS[i].defaultValue;
            }
        }
    }
    return 0;
}

void ConfigFileManager::printAllConfigValuesToSerial() {
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        float value;
        if (readConfigValue(CONFIG_KEYS[i].key, value)) {
            Serial.print(CONFIG_KEYS[i].name);
            Serial.print(": ");
            Serial.println(value);
        } else {
            Serial.print(CONFIG_KEYS[i].name);
            Serial.print(": ");
            Serial.println(CONFIG_KEYS[i].defaultValue);
        }
    }
}

void ConfigFileManager::loadConfigValues() {
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        float value;
        if (readConfigValue(CONFIG_KEYS[i].key, value)) {
            // update pointer
            AssignConfigValue(CONFIG_KEYS[i].key, value);
        }
    }
}

bool ConfigFileManager::deleteConfigFile() {
    return fm.deleteFile(fm.configFileName);
}

void ConfigFileManager::AssignConfigValue(uint8_t key, float value) {
    if (key >= NUM_CONFIG_KEYS) {
        Serial.print("Invalid key: ");
        Serial.println(key, HEX);
        return;
    }

    if (CONFIG_KEYS[key].variable == nullptr) {
        Serial.print("Null pointer for key: ");
        Serial.println(key, HEX);
        return;
    }

    *(CONFIG_KEYS[key].variable) = value;
    // Serial.print("Assigned value ");
    // Serial.print(value);
    // Serial.print(" to key ");
    // Serial.println(keyToString(key));
}


uint8_t ConfigFileManager::stringToKey(const char* keyName) {
    for (size_t i = 0; i < NUM_CONFIG_KEYS; ++i) {
        if (strcmp(keyName, CONFIG_KEYS[i].name) == 0) {
            return CONFIG_KEYS[i].key;
        }
    }
    Serial.print("Unknown config key: ");
    Serial.println(keyName);
    return 0xFF;  // Return an invalid key if not found
}