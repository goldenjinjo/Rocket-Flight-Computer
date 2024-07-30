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

class FlightStateMachine {
public:
    FlightStateMachine(BuzzerFunctions& buzzerFunc_, DataLogger& logger_);
    void update();
    FlightState getCurrentState() const;
    void transitionToState(FlightState newState);

private:
    FlightState currentState;
    PressureSensor pressureSensor;
    IMUSensor imu;
    std::shared_ptr<BarometricProcessor> altitudeProcessor;
    SensorFusion sensors;
    PyroController pyroDrogue;
    PyroController pyroMain;
    BuzzerFunctions buzzerFunc_;
    DataLogger logger_;

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

    float currentAltitude_;
    float currentVelocity_;
    float maxAltitude_;
    float maxVelocity_;
    float groundAltitude_;
    const float APOGEE_VELOCITY_THRESHOLD = 0.5; // meters per second
    const float LANDING_VEL_THRESHOLD = 1; // meters per second
};

#endif // FLIGHT_STATE_MACHINE_HPP