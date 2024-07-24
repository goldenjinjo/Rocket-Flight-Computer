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

class FlightStateMachine {
public:
    FlightStateMachine();
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

    float currentAltitude_;
    float currentVelocity_;
    float maxAltitude_;
    float maxVelocity_;
    const float APOGEE_VELOCITY_THRESHOLD = 0.5; // Example value in m/s
    const float LANDING_ALTITUDE = 10.0; // Example value in meters
};

#endif // FLIGHT_STATE_MACHINE_HPP