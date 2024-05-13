
#include "dataLogger.hpp"
    
    // file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    // file.println("time, temp, ax,ay,az,mag_x,mag_y,mag_z,gx,gy,gz");
    // file.close();

DataLogger::DataLogger(const char* logFileName, const char* dataFileName) 
    : logFileName(logFileName) {}


bool DataLogger::initialize() {
    if (!sd.begin(chipSelectPin, SPI_FULL_SPEED)) {
        Serial.println("SD card initialization failed.");
        return false;
    }

    // Create or append to the data file
    if (!dataFile.open(dataFileName, O_RDWR | O_CREAT | O_AT_END)) {
        Serial.println("Failed to open data file.");
        return false;
    }

    // Append to the log file
    if (!logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END)) {
        Serial.println("Failed to open log file.");
        return false;
    }

    // Write header to data file
    dataFile.println("time, temp, ax, ay, az, mag_x, mag_y, mag_z, gx, gy, gz");
    dataFile.close();

    // Format log file
    logFile.println("");
    logFile.println("-----FLIGHT LOG-----");
    logFile.println("");
    logFile.println("-----BEGIN INITIALIZATION SEQUENCE-----");
    logFile.println("");
    logFile.println("Bellerophon v3.5 Online!");
    logFile.println("Flash Chip Found...");

    logFile.close();

    return true;
}

void DataLogger::logEvent(const char* message, unsigned long time) {
    logFile.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
    logFile.print(time);
    logFile.print(": ");
    logFile.print(message);
    logFile.println("");
    logFile.close();
}

void DataLogger::logData(unsigned long time, int32_t* lsm_acc, int32_t* lsm_gyro) {
    if (dataFile.open(dataFileName, O_RDWR | O_CREAT | O_AT_END)) {
        dataFile.print(time);
        dataFile.print(",");
        for (int i = 0; i < 3; ++i) {
            dataFile.print(lsm_acc[i]);
            dataFile.print(",");
        }
        for (int i = 0; i < 3; ++i) {
            dataFile.print(lsm_gyro[i]);
            dataFile.print(",");
        }
        // Add more data logging here if needed

        dataFile.println();
        dataFile.close();
    }
}


    // Delete old versions of file if deleteFile condition is true
    // TODO: Change to function, and remove deleteFile config
    // if (deleteFile) {
    //     if (sd.exists(logFileName)) {
    //         sd.remove(logFileName);
    //     } 

    //     if (sd.exists(dataFileName)) {
    //         sd.remove(dataFileName);
    //     }
    // }




// void logFlightData(const unsigned long time) {
        
//     file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    
//     file.print(time);
//     file.print(",");

//    // Get LSM IMU Sensor Data
//     int32_t lsm_acc[3];
//     int32_t lsm_gyro[3];
//     lsm.Get_X_Axes(lsm_acc);
//     lsm.Get_G_Axes(lsm_gyro);

//     file.print(lsm_acc[0]);
//     file.print(",");
//     file.print(lsm_acc[1]);
//     file.print(",");
//     file.print(lsm_acc[2]);
//     file.print(",");

//     file.print(lsm_gyro[0]);
//     file.print(",");
//     file.print(lsm_gyro[1]);
//     file.print(",");
//     file.print(lsm_gyro[2]);
//     file.print(",");

    
//     //Barometer Data
//     file.print(baro.readPressure());
//     file.print(",");
//     file.print(baro.readTemp());
    
    


//     if(icm_enable == true){
//         sensors_event_t accel;
//         sensors_event_t gyro;
//         sensors_event_t mag;
//         sensors_event_t temp;
//         icm.getEvent(&accel, &gyro, &temp, &mag);

//         float a_z = accel.acceleration.z;

//         if(abs(a_z) > LAUNCH_ACC_THRESHOLD){
//             Serial.println("launch detect");
//             int launch_time = millis();

//         }


//         file.print(",");
//         file.print(temp.temperature);
//         file.print(",");
//         file.print(accel.acceleration.x);
//         file.print(",");
//         file.print(accel.acceleration.z);
//         file.print(",");
//         file.print(accel.acceleration.z);
//         file.print(",");
//         file.print(mag.magnetic.x);
//         file.print(",");
//         file.print(mag.magnetic.y);
//         file.print(",");
//         file.print(mag.magnetic.z);
//         file.print(",");
//         file.print(gyro.gyro.x);
//         file.print(",");
//         file.print(gyro.gyro.y);
//         file.print(",");
//         file.print(gyro.gyro.z);
//         file.print(",");
//         file.println();
//     } else {
//         file.println("");
//     }

//     file.close();
// }