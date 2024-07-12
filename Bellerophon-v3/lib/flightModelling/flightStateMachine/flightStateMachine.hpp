#ifndef FLIGHT_STATE_MACHINE_HPP
#define FLIGHT_STATE_MACHINE_HPP

#include "flightStates.hpp"
#include "IMUSensor.hpp"
#include "pressureSensor.hpp"
#include "pyroController.hpp"
#include "pinAssn.hpp"
#include "configKeys.hpp"

class FlightStateMachine {
private:
    FlightState currentState;
    PressureSensor pressure;
    IMUSensor imu;
    PyroController pyroDrogue;
    PyroController pyroMain;


public:
    FlightStateMachine();
    void update();
    FlightState getCurrentState() const;
    void transitionToState(FlightState newState);

private:
    void handlePreLaunch();
    void handleAscent();
    void handleApogee();
    void handleDescentDrogue();
    void handleLowAltitudeDetection();
    void handleDescentMain();
    void handleLanding();
    void handleStageSeparation();
};

#endif // FLIGHT_STATE_MACHINE_HPP