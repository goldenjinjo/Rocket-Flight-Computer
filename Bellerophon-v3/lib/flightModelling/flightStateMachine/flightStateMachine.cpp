#include "flightStateMachine.hpp"

FlightStateMachine::FlightStateMachine()
    : currentState(FlightState::PRE_LAUNCH), 
      pressureSensor(0), 
      altitudeProcessor(pressureSensor, 800, 10),
      imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000),
      pyroDrogue(PYRO_DROGUE, DROGUE_DELAY), 
      pyroMain(PYRO_MAIN, MAIN_DELAY) {
    // Initialize sensors and actuators
    pressureSensor.initialize();
}

void FlightStateMachine::update() {
    updateSensorData();
    handleStateTransition();
}

void FlightStateMachine::updateSensorData() {
    altitudeProcessor.update(); // Update altitude processor data
    currentAltitude = altitudeProcessor.getAltitude(); // Get the current altitude

    velocity = altitudeProcessor.getVerticalVelocity();

    Serial.println(pressureSensor.getData());
    Serial.println(currentAltitude);
    Serial.println(velocity);
    Serial.print("Max Altitude: ");
    Serial.println(altitudeProcessor.getMaxAltitude());
    Serial.print("Max Velocity: ");
    Serial.println(altitudeProcessor.getMaxVelocity());
    Serial.println("----");
}

void FlightStateMachine::handleStateTransition() {
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
    if (currentAltitude > 70) {
        transitionToState(FlightState::ASCENT);
    }
}

void FlightStateMachine::handleAscent() {
    // Ascent logic
   
    // Apogee detection logic
    if (velocity <= APOGEE_VELOCITY_THRESHOLD) {
        transitionToState(FlightState::APOGEE);
    }
}

void FlightStateMachine::handleApogee() {
    // Apogee logic
    // Trigger drogue parachute
    if(pyroDrogue.trigger()) {
        transitionToState(FlightState::DESCENT_DROGUE);
    }
}

void FlightStateMachine::handleDescentDrogue() {
    // Descent under drogue logic
    if (currentAltitude <= MAIN_DEPLOYMENT_ALT) {
        transitionToState(FlightState::LOW_ALTITUDE_DETECTION);
    }
}

void FlightStateMachine::handleLowAltitudeDetection() {
    // Trigger main parachutes
    if(pyroMain.trigger()){
        transitionToState(FlightState::DESCENT_MAIN);
    }
    
}

void FlightStateMachine::handleDescentMain() {
    // Descent under main logic
    if (currentAltitude <= LANDING_ALTITUDE) {
        transitionToState(FlightState::LANDING);
    }
}

void FlightStateMachine::handleLanding() {
    // Landing logic
    // Handle landing operations
}

void FlightStateMachine::handleStageSeparation() {
    // Stage separation logic
    // Placeholder for future implementation
}
