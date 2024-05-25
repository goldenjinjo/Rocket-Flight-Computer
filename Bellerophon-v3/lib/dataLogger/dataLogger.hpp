#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include "SdFat.h"
#include "config.hpp"
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <CRC32.h>

class DataLogger {
public:
    /**
     * @brief  Constructor for DataLogger class.
     * @param  logFileName Name of the log file.
     * @param  dataFileName Name of the data file.
     */
    DataLogger();
    
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

     /**
     * @brief  Reads data from the specified file and prints it to the serial monitor.
     * @param  fileName The name of the file to be read.
     */
    void readDataFromFile(const char* fileName);

    /**
     * @brief  Scans all files on the SD card and prints their names to Serial.
     */
    void scanFiles();

    /**
     * @brief  Scans all files on the SD card and stores their names in a vector.
     * @param  fileNames A vector to hold the names of the files.
     */
    void updateFileList();

     /**
     * @brief  Deletes all files by iterating through the fileNames vector
     * @param  fileNames A vector to hold the names of the files.
     */
    void deleteAllFiles();


    void updateIndexFile();
    void loadIndexFile();
    void createNewLogFile();
    void createNewDataFile();

    void sendAllFiles();

    

    // ------------------------- MEMBERS ------------------------- //
    // Tracks time since program inception
    unsigned long currentTime = millis();
    // Member to hold array of file names
    std::vector<std::string> fileNames;
    // Member variables for file names
    static const uint8_t maxFileNameLength = 30; // TODO: make this dynamically sized
    char logFileName[maxFileNameLength];
    char dataFileName[maxFileNameLength];
    
    


private:
    // ------------------------- MEMBERS ------------------------- //
    char logBuffer = 100; // Size of the log buffer
    // Two file instances so they may be written to concurrently
    FsFile logFile;  // File for logging events
    FsFile dataFile; // File for logging data
    FsFile indexFile; // File for tracking file naming counters
    // SD card instance
    SdFs sd;

    CRC32 crc; // CRC32 object for checksum calculation

    uint32_t logFileCounter;
    uint32_t dataFileCounter;

    const char* indexFileName = "index.dat";

    
   

    // ------------------------- METHODS ------------------------- //
    /**
     * @brief  Writes a message to the specified file, with debug functionality to write to serial.
     * @param  fileType  Type of file to write to (logFile or dataFile).
     * @param  fileName  Name of the file to write to.
     * @param  message   Message to be written.
     */
    void print(FsFile& fileType, const char* fileName, const char* message);

    void initializeIndexFile();

    bool waitForMessage(const String& expectedMessage, uint32_t timeout);

    void sendSerialMessage(const String& message);
};

#endif //DATA_LOGGER_HPP
