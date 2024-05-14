#include "IMUSensor.hpp"

IMUSensor::IMUSensor(TwoWire* i2c, uint8_t addr, uint8_t accelRange, uint16_t gyroRange)
    : imu(i2c,addr), accelRange(accelRange), gyroRange(gyroRange) {
    
}



// Initialize the IMU sensor
void IMUSensor::init() {
    imu.begin();
    enable();
    
    // check sensitivity values, unusued for now
    LSM6DSLStatusTypeDef status = imu.Get_X_Sensitivity(&accelSense);
    if (status != LSM6DSL_STATUS_OK) {
        Serial.println("Error retrieving accelerometer sensitivity.");
        return;
    }
    status = imu.Get_G_Sensitivity(&gyroSense);
    if (status != LSM6DSL_STATUS_OK) {
        Serial.println("Error retrieving gyroscope sensitivity.");
        return;
    }
}

// Enable the IMU sensor
void IMUSensor::enable() {
    imu.Enable_X();
    imu.Enable_G();
    imu.Set_X_FS(accelRange);
    imu.Set_G_FS(gyroRange);
}

// Get accelerometer data as an array
// TODO: adjust for gravity, currently g = 1. Weird...
float* IMUSensor::getAccelerometerData() {
    static float accelData[3]; // Array to hold accelerometer data: [accel_x, accel_y, accel_z]

    int32_t lsm_acc[3]; // Array to store raw accelerometer data
    LSM6DSLStatusTypeDef status;

    status = imu.Get_X_Axes(lsm_acc);

    if (status != LSM6DSL_STATUS_OK) {
        // Handle error
        return nullptr;
    }

    // Convert raw accelerometer data to physical units and store in the accelData array
    accelData[0] = static_cast<float>(lsm_acc[0]) / 1000.0f;
    accelData[1] = static_cast<float>(lsm_acc[1]) / 1000.0f;
    accelData[2] = static_cast<float>(lsm_acc[2]) / 1000.0f;

    return accelData;
}

// Get gyroscope data as an array
float* IMUSensor::getGyroscopeData() {
    static float gyroData[3]; // Array to hold gyroscope data: [gyro_x, gyro_y, gyro_z]

    int32_t lsm_gyro[3]; // Array to store raw gyroscope data
    LSM6DSLStatusTypeDef status;

    status = imu.Get_G_Axes(lsm_gyro);
    if (status != LSM6DSL_STATUS_OK) {
        // Handle error
        return nullptr;
    }

    // Convert raw gyroscope data to physical units and store in the gyroData array
    gyroData[0] = static_cast<float>(lsm_gyro[0]) / 1000.0f;
    gyroData[1] = static_cast<float>(lsm_gyro[1]) / 1000.0f;
    gyroData[2] = static_cast<float>(lsm_gyro[2]) / 1000.0f;

    return gyroData;
}
