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
    void logEvent(unsigned long time, const char* message);
    void logData(unsigned long time, float* data, size_t numFloats);
    bool deleteFile(const char* fileName);

private:
    const uint8_t chipSelectPin = 10;
    const char* logFileName;
    const char* dataFileName;
    FsFile logFile;
    FsFile dataFile;
    SdFs sd;
};

#endif //DATA_LOGGER_HPP
