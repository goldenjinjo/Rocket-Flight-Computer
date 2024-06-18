#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <SdFat.h>
#include <Arduino.h>
#include "config.hpp"
#include "constants.hpp"
#include "pinAssn.hpp"

class FileManager {
public:

    // MEMBERS
    FsFile logFile;               // File for logging events
    FsFile dataFile;              // File for logging data
    FsFile indexFile;             // File for tracking file naming counters
    const char* indexFileName = "index.dat"; // Name of the index file

    static const uint8_t maxFileNameLength = 30; // Maximum length for file names
    char logFileName[maxFileNameLength];         // Name of the log file
    char dataFileName[maxFileNameLength];        // Name of the data file

    FileManager();

    /**
     * @brief  Initializes flash memory and file names
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();

private:

    // MEMBERS
    uint32_t logFileCounter;      // Counter for log files
    uint32_t dataFileCounter;     // Counter for data files
    SdFs sd;                      // SD card instance



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
