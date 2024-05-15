#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP

#include "SdFat.h"
#include "config.hpp"

class DataLogger {
public:
    DataLogger(const char* logFileName, const char* dataFileName);
    
    bool initialize();
  
    void logEvent(const char* message);

    void logData(float* data, size_t numFloats);

    /**
     * @brief  Deletes specified file. No going back.
     * @param  fileName the name of the file
     */
    bool deleteFile(const char* fileName);

    unsigned long currentTime = millis();

private:
    const uint8_t chipSelectPin = 10;
    char logBuffer = 100;
    const char* logFileName;
    const char* dataFileName;
    
    // Two separate file instances so they may be written to concurrently
    FsFile logFile;
    FsFile dataFile;
    SdFs sd;

    void print(FsFile fileType, const char* fileName, const char* message);
};

#endif //DATA_LOGGER_HPP
