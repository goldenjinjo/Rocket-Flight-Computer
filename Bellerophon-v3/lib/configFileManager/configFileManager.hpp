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
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
     * @brief Initializes config values with defaults and reads from the config file.
     */
    void initializeWithDefaults();

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
     * @brief Retrieves a config value by key.
     * @param key The byte identifier of the config value to retrieve.
     * @return The config value as a float.
     */
    float getConfigValue(uint8_t key);

    /**
     * @brief Prints all config values from the config file to Serial.
     */
    void printAllConfigValuesToSerial();

    /**
     * @brief Deletes the config file.
     * @return True if the config file is successfully deleted, false otherwise.
     */
    bool deleteConfigFile();

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
     * @brief Writes a key-value pair to the config file at a specific position.
     * @param key The byte identifier of the config value.
     * @param value The value to write.
     * @return True if the key-value pair is successfully written, false otherwise.
     */
    bool writeValue(uint8_t key, float value);
};

#endif // CONFIG_FILE_MANAGER_HPP
