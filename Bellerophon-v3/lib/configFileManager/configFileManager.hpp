#ifndef CONFIG_FILE_MANAGER_HPP
#define CONFIG_FILE_MANAGER_HPP

#include "fileManager.hpp"
#include "configKeys.hpp"
#include <map>

/**
 * @class ConfigFileManager
 * @brief A class to manage configuration file operations on an SD card,
 *        including reading and writing predefined configuration values.
 *        Config values are stored as floats for robustness.
 */
class ConfigFileManager {
public:
    // Constructor
    ConfigFileManager(FileManager& fm);

    /**
     * @brief Initializes the config file. Creates it if it doesn't exist.
     */
    void initialize();

    /**
     * @brief Initializes config values with defaults and reads from the config file.
     */
    void initializeWithDefaults();

    /**
     * @brief Deletes config file and reinitializes the values.
     */
    void restoreDefaults();

    /**
     * @brief Reads a config value from the config file.
     * @param key The byte identifier of the config value to read.
     * @param value Reference to store the read value.
     * @return True if the value is successfully read, false otherwise.
     */
    bool readConfigValue(uint8_t key, float& value);

    /**
     * @brief Writes a config value to the config file.
     * @param key The byte identifier of the config value to write.
     * @param value The value to write.
     * @return True if the value is successfully written, false otherwise.
     */
    bool writeConfigValue(uint8_t key, float value);

    /**
     * @brief Writes a config value to the config file.
     * @param keyName The const char* name of the variable to write.
     * @param value The value to write.
     * @return True if the keyName is valid and the value is successfully written, false otherwise.
     */
    bool writeConfigValueFromString(const char* keyName, float value);

    /**
     * @brief Retrieves a config value by key name.
     * @param keyName The string identifier of the config value to retrieve.
     * @return The config value as a float.
     */
    float getConfigValue(const char* keyName);

    /**
     * @brief Prints all config values from the config file to Serial.
     */
    void printAllConfigValuesToSerial();

    /**
     * @brief Converts a config key to its string name.
     * @param key The byte identifier of the config value to retrieve.
     * @return The config value name as a const char*.
     */
    const char* keyToString(uint8_t key);

    /**
     * @brief Loads the configuration values from the config file into the global variables.
     *
     * This function reads the values from the config file and assigns them to the corresponding
     * global variables using the pointers in the CONFIG_KEYS array.
     */
    void loadConfigValues();

private:
    FileManager& fm;                 // Reference to the parent FileManager instance

    /**
     * @brief Struct for storing default configuration items.
     */
    struct ConfigItem {
        uint8_t key;
        float defaultValue;
    };

    /**
     * @brief Default configuration items with predefined positions.
     */
    static const ConfigItem defaultConfigs[];

    /**
     * @brief Deletes the config file.
     * @return True if the config file is successfully deleted, false otherwise.
     */
    bool deleteConfigFile();

    /**
     * @brief Updates pointer of external variable for a given key.
     * @param key The byte identifier of the config value to update.
     * @param value The value to assign to the external variable.
     */
    void AssignConfigValue(uint8_t key, float value);

    /**
     * @brief Converts a config key name to its byte identifier.
     * @param keyName The string identifier of the config value to convert.
     * @return The byte identifier of the config value.
     */
    uint8_t stringToKey(const char* keyName);
};

#endif // CONFIG_FILE_MANAGER_HPP

