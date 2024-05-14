#include "dataLogger.hpp"
    
DataLogger::DataLogger(const char* logFileName, const char* dataFileName) 
    : logFileName(logFileName), dataFileName(dataFileName) {}


void DataLogger::print(const char* message) {
    if (DEBUG) {
        Serial.print(message);
    } else {
        logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
        logFile.print(message);
        logFile.close();
    }
}

bool DataLogger::initialize() {
    if (!sd.begin(chipSelectPin, SPI_FULL_SPEED)) {
        print("SD card initialization failed.\n");
        return false;
    }

    // Create or append to the data file
    if (!dataFile.open(dataFileName, O_RDWR | O_CREAT | O_AT_END)) {
        print("Failed to open data file.\n");
        return false;
    }

    // Append to the log file
    if (!logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END)) {
        print("Failed to open log file.\n");
        return false;
    }

    // Print debug warning
    if(DEBUG){
        print("Warning! DEBUG Enabled.\n");
    }

    // Write header to data file
    print("time, temp, ax, ay, az, mag_x, mag_y, mag_z, gx, gy, gz");

    // Format log file
    print("\n-----FLIGHT LOG-----\n\n");
    print("-----BEGIN INITIALIZATION SEQUENCE-----\n\n");
    print("Bellerophon v3.5 Online!\n");
    print("Flash Chip Found...\n");

    return true;
}

void DataLogger::logEvent(const char* message, unsigned long time) {
    char buffer[50]; // Adjust buffer size as needed
    snprintf(buffer, sizeof(buffer), "%lu: %s\n", time, message);
    print(buffer);
}

void DataLogger::logData(unsigned long time, float* data, size_t numFloats) {
    char buffer[200]; // Adjust buffer size as needed
    int offset = snprintf(buffer, sizeof(buffer), "%lu,", time);
    for (size_t i = 0; i < numFloats; ++i) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%f,", data[i]);
    }
    snprintf(buffer + offset - 1, 2, "\n"); // Replace the last comma with a newline
    print(buffer);
}

/**
 * @brief  Deletes specified file. No going back.
 * @param  fileName the name of the file
 */
bool DataLogger::deleteFile(const char* fileName) {
    if (sd.exists(fileName)) {
        return sd.remove(fileName);
    } else {
        print("File not found, nothing deleted\n");
        return false;
    }
}