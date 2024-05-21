#include "dataLogger.hpp"

DataLogger::DataLogger(const char* logFileName, const char* dataFileName) 
    : logFileName(logFileName), dataFileName(dataFileName) {}


// TODO: Assess the performance of opening the file in initialization and keeping it open for the duration of the program
void DataLogger::print(FsFile& fileType, const char* fileName, const char* message) {
    if (DEBUG) {
        Serial.print(message);
    }

    fileType.open(fileName, O_RDWR | O_CREAT | O_AT_END);
    fileType.print(message);
    fileType.close();
    }

bool DataLogger::initialize() {
    if (!sd.begin(CHIP_SELECT, SPI_FULL_SPEED)) {
        logEvent("SD card initialization failed.\n");
        return false;
    }

    // Create or append to the data file
    if (!dataFile.open(dataFileName, O_RDWR | O_CREAT | O_AT_END)) {
        logEvent("Failed to open data file.\n");
        return false;
    }

    // Append to the log file
    if (!logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END)) {
        logEvent("Failed to open log file.\n");
        return false;
    }

    // Print debug warning
    if(DEBUG){
        logEvent("Warning! DEBUG Enabled.\n");
    }
    // Format log file
    logEvent("Bellerophon v3.5 Online!\n");

    // Write header to data file
    // TODO: change dataLogger to input activated sensors and dynamically write this
    print(dataFile, dataFileName, "time, pressure, temp, ax, ay, az gx, gy, gz \n");

    return true;
}

void DataLogger::logEvent(const char* message) {
    currentTime = millis();
    char buffer[logBuffer];
    snprintf(buffer, sizeof(buffer), "%lu: %s\n", currentTime, message);
    print(logFile, logFileName, buffer);
}

void DataLogger::logData(float* data, size_t numFloats) {
    currentTime = millis();
    char buffer[logBuffer];
    int offset = snprintf(buffer, sizeof(buffer), "%lu,", currentTime);
    for (size_t i = 0; i < numFloats; ++i) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, "%f,", data[i]);
    }
    snprintf(buffer + offset - 1, 2, "\n"); // Replace the last comma with a newline
    print(dataFile, dataFileName, buffer);
}


bool DataLogger::deleteFile(const char* fileName) {
    if (sd.exists(fileName)) {
        return sd.remove(fileName);
    } else {
        logEvent("File not found, nothing deleted\n");
        return false;
    }
}

void DataLogger::readDataFromFile(const char* fileName) {
    if (!sd.exists(fileName)) {
        Serial.println("Data file not found.");
        return;
    }

    FsFile file;
    if (!file.open(fileName, O_READ)) {
        sd.errorHalt("Opening for read failed");
    }

    // Read from the file until there's nothing else in it
    int data;
    while ((data = file.read()) >= 0) {
        Serial.write(data);
    }

    file.close();
}