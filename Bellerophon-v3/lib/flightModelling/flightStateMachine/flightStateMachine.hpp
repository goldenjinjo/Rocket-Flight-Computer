#ifndef FLIGHT_STATE_MACHINE_HPP
#define FLIGHT_STATE_MACHINE_HPP

#include "flightStates.hpp"
#include "IMUSensor.hpp"
#include "pressureSensor.hpp"
#include "pyroController.hpp"
#include "pinAssn.hpp"
#include "configKeys.hpp"
#include "barometricProcessor.hpp"
#include "sensorFusion.hpp"
#include "buzzerFunctions.hpp"
#include "dataLogger.hpp"
#include "IMUProcessor.hpp"

class FlightStateMachine {
public:
    FlightStateMachine(BuzzerFunctions& buzzerFunc_, DataLogger& logger_);
    void update();
    FlightState getCurrentState() const;
    void transitionToState(FlightState newState);

    void logSensorData(uint16_t delayTime);

private:
    FlightState currentState_;
    PressureSensor pressureSensor_;
    IMUSensor imu_;
    std::shared_ptr<BarometricProcessor> altitudeProcessor_;
    std::shared_ptr<IMUProcessor> imuProcessor_;
    SensorFusion sensors_;
    PyroController pyroDrogue_;
    PyroController pyroMain_;
    BuzzerFunctions& buzzerFunc_;
    DataLogger& logger_;
    Timer loggingTimer_;
    float currentAltitude_;
    float currentVelocity_;
    float maxAltitude_;
    float maxVelocity_;
    float groundAltitude_;
    const float APOGEE_VELOCITY_THRESHOLD = 0.5; // meters per second
    const float LANDING_VEL_THRESHOLD = 1; // meters per second

    /**
     * @brief Initialize sensors and add them to sensor fusion.
     */
    void initializeSensors();

    void updateSensorData();
    void handleStateTransition();

    void handlePreLaunch();
    void handleAscent();
    void handleApogee();
    void handleDescentDrogue();
    void handleLowAltitudeDetection();
    void handleDescentMain();
    void handleLanding();
    void handleStageSeparation();
    void handleFailure();



    // for logging data
    template <typename SensorType>
    void appendSensorDataToArray(float* dataArray, size_t& offset, const SensorType& sensor) {
        float* allData = sensor.getAllData();
        size_t numValues = sensor.getNumValues(); // Get the number of values from the sensor

        for (size_t i = 0; i < numValues; ++i) {
            dataArray[offset++] = allData[i];
        }
    }
};

#endif // FLIGHT_STATE_MACHINE_HPP