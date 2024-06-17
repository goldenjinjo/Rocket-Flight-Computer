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

DataLogger::DataLogger(SerialCommunicator& serialComm) : serialComm(serialComm), baro(1), imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000) {}

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
    } else {
        logEvent("Start LOG FILE");
    }

    // Sensor config
    imu.setPollRate(10); 

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

// Method to read data from a specific file and send it over serial
void DataLogger::readDataFromFile(const char* fileName) {
    if (!sd.exists(fileName)) {
        Serial.println("Data file not found.");
        return;
    }

    // Calculate CRC32 checksum
    crc.reset(); // Reset CRC32 object before calculating a new checksum
    FsFile file;
    if (!file.open(fileName, O_READ)) {
        sd.errorHalt("Opening for read failed");
        return;
    }

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
    if (!file.open(fileName, O_READ)) {
        sd.errorHalt("Opening for read failed");
    }

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
        // Handle timeout (optional)
        buzzerFailure();
        return;
    }
}

void DataLogger::sendAllFiles() {
    uint32_t timeout = 1800*1000; // 30 minute timeout

    // Update the file list to ensure we have the latest list of files
    updateFileList();

    // Wait for handshake message from the Python script
    if (serialComm.waitForMessage(HANDSHAKE_MESSAGE, timeout)) {
        serialComm.sendSerialMessage(ACK_MESSAGE);
        buzzerSuccess();
    } else {
        // Handle timeout (optional)
        buzzerFailure();
        Serial.println("TIME OUT");
        return;
    }

    // Iterate through each file name in the list
    for (const auto& fileName : fileNames) {
        // Check if the file name is the index file and skip it if so
        if (strcmp(fileName.c_str(), indexFileName) == 0) {
            continue; // Skip this iteration and move to the next file
        }

        // Read the data from the current file and send it over Serial
        readDataFromFile(fileName.c_str());
    }
    
    // Send the end-of-transmission acknowledgment
    serialComm.sendSerialMessage(ALL_FILES_SENT);
   

    // Wait for the next file to be sent
    if (!serialComm.waitForMessage(ALL_FILES_SENT_ACK, timeout)) {
        // Handle timeout (optional)
        buzzerFailure();
        return;
    } else {
        buzzerSuccess();
        LEDBlink(G_LED, 1000);
    }
}

void DataLogger::serialFileTransfer() {
    // Check for incoming serial message
    if (Serial.available()){
        String message = Serial.readStringUntil('\n');
        if (message == REQUEST_FILE_DOWNLOAD){
            LEDBlink(G_LED, 300);
            sendAllFiles();
        } else {
            LEDBlink(R_LED, 500);
            Serial.println(message);
            return;
        }
    }
}

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

        // Check if the file name is the index file and skip it if so
        if (strcmp(fileName.c_str(), indexFileName) == 0) {
            continue; // Skip this iteration and move to the next file
        }

        deleteFile(fileName.c_str());
    }
        
    Serial.println("All files deleted.");
    LEDBlink(R_LED, 2000);

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

    
}

bool DataLogger::fileExists(const char* fileName) {
    return sd.exists(fileName);
}

// SENSOR logging - logging mode
void DataLogger::logData() {
    
    // Write header to the new data file on first run of loop
    if (!fileExists(dataFileName)) {
        print(dataFile, dataFileName, "time, pressure, temp, ax, ay, az, gx, gy, gz\n");

    }

    if(DEBUG){
        delay(1000);
    }
    // get IMU data
    float* acc = imu.getAccelerometerData();
    float* gyro = imu.getGyroscopeData();

    float* sensorArray = new float[9];

    // put all sensors into sensor array
    // TODO: Abstract this to dataLogger class
    // TODO: improve speed. Last check, 13ms between loops. Way too slow.
    sensorArray[0] = baro.getPressure();
    sensorArray[1] = baro.getTemperature();  // Resolved by commenting out TDR bit check in SparkFun Lib. 
    sensorArray[2] = acc[0];
    sensorArray[3] = acc[1];
    sensorArray[4] = acc[2];
    sensorArray[5] = acc[3];
    sensorArray[6] = gyro[1];
    sensorArray[7] = gyro[2];
    sensorArray[8] = gyro[3];
    // log sensor data
    logData(sensorArray, 9);

    delete[] sensorArray; // Don't forget to free the allocated memory
}

