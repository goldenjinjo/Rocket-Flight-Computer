#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP


#include "SdFat.h"
#include "config.hpp"


// flash memory



class DataLogger {
public:
    DataLogger(const char* logFileName, const char* dataFileName);
    bool initialize();
    void print(const char* message);
    void logEvent(const char* message);
    void logData(float* data, size_t numFloats);
    bool deleteFile(const char* fileName);

    unsigned long currentTime = millis();

private:
    const uint8_t chipSelectPin = 10;
    char logBuffer = 100;
    const char* logFileName;
    const char* dataFileName;
    FsFile logFile;
    FsFile dataFile;
    SdFs sd;
};

#endif //DATA_LOGGER_HPP
