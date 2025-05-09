#include "IMUSensor.hpp"

IMUSensor::IMUSensor(TwoWire* i2c, uint8_t addr, uint8_t accelRange, uint16_t gyroRange)
    : imu(i2c,addr), accelRange(accelRange), gyroRange(gyroRange) {

    initialize();        
}

// Initialize the IMU sensor
void IMUSensor::initialize() {
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

    // Convert raw accelerometer data to physical units and store in the accelData array
    // divide by 1000 to convert from milli g, to g
    accelData[0] = static_cast<float>(accelArray_[0]) / 1000.0f;
    accelData[1] = static_cast<float>(accelArray_[1]) / 1000.0f;
    accelData[2] = static_cast<float>(accelArray_[2]) / 1000.0f;

    return accelData;
}

// Get gyroscope data as an array
float* IMUSensor::getGyroscopeData() {
    static float gyroData[3]; // Array to hold gyroscope data: [gyro_x, gyro_y, gyro_z]
    
    // Convert raw gyroscope data to physical units and store in the gyroData array
    // Divide by 1000 to convert from milli degrees per second
    gyroData[0] = static_cast<float>(gyroArray_[0]) / 1000.0f;
    gyroData[1] = static_cast<float>(gyroArray_[1]) / 1000.0f;
    gyroData[2] = static_cast<float>(gyroArray_[2]) / 1000.0f;

    return gyroData;
}

// configure polling rate for gyroscope and accelerometer on a scale of 1 to 10 (1.6 - 6664 Hz)
void IMUSensor::setPollRate(char rate) {
// Define the mapping of input byte values to output data rates
    // Corresponding output data rates in Hz
    const float dataRates[] = {12.5, 26, 52, 104, 208, 416, 833, 1666, 3332, 6664}; 
    const int numRates = sizeof(dataRates) / sizeof(dataRates[0]);

    // Ensure the rate value is within the valid range
    if (rate < 1) {
        rate = 1;
    }
    if(rate > numRates) {
        rate = numRates;
    }

    // Convert the byte input to an index for dataRates array
    int index = rate - 1; // Adjust to 0-based index

    // Set the output data rates for both gyroscope and accelerometer
    imu.Set_X_ODR(dataRates[index]);
    imu.Set_G_ODR(dataRates[index]);

}

// Wrapper method to get the polling rate (ODR) of the accelerometer
float IMUSensor::getAccelPollRate() {
    float accelPollRate;
    LSM6DSLStatusTypeDef accelStatus = imu.Get_X_ODR(&accelPollRate);
    if (accelStatus != LSM6DSL_STATUS_OK) {
        Serial.println("Error retrieving accelerometer polling rate.");
        return -1.0f; // Return a negative value to indicate error
    }
    return accelPollRate;
}

void IMUSensor::update() {
    imu.Get_G_Axes(gyroArray_);
    imu.Get_X_Axes(accelArray_);
}

// Wrapper method to get the polling rate (ODR) of the gyroscope
float IMUSensor::getGyroPollRate() {
    float gyroPollRate;
    LSM6DSLStatusTypeDef gyroStatus = imu.Get_G_ODR(&gyroPollRate);
    if (gyroStatus != LSM6DSL_STATUS_OK) {
        Serial.println("Error retrieving gyroscope polling rate.");
        return -1.0f; // Return a negative value to indicate error
    }
    return gyroPollRate;
}

float* IMUSensor::getAllData() {
    static float allData[6];
    float* accelData = getAccelerometerData();
    float* gyroData = getGyroscopeData();

    for (int i = 0; i < 3; ++i) {
        allData[i] = accelData[i];
        allData[i + 3] = gyroData[i];
    }

    return allData;
}
