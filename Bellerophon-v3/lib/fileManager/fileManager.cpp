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

void FileManager::initializeFileItem(FileItem& fileItem, const char* name) {
    fileItem.name = name;
    // Initialize the type (FsFile object).
    fileItem.type = FsFile(); // Ensure type is in a known state
}


/* 
    FILE NAME CREATION METHODS
*/
void FileManager::initializeIndexFile() {
   
   initializeFileItem(indexFile, indexFileName);
   createFile(indexFile);

    // Initialize counters to 0
    logFileCounter = 0;
    dataFileCounter = 0;

    indexFile.type.open(indexFile.name, O_WRITE);
    // Write the initialized counters to the index file
    indexFile.type.write((uint8_t*)&logFileCounter, sizeof(logFileCounter));
    indexFile.type.write((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
    closeFile(indexFile);
}

void FileManager::loadIndexFile() {
    if (indexFile.type.open(indexFileName, O_RDWR)) {
        indexFile.type.read((uint8_t*)&logFileCounter, sizeof(logFileCounter));
        indexFile.type.read((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
        closeFile(indexFile);
    } else {
        initializeIndexFile();
    }
}

void FileManager::updateIndexFile() {
    if (!indexFile.type.open(indexFileName, O_RDWR | O_CREAT)) {
        initializeIndexFile();
    }

    indexFile.type.write((uint8_t*)&logFileCounter, sizeof(logFileCounter));
    indexFile.type.write((uint8_t*)&dataFileCounter, sizeof(dataFileCounter));
    closeFile(indexFile);
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


    initializeFileItem(logFile, logFileName);
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

    initializeFileItem(dataFile, dataFileName);
    // Increment the log file counter
    dataFileCounter++;
    // update index file for next file creation
    updateIndexFile(); 
}


/*
    FILES READING AND MANIPULATION
*/
// file directory reading
void FileManager::scanFiles() {
    Serial.println("Scanning files on the SD card:");
    sd.ls(LS_R);
}

void FileManager::updateFileList() {
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

bool FileManager::fileExists(const char* fileName) {
    return sd.exists(fileName);
}


// deleting files
bool FileManager::deleteFile(const char* fileName) {
    if (sd.exists(fileName)) {
        Serial.println("File Successfully Deleted");
        return sd.remove(fileName);

    } else {
        buzzerFailure();
        Serial.println("File not found, nothing deleted\n");
        return false;
    }
}

// opening files
bool FileManager::openFileForRead(FileItem& fileItem) {
     if (!fileItem.type.open(fileItem.name, O_READ)) {
        sd.errorHalt("Opening for read failed: ");
        Serial.println(fileItem.name);
        return false;
    }
    // else return true
    return true;
}

bool FileManager::closeFile(FileItem& fileItem) {
    if (!fileItem.type.close()) {
        Serial.print("Error closing file: ");
        Serial.println(fileItem.name);
        return false;
    }
    return true;
}

bool FileManager::createFile(FileItem& fileItem) {
     // Tries to create a file if file does not already exist
     if(!fileExists(fileItem.name)) {
        if (!fileItem.type.open(fileItem.name, O_RDWR | O_CREAT)) {
            sd.errorHalt("Error opening or creating file: ");
            Serial.println(fileItem.name);
            // false if file can not be created
            return false;
        }
        // close file after creation
        closeFile(fileItem);
        // true if file is created
        return true;
    }
    // false if file already exists
    return false;
}

void FileManager::readFile(FileItem& fileItem, long value) {
    openFileForRead(fileItem);
    fileItem.type.read((uint8_t*)&value, sizeof(value));
    closeFile(fileItem);
}

void FileManager::print(FileItem& fileItem, const char* message) {
    if (DEBUG) {
        Serial.print(message);
    }

    fileItem.type.open(fileItem.name, O_RDWR | O_CREAT | O_AT_END);
    fileItem.type.print(message);
    closeFile(fileItem);
}

