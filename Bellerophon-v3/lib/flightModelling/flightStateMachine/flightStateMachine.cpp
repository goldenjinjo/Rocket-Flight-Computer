#include "flightStateMachine.hpp"

FlightStateMachine::FlightStateMachine(BuzzerFunctions& buzzerFunc, DataLogger& logger)
    : currentState_(FlightState::PRE_LAUNCH),
      pressureSensor_(0),
      imu_(&Wire, LSM6DSL_ACC_GYRO_I2C_ADDRESS_LOW, 16, 1000),
      altitudeProcessor_(std::make_shared<BarometricProcessor>(pressureSensor_, 150, 0.8)),
      imuProcessor_(std::make_shared<IMUProcessor>(imu_, 150, 0.8)),
      pyroDrogue_(PYRO_DROGUE, DROGUE_DELAY),
      pyroMain_(PYRO_MAIN, MAIN_DELAY),
      buzzerFunc_(buzzerFunc),
      logger_(logger) {
    // Initialize sensors_ and actuators
    initializeSensors();
}



void FlightStateMachine::initializeSensors() {
   
    imu_.setPollRate(10); 
    
    // TODO: Add logic to determine if the sensor should be added
    // Example: Check if sensor is found and if stable readings can be identified
    
    // Add BarometricProcessor to SensorFusion
    sensors_.addSensor(altitudeProcessor_);
    sensors_.addSensor(imuProcessor_);
}

void FlightStateMachine::update() {
    updateSensorData();
    handleStateTransition();
}


void FlightStateMachine::logSensorData(uint16_t delayTime) {

    loggingTimer_.start(delayTime);

    if(!loggingTimer_.hasElapsed()) {
        // do not log data if wait time is in effect
        return;
    }

    size_t numDataPoints = imu_.getNumValues() + pressureSensor_.getNumValues();
    float allData[numDataPoints];
    size_t offset = 0;
    appendSensorDataToArray(allData, offset, pressureSensor_);
    appendSensorDataToArray(allData, offset, imu_);
    
    // create title
    std::string sensorNames = "time," + pressureSensor_.getNames() + "," + imu_.getNames();
    logger_.addDataFileHeading(sensorNames.c_str());
    Serial.println(sensorNames.c_str());
    // log data
    logger_.logData(allData, numDataPoints);

    // DEBUG
    for (int i = 0; i < numDataPoints; ++i) {
        Serial.println(allData[i]);
    }

    Serial.println(currentAltitude_);
    Serial.println(currentVelocity_);
    Serial.print("Max Altitude: ");
    Serial.println(maxAltitude_);
    Serial.print("Max Velocity: ");
    Serial.println(maxVelocity_);
    Serial.print("Ground Altitude: ");
    Serial.println(groundAltitude_);
    Serial.print("Outlier Count: ");
    Serial.println(altitudeProcessor_->getOutlierCount());
    Serial.println("----");

    // reset timer for next cycle
    loggingTimer_.reset();

}

void FlightStateMachine::updateSensorData() {
    sensors_.updateSensors(); // Update altitude processor data
    
    currentAltitude_ = sensors_.getFusedAltitude(); // Get the current altitude
    currentVelocity_ = sensors_.getFusedVerticalVelocity(); // get the current velocity
    groundAltitude_ = sensors_.getGroundAltitude();
    maxAltitude_ = sensors_.getMaxAltitude();
    maxVelocity_ = sensors_.getMaxVelocity();

    imu_.update(); 
}

void FlightStateMachine::handleStateTransition() {
    switch (currentState_) {
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
    return currentState_;
}

void FlightStateMachine::transitionToState(FlightState newState) {
    currentState_ = newState;
}

void FlightStateMachine::handlePreLaunch() {
    // Pre-launch logic
    
    // play regular wait for launch tone, only in non debug mode
    if(!DEBUG) {
        buzzerFunc_.preLaunchTone();
    }
   
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
    logSensorData(0);
   
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
    logSensorData(0);
    if(maxAltitude_ < MINIMUM_APOGEE) {
        // Do not allow pyro to trigger if minimum apogee was not reached,
        /// TODO: create failure mode for this
        return;
    }
    // Trigger drogue parachute
    if(pyroDrogue_.trigger()) {
        transitionToState(FlightState::DESCENT_DROGUE);
        logger_.logEvent("DROGUE DEPLOYED");
    }
}

void FlightStateMachine::handleDescentDrogue() {
    // Descent under drogue logic
    logSensorData(500);
    if (currentAltitude_ <= MAIN_DEPLOYMENT_ALT) {
        transitionToState(FlightState::LOW_ALTITUDE_DETECTION);
    }
}

void FlightStateMachine::handleLowAltitudeDetection() {
    
    logSensorData(500);
    // Trigger main parachutes
    if(pyroMain_.trigger()){
        transitionToState(FlightState::DESCENT_MAIN);
        logger_.logEvent("MAIN DEPLOYED");
    }
    
}

void FlightStateMachine::handleDescentMain() {
    
    logSensorData(500);
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
