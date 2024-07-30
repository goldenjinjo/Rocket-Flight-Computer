#include "flightStateMachine.hpp"

FlightStateMachine::FlightStateMachine(BuzzerFunctions& buzzerFunc_, DataLogger& logger_)
    : currentState(FlightState::PRE_LAUNCH), 
      pressureSensor(0), 
      imu(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000),
      altitudeProcessor(std::make_shared<BarometricProcessor>(pressureSensor, 150, 0.8)),
      pyroDrogue(PYRO_DROGUE, DROGUE_DELAY),
      pyroMain(PYRO_MAIN, MAIN_DELAY),
      buzzerFunc_(buzzerFunc_),
      logger_(logger_) {
    // Initialize sensors and actuators
    initializeSensors();
}

void FlightStateMachine::initializeSensors() {
    pressureSensor.initialize();
    
    // TODO: Add logic to determine if the sensor should be added
    // Example: Check if sensor is found and if stable readings can be identified
    
    // Add BarometricProcessor to SensorFusion
    sensors.addSensor(altitudeProcessor);
    // Example for adding other sensors (IMU)
    // sensorFusion.addSensor(std::make_shared<IMUProcessor>(imu));
}

void FlightStateMachine::update() {
    updateSensorData();
    handleStateTransition();
}

void FlightStateMachine::updateSensorData() {
    sensors.updateSensors(); // Update altitude processor data
    
    currentAltitude_ = sensors.getFusedAltitude(); // Get the current altitude
    currentVelocity_ = sensors.getFusedVerticalVelocity(); // get the current velocity
    groundAltitude_ = sensors.getGroundAltitude();
    maxAltitude_ = sensors.getMaxAltitude();
    maxVelocity_ = sensors.getMaxVelocity();


    imu.update();

    float* accelArray;
    accelArray = imu.getAccelerometerData();
    Serial.println(accelArray[0]);
    Serial.println(accelArray[1]);
    Serial.println(accelArray[2]);

    float* gyroArray;
    gyroArray = imu.getGyroscopeData();
    Serial.println(gyroArray[0]);
    Serial.println(gyroArray[1]);
    Serial.println(gyroArray[2]);


    Serial.println(pressureSensor.getData());
    Serial.println(currentAltitude_);
    Serial.println(currentVelocity_);
    Serial.print("Max Altitude: ");
    Serial.println(maxAltitude_);
    Serial.print("Max Velocity: ");
    Serial.println(maxVelocity_);
    Serial.print("Ground Altitude: ");
    Serial.println(groundAltitude_);
    Serial.print("Outlier Count: ");
    Serial.println(altitudeProcessor->getOutlierCount());
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
        case FlightState::FAILURE:
            handleFailure();
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
    buzzerFunc_.preLaunchTone();

    if (currentVelocity_ > LAUNCH_VEL_THRESHOLD) {
        transitionToState(FlightState::ASCENT);
        char logMessage[40];
        snprintf(logMessage, sizeof(logMessage), "Launch detected for velocity = %.2f", currentVelocity_);
        logger_.logEvent(logMessage);
        return;
    }

    // redudant altitude check
    if (currentAltitude_ > LAUNCH_ALTITUDE_THRESHOLD) {
        transitionToState(FlightState::ASCENT);
        char logMessage[40];
        snprintf(logMessage, sizeof(logMessage), "Launch detected for altitude = %.2f", currentAltitude_);
        logger_.logEvent(logMessage);
        return;
    }
}

void FlightStateMachine::handleAscent() {
    // Ascent logic
   
    // Apogee detection logic
    if (currentVelocity_ <= APOGEE_VELOCITY_THRESHOLD) {
        transitionToState(FlightState::APOGEE);
        char logMessage[40];
        snprintf(logMessage, sizeof(logMessage), "APOGEE DETECTED = %.2f METERS", currentAltitude_);
        logger_.logEvent(logMessage);
    }
}

void FlightStateMachine::handleApogee() {
    // Apogee logic
    if(maxAltitude_ < MINIMUM_APOGEE) {
        // Do not allow pyro to trigger if minimum apogee was not reached,
        /// TODO: create failure mode for this
        return;
    }
    // Trigger drogue parachute
    if(pyroDrogue.trigger()) {
        transitionToState(FlightState::DESCENT_DROGUE);
        logger_.logEvent("DROGUE DEPLOYED");
    }
}

void FlightStateMachine::handleDescentDrogue() {
    // Descent under drogue logic
    if (currentAltitude_ <= MAIN_DEPLOYMENT_ALT) {
        transitionToState(FlightState::LOW_ALTITUDE_DETECTION);
    }
}

void FlightStateMachine::handleLowAltitudeDetection() {
    // Trigger main parachutes
    if(pyroMain.trigger()){
        transitionToState(FlightState::DESCENT_MAIN);
        logger_.logEvent("MAIN DEPLOYED");
    }
    
}

void FlightStateMachine::handleDescentMain() {
    // Descent under main logic
    if (currentVelocity_ <= LANDING_VEL_THRESHOLD) {
        transitionToState(FlightState::LANDING);
        logger_.logEvent("LANDING DETECTED");
    }
}

void FlightStateMachine::handleLanding() {
    // infinitely play
    buzzerFunc_.landingTone();
}

void FlightStateMachine::handleStageSeparation() {
    // Stage separation logic
    // Placeholder for future implementation
}

void FlightStateMachine::handleFailure() {
    // failure logic
    // Placeholder for future implementation
}
