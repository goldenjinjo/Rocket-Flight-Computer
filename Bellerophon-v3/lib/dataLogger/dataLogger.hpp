#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include "SdFat.h"
#include "config.hpp"
#include "pinAssn.hpp"

class DataLogger {
public:
    /**
     * @brief  Constructor for DataLogger class.
     * @param  logFileName Name of the log file.
     * @param  dataFileName Name of the data file.
     */
    DataLogger(const char* logFileName, const char* dataFileName);
    
    /**
     * @brief  Initializes the data logger.
     * @return True if initialization is successful, false otherwise.
     */
    bool initialize();
  
    /**
     * @brief  Logs an event message to the log file.
     * @param  message The message to be logged.
     */
    void logEvent(const char* message);

    /**
     * @brief  Logs an array of floating-point data to the data file.
     * @param  data      Pointer to the array of floating-point data.
     * @param  numFloats Number of floats in the array.
     */
    void logData(float* data, size_t numFloats);

    /**
     * @brief  Deletes the specified file. This action cannot be undone.
     * @param  fileName The name of the file to be deleted.
     * @return True if the file is successfully deleted, false otherwise.
     */
    bool deleteFile(const char* fileName);

    // ------------------------- METHODS ------------------------- //
    // Tracks time since program inception
    unsigned long currentTime = millis(); 

private:
    // ------------------------- MEMBERS ------------------------- //
    // Size of the log buffer
    char logBuffer = 100;  
    // Name of the log file
    const char* logFileName;  
    // Name of the data file
    const char* dataFileName; 
    // Two separate file instances so they may be written to concurrently
    FsFile logFile;  // File for logging events
    FsFile dataFile; // File for logging data
    // SD card instance
    SdFs sd; 

    // ------------------------- METHODS ------------------------- //
    /**
     * @brief  Writes a message to the specified file, with debug functionality to write to serial.
     * @param  fileType  Type of file to write to (logFile or dataFile).
     * @param  fileName  Name of the file to write to.
     * @param  message   Message to be written.
     */
    void print(FsFile fileType, const char* fileName, const char* message);
};

#endif //DATA_LOGGER_HPP
