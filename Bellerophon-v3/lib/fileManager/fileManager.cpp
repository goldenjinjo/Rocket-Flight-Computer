#include "FileManager.hpp"

FileManager::FileManager() {}


bool FileManager::initialize() {
    
    if (!sd.begin(CHIP_SELECT, SPI_FULL_SPEED)) {
        Serial.println("SD card initialization failed.\n");
        return false;
    }
    // Load the index file and read the counters
    loadIndexFile();
    // Initialize log and data file names
    createNewLogFile();
    createNewDataFile();
    // Update index file
    updateIndexFile();
}

void FileManager::initializeIndexFile() {
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

void FileManager::loadIndexFile() {
    if (indexFile.open(indexFileName, O_RDWR)) {
        indexFile.read((uint8_t*)&logFileCounter, sizeof(logFileCounter));
        indexFile.read((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
        indexFile.close();
    } else {
        initializeIndexFile();
    }
}

void FileManager::updateIndexFile() {
    if (!indexFile.open(indexFileName, O_RDWR | O_CREAT)) {
        initializeIndexFile();
    }

    indexFile.write((uint8_t*)&logFileCounter, sizeof(logFileCounter));
    indexFile.write((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
    indexFile.close();
}

void FileManager::createNewLogFile() {
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

void FileManager::createNewDataFile() {
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
}
