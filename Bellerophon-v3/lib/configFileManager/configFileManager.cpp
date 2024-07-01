#include "ConfigFileManager.hpp"
#include "configKeys.hpp"
#include <Arduino.h>


ConfigFileManager::ConfigFileManager(FileManager& fm) : fm(fm) {
    // Initalize keys defined in configKeys.cpp
    //initializeConfigKeys();
}


bool ConfigFileManager::initialize() {
  
    // initialize ConfigKey Struct
    initializeConfigKeys();

    // Initalize config struct 
    fm.initializeFileItem(fm.configFile, fm.configFileName);
    
    // initalizes external variables with default values only if the config file does not exist
    initializeWithDefaults();

    // assign config values to external variables for run time
    loadConfigValues();

    return true;
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
        fm.writeFloatToFile(fm.configFile, CONFIG_KEYS[i].key, CONFIG_KEYS[i].defaultValue); 
    }
}

void ConfigFileManager::restoreDefaults() {
    deleteConfigFile();
    initializeWithDefaults();
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
    Serial.print("Assigned value ");
    Serial.print(value);
    Serial.print(" to key ");
    Serial.println(keyToString(key));
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
   
    fm.openFileForRead(fm.configFile);

    uint32_t position = key * sizeof(float);
    if (!fm.configFile.type.seekSet(position)) {
        Serial.println("Failed to seek to position.");
        fm.closeFile(fm.configFile);
        return false;
    }

    if (fm.configFile.type.read((uint8_t*)&value, sizeof(value)) != sizeof(value)) {
        Serial.println("Failed to read the full float value.");
        fm.closeFile(fm.configFile);
        return false;
    }

    fm.closeFile(fm.configFile);

    Serial.print("Read value for key ");
    Serial.print(keyToString(key));
    Serial.print(": ");
    Serial.println(value);

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

float ConfigFileManager::getConfigValue(uint8_t key) {
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



bool ConfigFileManager::writeValue(uint8_t key, float value) {
    if (!fm.configFile.type.open(fm.configFile.name, O_WRITE)) {
        Serial.println("Failed to open config file for writing.");
        return false;
    }

    uint32_t position = key * sizeof(float);
    if (!fm.configFile.type.seekSet(position)) {
        Serial.println("Failed to seek to position.");
        fm.configFile.type.close();
        return false;
    }

    if (fm.configFile.type.write((uint8_t*)&value, sizeof(value)) != sizeof(value)) {
        Serial.println("Failed to write float value.");
        fm.configFile.type.close();
        return false;
    }

    fm.closeFile(fm.configFile);

    Serial.print("Wrote value ");
    Serial.print(value);
    Serial.print(" to key ");
    Serial.println(keyToString(key));

    // Update pointer with value
    AssignConfigValue(key, value);

    return true;
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
