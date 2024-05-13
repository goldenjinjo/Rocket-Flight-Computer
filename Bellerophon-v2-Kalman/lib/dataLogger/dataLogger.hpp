#ifndef DATA_LOGGER_HPP
#define DATA_LOGGER_HPP


#include "SdFat.h"
#include "config.hpp"


// flash memory



class DataLogger {
public:
    DataLogger(const char* logFileName, const char* dataFileName);
    bool initialize();
    void logEvent(const char* message, unsigned long time);
    void logData(unsigned long time, int32_t* lsm_acc, int32_t* lsm_gyro);

private:
    const uint8_t chipSelectPin = 10;
    const char* logFileName;
    const char* dataFileName;
    FsFile logFile;
    FsFile dataFile;
    SdFs sd;
};

#endif //DATA_LOGGER_HPP
