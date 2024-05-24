#include "dataLogger.hpp"

DataLogger::DataLogger() {}

bool DataLogger::initialize() {
    if (!sd.begin(CHIP_SELECT, SPI_FULL_SPEED)) {
        Serial.println("SD card initialization failed.\n");
        return false;
    }

    // Load the index file and read the counters
    loadIndexFile();
    

    // Initialize log and data file names
    createNewLogFile();
    createNewDataFile();

    updateIndexFile();
    
    // Print debug warning
    if (DEBUG) {
        logEvent("Warning! DEBUG Enabled.\n");
    }
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

void DataLogger::readDataFromFile(const char* fileName) {
    // Wait for handshake message from the Python script
    while (true) {
        if (Serial.available()) {
            String message = Serial.readStringUntil('\n');
            if (message == "START_TRANSFER") {
                // Send acknowledgment back to Python script
                Serial.println("TRANSFER_ACK");
                buzzerSuccess();
                break;
            }
        }
    }

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

    // Send end-of-transmission message
    Serial.println("END_OF_TRANSMISSION");
}


// helper
void DataLogger::print(FsFile& fileType, const char* fileName, const char* message) {
    if (DEBUG) {
        Serial.print(message);
    }

    fileType.open(fileName, O_RDWR | O_CREAT | O_AT_END);
    fileType.print(message);
    fileType.close();
}

// file directory reading
void DataLogger::scanFiles() {
    Serial.println("Scanning files on the SD card:");
    sd.ls(LS_R);
}

void DataLogger::updateFileList() {
    // Ensure the vector is empty before starting
    fileNames.clear();

    // Open the root directory
    FsFile dir;
    if (!dir.open("/", O_READ)) {
        Serial.println("Failed to open root directory.");
        return;
    }

    // Create a file object to hold each file entry
    FsFile file;
    while (file.openNext(&dir, O_READ)) {
        char fileName[64];
        file.getName(fileName, sizeof(fileName));
        fileNames.push_back(std::string(fileName));
        file.close();
    }

    // Close the directory
    dir.close();
}


// deleting files
bool DataLogger::deleteFile(const char* fileName) {
    if (sd.exists(fileName)) {
        buzzerSuccess();
        Serial.println("File Successfully Deleted");
        return sd.remove(fileName);

    } else {
        buzzerFailure();
        logEvent("File not found, nothing deleted\n");
        return false;
    }
}

void DataLogger::deleteAllFiles() {
    // update fileNames array, just in case
    updateFileList();

    for (const auto& fileName : fileNames) {
        Serial.print("Deleting file: ");
        Serial.println(fileName.c_str());
        if (!sd.remove(fileName.c_str())) {
            Serial.println("Failed to delete file.");
        }
    }
    Serial.println("All files deleted.");

    // update fileNames array, which now should be empty
    updateFileList();
}

// file name creation
// TODO: generalise this for arbitary numver of possible variables
void DataLogger::loadIndexFile() {
    // Open the index file
    if (indexFile.open(indexFileName, O_RDWR)) {
        // Read the counters from the index file
        indexFile.read((uint8_t*)&logFileCounter, sizeof(logFileCounter));
        indexFile.read((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
        indexFile.close();
    } else {
        // If the index file doesn't exist, initialize counters to 0
        initializeIndexFile();
    }
}

void DataLogger::updateIndexFile() {
    // Open the index file
    if (!indexFile.open(indexFileName, O_RDWR | O_CREAT)) {
        // If the index file doesn't exist, create it and initialize counters to 0
        initializeIndexFile();
    }

    // Write the updated counters to the index file
    indexFile.write((uint8_t*)&logFileCounter, sizeof(logFileCounter));
    indexFile.write((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
    indexFile.close();
}

void DataLogger::initializeIndexFile() {
    // Open the index file for writing
    if (!indexFile.open(indexFileName, O_RDWR | O_CREAT)) {
        // If unable to open or create the file, handle error (e.g., log error message)
        return;
    }

    // Initialize counters to 0
    logFileCounter = 0;
    dataFileCounter = 0;

    // Write the initialized counters to the index file
    indexFile.write((uint8_t*)&logFileCounter, sizeof(logFileCounter));
    indexFile.write((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
    indexFile.close();
}

void DataLogger::createNewLogFile() {
    char tempFileName[maxFileNameLength];
  
    // Generate the new data file name based on the counter
    snprintf(tempFileName, maxFileNameLength, "%s%0*d%s", logFilePrefix, zeroPadding, \
     logFileCounter, logFileSuffix);
    // Print debug message
    if (DEBUG) {
        // add a debug prefix to the file name
        snprintf(logFileName, maxFileNameLength, "%s%s", debugPrefix, tempFileName);
        Serial.print("New log file created: ");
        Serial.println(logFileName);
    } else {
        strcpy(logFileName, tempFileName);
    }

    // Increment the log file counter
    logFileCounter++;
    // update index file for next file creation
    updateIndexFile();

}

void DataLogger::createNewDataFile() {
    char tempFileName[maxFileNameLength];

    // Generate the new data file name based on the counter
    snprintf(tempFileName, maxFileNameLength, "%s%0*d%s", dataFilePrefix, zeroPadding, \
     dataFileCounter, dataFileSuffix);

    if (DEBUG) {
        // add a debug prefix to the file name
        snprintf(dataFileName, maxFileNameLength, "%s%s", debugPrefix, tempFileName);
        Serial.print("New data file created: ");
        Serial.println(dataFileName);
    } else {
        strcpy(dataFileName, tempFileName);
    }

    // Increment the log file counter
    dataFileCounter++;
    // update index file for next file creation
    updateIndexFile();

    // Write header to the new data file
    print(dataFile, dataFileName, "time, pressure, temp, ax, ay, az, gx, gy, gz\n");
}

