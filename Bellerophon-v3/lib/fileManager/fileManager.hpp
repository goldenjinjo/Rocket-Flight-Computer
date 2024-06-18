#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <SdFat.h>
#include <Arduino.h>
#include <vector>
#include "config.hpp"
#include "constants.hpp"
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"

class FileManager {
public:

    // MEMBERS
    FsFile logFile;               // File for logging events
    FsFile dataFile;              // File for logging data
    FsFile indexFile;             // File for tracking file naming counters
    const char* indexFileName = "index.dat"; // Name of the index file

    SdFs sd;                      // SD card instance

    static const uint8_t maxFileNameLength = 30; // Maximum length for file names
    char logFileName[maxFileNameLength];         // Name of the log file
    char dataFileName[maxFileNameLength];        // Name of the data file

    // Member to hold array of file names
    std::vector<std::string> fileNames;

    FileManager();

    /**
     * @brief  Initializes flash memory and file names
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();


    /**
     * @brief  Scans all files on the SD card and stores their names in a vector.
     */
    void updateFileList();

    /**
     * @brief  Scans all files on the SD card and prints their names to Serial.
     */
    void scanFiles();

    bool fileExists(const char* fileName);

    /**
     * @brief  Deletes the specified file. This action cannot be undone.
     * @param  fileName The name of the file to be deleted.
     * @return True if the file is successfully deleted, false otherwise.
     */
    bool deleteFile(const char* fileName);

    bool openFileForRead(FsFile& fileType, const char* fileName);

    bool closeFile(FsFile& fileType, const char* fileName);

    /**
     * @brief  Writes a message to the specified file, with debug functionality to write to serial.
     * @param  fileType  Type of file to write to (logFile or dataFile).
     * @param  fileName  Name of the file to write to.
     * @param  message   Message to be written.
     */
    void print(FsFile& fileType, const char* fileName, const char* message);

private:

    // MEMBERS
    uint32_t logFileCounter;      // Counter for log files
    uint32_t dataFileCounter;     // Counter for data files



    /**
     * @brief  Initializes the index file with counters set to 0.
     */
    void initializeIndexFile();

    
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

};

#endif // FILE_MANAGER_HPP
