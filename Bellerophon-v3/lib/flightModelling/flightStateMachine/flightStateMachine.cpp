// FlightStateMachine.cpp
#include "FlightStateMachine.hpp"

FlightStateMachine::FlightStateMachine() : currentState(FlightState::PRE_LAUNCH), pressure(1), imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000),
pyroDrogue(PYRO_DROGUE, DROGUE_DELAY), pyroMain(PYRO_MAIN, MAIN_DELAY)  {
    // Initialize sensors and actuators
}

void FlightStateMachine::update() {
    switch (currentState) {
        case FlightState::PRE_LAUNCH:
            handlePreLaunch();
            break;
        case FlightState::ASCENT:
            handleAscent();
            break;
        case FlightState::APOGEE:
            handleApogee();
            break;
        case FlightState::DESCENT_DROGUE:
            handleDescentDrogue();
            break;
        case FlightState::LOW_ALTITUDE_DETECTION:
            handleLowAltitudeDetection();
            break;
        case FlightState::DESCENT_MAIN:
            handleDescentMain();
            break;
        case FlightState::LANDING:
            handleLanding();
            break;
        case FlightState::STAGE_SEPARATION:
            handleStageSeparation();
            break;
    }
}

FlightState FlightStateMachine::getCurrentState() const {
    return currentState;
}

void FlightStateMachine::transitionToState(FlightState newState) {
    currentState = newState;
}

void FlightStateMachine::handlePreLaunch() {
    // Pre-launch logic
    // if (/* condition to start ascent */) {
    //     transitionToState(FlightState::ASCENT);
    // }
}

void FlightStateMachine::handleAscent() {
    // Ascent logic
    // if (/* condition to detect apogee */) {
    //     transitionToState(FlightState::APOGEE);
    // }
}

void FlightStateMachine::handleApogee() {
    // Apogee logic
    transitionToState(FlightState::DESCENT_DROGUE);
}

void FlightStateMachine::handleDescentDrogue() {
    // Descent under drogue logic
    // if (/* condition for low altitude detection */) {
    //     transitionToState(FlightState::LOW_ALTITUDE_DETECTION);
    // }
}

void FlightStateMachine::handleLowAltitudeDetection() {
    // Low altitude detection logic
    transitionToState(FlightState::DESCENT_MAIN);
}

void FlightStateMachine::handleDescentMain() {
    // Descent under main logic
    // if (/* condition for landing */) {
    //     transitionToState(FlightState::LANDING);
    // }
}

void FlightStateMachine::handleLanding() {
    // Landing logic
    // Handle landing operations
}

void FlightStateMachine::handleStageSeparation() {
    // Stage separation logic
    // Placeholder for future implementation
}
