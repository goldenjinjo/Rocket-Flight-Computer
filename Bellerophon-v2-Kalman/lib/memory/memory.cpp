 // track number of successful activations
    int successCounter = 0;

    // if (!sd.begin(CHIP_SELECT, SPI_SPEED)) {
    //     buzzerFailure();
    // }



    file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    file.println("time, temp, ax,ay,az,mag_x,mag_y,mag_z,gx,gy,gz");
    file.close();

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
    // create data file and start writing
    file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);

    // Format file to distingish between new data sets
    file.println("");
    file.println("-----FLIGHT LOG-----");
    file.println("");
    file.println("-----BEGIN INITIALIZATION SEQUENCE-----");
    file.println("");
  
    file.println("Bellerophon v3 Online!");
    file.println("Flash Chip Found...");


    // void logFlightEvent(const char* message, const unsigned long time) {
//     file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
//     file.print(time);
//     file.print(": ");
//     file.print(message);
//     file.println("");
//     file.close();
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