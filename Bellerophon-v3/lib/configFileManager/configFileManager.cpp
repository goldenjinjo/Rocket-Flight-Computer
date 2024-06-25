#include "ConfigFileManager.hpp"

// Define default configuration items with predefined positions
const ConfigFileManager::ConfigItem ConfigFileManager::defaultConfigs[] = {
    {ALTITUDE_BUFFER_PERIOD, 2000},
    {G_OFFSET, 9.81},
    {LAUNCH_VEL_THRESHOLD, 15},
    {LAUNCH_ACC_THRESHOLD, 60},
    {APOGEE_TIMER, 100},
    {LANDING_VEL_THRESHOLD, 1},
};

// Constructor for ConfigFileManager
ConfigFileManager::ConfigFileManager(FileManager& fm) : fm(fm) {}

// Initialize the config file
bool ConfigFileManager::initialize() {
    // Use FileManager's initialize method to set up the SD card
    if (!fm.initialize()) {
        Serial.println("FileManager initialization failed.");
        return false;
    }

    // initalize config file struct
    fm.initializeFileItem(fm.configFile, fm.configFileName);
   
    initializeWithDefaults();

    return true;
}

// Initialize config values with defaults and read from the config file
void ConfigFileManager::initializeWithDefaults() {
    // Do not initalize if config file already exists
    if (!fm.createFile(fm.configFile)) {
            Serial.println("Config File Exists");
            return;
        }
    Serial.println("Time to init config");
    // If the config file was just created, initalize the values
    for (const auto& config : defaultConfigs) {
        // If the key is not found, write the default value
            Serial.println("writing initial value for");
            Serial.println(config.key);
            Serial.println(config.defaultValue);
            writeValue(config.key, config.defaultValue);    
    }
}

// Read a config value from the config file
bool ConfigFileManager::readConfigValue(uint8_t key, float& value) {
    fm.openFileForRead(fm.configFile);
    // Seek to the correct position based on the key
    fm.configFile.type.seekSet(key * sizeof(float));
    fm.configFile.type.read((uint8_t*)&value, sizeof(value));
    fm.closeFile(fm.configFile);

    Serial.print("Read value for key: ");
    Serial.print(key, HEX);
    Serial.print(" = ");
    Serial.println(value);

    return true;
}

// Write a config value to the config file
bool ConfigFileManager::writeConfigValue(uint8_t key, float value) {
    return writeValue(key, value);
}

// Retrieve a config value by key
float ConfigFileManager::getConfigValue(uint8_t key) {
    float value;
    if (readConfigValue(key, value)) {
        return value;
    } else {
        // Return default value if key is not found
        for (const auto& config : defaultConfigs) {
            if (config.key == key) {
                return config.defaultValue;
            }
        }
    }
    return 0; // Default to 0 if key is not found and no default value is specified
}

// Write a value to the config file at a specific position
bool ConfigFileManager::writeValue(uint8_t key, float value) {
    fm.configFile.type.open(fm.configFile.name, O_WRITE);

    // Seek to the correct position based on the key
    fm.configFile.type.seekSet(key * sizeof(float));
    fm.configFile.type.write((uint8_t*)&value, sizeof(value));
    fm.closeFile(fm.configFile);

    Serial.print("Wrote value ");
    Serial.print(value);
    Serial.print(" to key ");
    Serial.println(key, HEX);

    return true;
}

// Print all config values from the config file to Serial
void ConfigFileManager::printAllConfigValuesToSerial() {
    for (const auto& config : defaultConfigs) {
        float value;
        if (readConfigValue(config.key, value)) {
            Serial.print(config.key, HEX);
            Serial.print(": ");
            Serial.println(value);
        } else {
            Serial.print(config.key, HEX);
            Serial.print(": ");
            Serial.println(config.defaultValue);
        }
    }
}

// Delete the config file
bool ConfigFileManager::deleteConfigFile() {
    return fm.deleteFile(fm.configFileName);
}
