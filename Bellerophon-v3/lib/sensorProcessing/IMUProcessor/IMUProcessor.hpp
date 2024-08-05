#ifndef IMU_PROCESSOR_HPP
#define IMU_PROCESSOR_HPP

#include "sensorProcessor.hpp"
#include "dataProcessor.hpp"
#include "IMUSensor.hpp"

class IMUProcessor : public SensorProcessor {

public:
    IMUProcessor(IMUSensor& imu, size_t historySize, float outlierThreshold = 10.0);

    void update() override;

    float* getRawData() const override;


     /**
     * @brief returns number of unique sensor values, wrapper of getNumValues()
     * 
     * @return size_t num of unique sensors
     */
    size_t getNumSensorValues() const override;

    std::string getSensorNames() const override;

protected:
    float getAltitude() const override {
        return 0.0;
    }

    float getVerticalVelocity() const override {
        return 0.0;
    }

    float getAcceleration() const override {
        return 0.0; // Assuming IMU class has this method
    }

    float getGroundAltitude() const override {
        // IMU can not calculate ground altitude
        return 0.0;
    }

    float getMaxAltitude() const override {
        return 0.0;
    }

    float getMaxVelocity() const override {
        return 0.0;
    }

    float getMaxAcceleration() const override {
        return 0.0;
    }

    

private:
    IMUSensor& imu_;
};

#endif // IMU_PROCESSOR_HPP