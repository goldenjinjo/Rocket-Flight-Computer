#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include <SdFat.h>
#include <CRC32.h>
#include <vector>
#include <string>
#include "deviceFunctions.hpp"
#include "config.hpp"

// sensors
#include "pressureSensor.hpp"
#include "IMUSensor.hpp"

/**
 * @file dataLogger.hpp
 * @brief This file contains the declaration of the DataLogger class, which is responsible for managing 
 *        data logging, file operations, and serial communication for a flight computer system. The class 
 *        provides functionality to initialize the logger, create log and data files, read and delete files, 
 *        and transfer files over a serial connection.
 */
class DataLogger {
public:
    /**
     * @brief  Constructor for DataLogger class.
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
     */
    void updateFileList();

    /**
     * @brief  Deletes all files on the SD card by iterating through the fileNames vector.
     */
    void deleteAllFiles();

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
     * @brief  Sends all files over serial communication.
     */
    void sendAllFiles();

    /**
     * @brief  Handles the serial file transfer process.
     */
    void serialFileTransfer();

    
    void logData();

    bool fileExists(const char* fileName);

    // ------------------------- MEMBERS ------------------------- //
    // Tracks time since program inception
    unsigned long currentTime = millis();
    // Member to hold array of file names
    std::vector<std::string> fileNames;
    // Member variables for file names
    static const uint8_t maxFileNameLength = 30; // Maximum length for file names
    char logFileName[maxFileNameLength];         // Name of the log file
    char dataFileName[maxFileNameLength];        // Name of the data file


    // Sensor Objects
    // set oversample rate (lower, faster)
    pressureSensor baro;
    // Change address to low or high based on PCB design
    IMUSensor imu;

private:
    // ------------------------- MEMBERS ------------------------- //
    char logBuffer = 100;         // Size of the log buffer
    FsFile logFile;               // File for logging events
    FsFile dataFile;              // File for logging data
    FsFile indexFile;             // File for tracking file naming counters
    SdFs sd;                      // SD card instance
    CRC32 crc;                    // CRC32 object for checksum calculation
    uint32_t logFileCounter;      // Counter for log files
    uint32_t dataFileCounter;     // Counter for data files
    const char* indexFileName = "index.dat"; // Name of the index file

    // ------------------------- METHODS ------------------------- //
    /**
     * @brief  Writes a message to the specified file, with debug functionality to write to serial.
     * @param  fileType  Type of file to write to (logFile or dataFile).
     * @param  fileName  Name of the file to write to.
     * @param  message   Message to be written.
     */
    void print(FsFile& fileType, const char* fileName, const char* message);

    /**
     * @brief  Initializes the index file with counters set to 0.
     */
    void initializeIndexFile();

    /**
     * @brief  Waits for a specific message on the serial port.
     * @param  expectedMessage The message to wait for.
     * @param  timeout         The timeout period in milliseconds.
     * @return True if the expected message is received, false otherwise.
     */
    bool waitForMessage(const String& expectedMessage, uint32_t timeout);

    /**
     * @brief  Sends a message over the serial port.
     * @param  message The message to be sent.
     */
    void sendSerialMessage(const String& message);

};

#endif //DATA_LOGGER_HPP
