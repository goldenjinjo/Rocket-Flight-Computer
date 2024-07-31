#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include <CRC32.h>
#include "configKeys.hpp"
#include "serialCommunicator.hpp"
#include "fileManager.hpp"
#include "timer.hpp"


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
    DataLogger(SerialCommunicator& serialComm, FileManager& files);
    
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
     * @brief  Reads data from the specified file and prints it to the serial monitor.
     * @param  fileName The name of the file to be read.
     */
    void readDataFromFile(const char* fileName);

    /**
     * @brief  Deletes all files on the SD card by iterating through the fileNames vector.
     */
    void deleteAllFiles();

    /**
     * @brief  Sends all files over serial communication.
     */
    void sendAllFiles();

private:
    
    // ------------------------- MEMBERS ------------------------- //

    SerialCommunicator& serialComm;

    // sub class
    FileManager& files;

    char logBuffer = 100;         // Size of the log buffer
    // SdFs sd;                      // SD card instance
    CRC32 crc;                    // CRC32 object for checksum calculation

    uint32_t timeout = 1800*1000; // 30 minute timeout

    // ------------------------- METHODS ------------------------- //


};

#endif //DATA_LOGGER_HPP
