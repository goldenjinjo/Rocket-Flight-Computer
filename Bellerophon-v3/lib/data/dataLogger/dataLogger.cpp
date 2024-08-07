#include "dataLogger.hpp"

/// TODO: create struct for unique file types (log and data files for right now)
/// TODO: Have unique identifier for communication messages e.g. "$" to use in WriteMessage()
/// TODO: properly set up data integrity checks (checksum incorrectly figured right now)
/// TODO: set up system to automatically suggest file deletion after successful download
/// TODO: configure data files only to generate in logging mode
/// TODO: call sensors inside dataLogger rather than main
/// TODO: remove magic numbers (e.g. maxFileLength and logBuffer)
/// TODO: break this up into more files / classes. It is getting too big.
/// TODO: add unique identifers for different Bellerophons in file name

DataLogger::DataLogger(SerialCommunicator& serialComm, FileManager& files) : \
serialComm(serialComm), files(files) {}

bool DataLogger::initialize() {
    
    // Initialize log and data file names
    files.createNewLogFile();
    files.createNewDataFile();

    // Print debug warning
    if (DEBUG) {
        logEvent("Warning! DEBUG Enabled.\n");
    } else {
        logEvent("Start LOG FILE");
    }

    return true;
}

void DataLogger::logEvent(const char* message) {
    int currentTime = Timer::currentTime();
    char buffer[logBuffer];
    snprintf(buffer, sizeof(buffer), "%lu: %s\n", currentTime, message);
    files.print(files.logFile, buffer);
}

void DataLogger::addDataFileHeading(const char* title) {
    // set initial state to false
    static bool headingSet = false;

    if (!files.fileExists(files.dataFileName)) {
        // reset state to false is ever deleted
        headingSet = false;
    }

    if(headingSet) {
        // only set the heading once
        return;
    }

    Serial.println("CREATING DATA FILE");
    files.print(files.dataFile, title);
    // start new line for data values
    files.print(files.dataFile, "\n");
    
    // prevent repeats of header creation
    headingSet = true;

}

void DataLogger::logData(float* data, size_t numFloats, uint8_t decimalPlaces) {
    uint32_t currentTime = Timer::currentTime();
    char buffer[logBuffer];
    int offset = snprintf(buffer, sizeof(buffer), "%lu,", currentTime);

    // Constrain decimalPlaces to a reasonable range, e.g., 0 to 10
    if (decimalPlaces > 10) {
        decimalPlaces = 10;
    }

    // Create a format string for the desired number of decimal places
    char formatString[10];
    snprintf(formatString, sizeof(formatString), "%%.%uf,", decimalPlaces);

    for (size_t i = 0; i < numFloats; ++i) {
        offset += snprintf(buffer + offset, sizeof(buffer) - offset, formatString, data[i]);
    }

    snprintf(buffer + offset - 1, 2, "\n"); // Replace the last comma with a newline
    files.print(files.dataFile, buffer);
}


// Method to read data from a specific file and send it over serial
void DataLogger::readDataFromFile(const char* fileName) {
    
    if (!files.fileExists(fileName)) {
        Serial.println("Data file not found.");
        return;
    }
    // Calculate CRC32 checksum
    crc.reset(); // Reset CRC32 object before calculating a new checksum
    FsFile file;
    file.open(fileName, O_READ);

    int data;
    while ((data = file.read()) >= 0) {
        crc.update(data); // Update the CRC32 checksum with each byte of data
    }
    uint32_t checksum = crc.finalize(); // Finalize the checksum calculation
    file.close();

    // Send file name and checksum to Python script
    Serial.print("FILE_NAME:");
    Serial.println(fileName);
    Serial.print("CHECKSUM:");
    Serial.println(checksum);

    // Open the file again for reading data
    //file.open(fileName, O_READ);
    file.open(fileName, O_READ);

    // Skip File read if serial comm sends this message
    if (serialComm.waitForMessage(FILE_COPY_MESSAGE, 100)){
        return;
    }

    // Read from the file until there's nothing else in it
    while ((data = file.read()) >= 0) {
        Serial.write(data); // Send each byte of data over serial
    }

    file.close();

    // Send end-of-transmission message
    Serial.println(END_OF_TRANSMISSION_MESSAGE);

    // handshake to finish file transfer
    if (!serialComm.waitForMessage(END_OF_TRANSMISSION_ACK, 1000)) {
        return;
    }
}

void DataLogger::sendAllFiles() {

    // Update the file list to ensure we have the latest list of files
    files.updateFileList();

    // Iterate through each file name in the list
    for (const auto& fileName : files.fileNames) {
        // Check if the file name is the index file and skip it if so
        if (strcmp(fileName.c_str(), files.indexFileName) == 0) {
            continue; // Skip this iteration and move to the next file
        }

        /// TODO: create array of files to exclude
        if (strcmp(fileName.c_str(), files.configFileName) == 0) {
            continue;
        }

        // Read the data from the current file and send it over Serial
        readDataFromFile(fileName.c_str());
    }
}

void DataLogger::deleteAllFiles() {
    // update files.fileNames array, just in case
    files.updateFileList();

    for (const auto& fileName : files.fileNames) {

        // Check if the file name is the index file and skip it if so
        if (strcmp(fileName.c_str(), files.indexFileName) == 0) {
            continue; // Skip this iteration and move to the next file
        }
        // Check if the file name is the config file and skip it if so
        if (strcmp(fileName.c_str(), files.configFileName) == 0) {
            continue; // Skip this iteration and move to the next file
        }

        files.deleteFile(fileName.c_str());
    }
        
    Serial.println("All files deleted.");

    // update fileNames array, which now should be empty
    files.updateFileList();
}

