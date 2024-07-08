#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <SdFat.h>
#include <Arduino.h>
#include <vector>
#include <string>
#include <type_traits>
#include "configKeys.hpp"
#include "constants.hpp"
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"


/**
 * @class FileManager
 * @brief A class to manage file operations on an SD card, including, data storage, file naming, 
 *        tracking and indexing. Provides functionality to initialize the SD card, create, read, 
 *        write, and delete files, as well as maintain an index file for tracking file names.
 */
class FileManager {
public:
    // MEMBERS
    const char* indexFileName = "index.dat"; // Name of the index file
    const char* configFileName = "config.dat"; // Name of the config file

    SdFs sd;                      // SD card instance

    static const uint8_t maxFileNameLength = 30; // Maximum length for file names
    char logFileName[maxFileNameLength];         // Name of the log file
    char dataFileName[maxFileNameLength];        // Name of the data file

    // Member to hold array of file names
    std::vector<std::string> fileNames;

    struct FileItem {
        FsFile type;
        const char* name;
    };

    FileItem logFile; // File for logging flight events
    FileItem dataFile; // File for recording sensor data
    FileItem configFile; // File for settings management system
    FileItem indexFile; // File for tracking file naming counters


    /**
     * @brief  Constructor for FileManager. Initializes counters and file names.
     */
    FileManager();

    /**
     * @brief  Initializes SD card and file names.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

    /**
    @brief Initializes a FileItem struct with a given name.
    @param fileItem The FileItem struct to initialize.
    @param name The name to assign to the fileItem.
    */
    void initializeFileItem(FileItem& fileItem, const char* name);

    /**
     * @brief  Scans all files on the SD card and stores their names in a vector.
     */
    void updateFileList();

    /**
     * @brief  Scans all files on the SD card and prints their names to Serial.
     */
    void scanFiles();

    /**
     * @brief  Checks if a file exists on the SD card.
     * @param  fileName The name of the file to check.
     * @return True if the file exists, false otherwise.
     */
    bool fileExists(const char* fileName);

    /**
     * @brief  Deletes the specified file. This action cannot be undone.
     * @param  fileName The name of the file to be deleted.
     * @return True if the file is successfully deleted, false otherwise.
     */
    bool deleteFile(const char* fileName);

    /**
     * @brief  Opens a file for reading.
     * @param fileItem The FileItem struct containing the file to open.
     * @return True if the file is successfully opened, false otherwise.
     */
    bool openFileForRead(FileItem& fileItem);

    /**
     * @brief  Creates a file and opens it for reading.
     * @param fileItem The FileItem struct containing the file to open.
     * @return True if the file is successfully opened, false otherwise.
     */
    bool createFile(FileItem& fileItem);

    /**
    @brief Closes the specified file.
    @param fileItem The FileItem struct containing the file to close.
    @return True if the file is successfully closed, false otherwise.
    */
    bool closeFile(FileItem& fileItem);

    /**
     * @brief  Writes a message to the specified file, with debug functionality to write to Serial.
     * @param  fileType  Type of file to write to (logFile or dataFile).
     * @param  fileName  Name of the file to write to.
     * @param  message   Message to be written.
     */
    void print(FileItem& fileItem, const char* message);

    /**
     * @brief Reads a float value from a specified position in a file.
     * @param fileItem The file item to read from.
     * @param position The position in the file from which to read the float value.
     * @param value Reference to store the read float value.
     * @return True if the float value is successfully read, false otherwise.
     *         If reading fails at any step, an error message is printed and the file is closed.
     */
    bool readFloatFromFile(FileItem& fileItem, uint32_t position, float& value);

     /**
     * @brief Recieves a float value and writes the byte data into a file at a specified position
     * @param position The number of bytes from the start of the file to start writing
     * @param value The value to write as a float
     * @return True if the flaot value is successfully written, false otherwise.
     */
    bool writeFloatToFile(FileItem& fileItem, uint32_t position, float value);

private:
    // MEMBERS
    uint32_t logFileCounter;      // Counter for log files
    uint32_t dataFileCounter;     // Counter for data files

    // define classes with access to protected methods
    friend class DataLogger;


    /**
     * @brief  Updates the index file with the current file counters.
     */
    void updateIndexFile();

    /**
     * @brief  Loads the index file and reads the counters.
     */
    void loadIndexFile();

    /**
     * @brief  Creates a new log file with a unique name.
     */
    void createNewLogFile();

    /**
     * @brief  Creates a new data file with a unique name.
     */
    void createNewDataFile();

    /**
     * @brief Opens a file for writing.
     * @param fileItem The file item to be opened for writing.
     * @return True if the file is successfully opened for writing, false otherwise.
     *         If the file cannot be opened, an error message is printed.
     */
    bool openFileForWrite(FileItem& fileItem);

    /**
     * @brief Sets the file position to a specified location.
     * @param fileItem The file item whose position is to be set.
     * @param position The position to set in the file.
     * @return True if the position is successfully set, false otherwise.
     *         If the position cannot be set, an error message is printed and the file is closed.
     */
    bool setFilePosition(FileItem& fileItem, uint32_t position);
};

#endif // FILE_MANAGER_HPP
