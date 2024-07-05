#include "PositionalServo.hpp"

/*
PUBLIC
*/
PositionalServo::PositionalServo() {
    initialize();
}

PositionalServo::~PositionalServo() {
    deactivateAll();
}

void PositionalServo::moveServoRelativeToCenter(char id, int& relativePosition) {
    ServoObject* servoObj = findServoByID(id);
    
    // Ensure relativePosition is within the allowed range
    maxDeflectionCheck(relativePosition);
    
    if (servoObj) {
        int newPosition = servoObj->centerPos + relativePosition;
        moveServoByID(id, newPosition);
    }
}

void PositionalServo::stopByID(char id) {
    ServoObject* servoObj = findServoByID(id);
    if (servoObj) {
        stop(*servoObj);
    }
}

void PositionalServo::centerAllServoPositions() {
    for (auto& pair : servoMap) {
        stop(pair.second);
    }
}

int PositionalServo::maxSetAngleCheck(int position) {
    
    if (position < (minPos+maxDeflectionAngle)){
        position = minPos +maxDeflectionAngle;
    } 
    if (position > (maxPos- maxDeflectionAngle)){
        position = (maxPos - maxDeflectionAngle);
    } 
    return position;
}

/*
PRIVATE
*/

void PositionalServo::initialize() {
    // Assign pins and IDs to each servo and populate the map
    servoMap['A'] = {Servo(), SERVO_PIN_A, static_cast<int>(SERVO_A_CENTER_POSITION)};
    servoMap['B'] = {Servo(), SERVO_PIN_B, static_cast<int>(SERVO_B_CENTER_POSITION)};
    servoMap['C'] = {Servo(), SERVO_PIN_C, static_cast<int>(SERVO_C_CENTER_POSITION)};
    servoMap['D'] = {Servo(), SERVO_PIN_D, static_cast<int>(SERVO_D_CENTER_POSITION)};

    // Activate all servos
    activateAll();
    // set initial servo positions
    centerAllServoPositions();
}

void PositionalServo::moveServoByID(char id, int position) {
    ServoObject* servoObj = findServoByID(id);
    if (servoObj) {
        // Ensure position is within bounds
        boundaryCheck(position);
        // move servo
        move(*servoObj, position);
    }
}

PositionalServo::ServoObject* PositionalServo::findServoByID(char id) {
    auto it = servoMap.find(id);
    if (it != servoMap.end()) {
        return &it->second;
    } else {
        if (DEBUG) {
            Serial.print("Invalid servo ID: ");
            Serial.println(id);
        }
        return nullptr;
    }
}

void PositionalServo::stop(ServoObject& servoObj) {
    // Move the servo to the center position
    move(servoObj, servoObj.centerPos);
}

void PositionalServo::boundaryCheck(int& position) {
    if (position < minPos) position = minPos;
    if (position > maxPos) position = maxPos;
}

void PositionalServo::maxDeflectionCheck(int& position) {
     if (position > maxDeflectionAngle) {
        position = maxDeflectionAngle;
    } else if (position < -maxDeflectionAngle) {
        position = -maxDeflectionAngle;
    }
}

/*
ARDUINO SERVO class wrappers
*/
void PositionalServo::activate(ServoObject& servoObj) {
    // Attach the servo to its pin
    servoObj.servo.attach(servoObj.pin);
}

void PositionalServo::deactivate(ServoObject& servoObj) {
    // Detach the servo from its pin
    servoObj.servo.detach();
}

void PositionalServo::activateAll() {
    // Activate all servos
    for (auto& pair : servoMap) {
        activate(pair.second);
    }
}

void PositionalServo::deactivateAll() {
    // Deactivate all servos
    for (auto& pair : servoMap) {
        deactivate(pair.second);
    }
}

void PositionalServo::move(ServoObject& servoObj, uint8_t position) {
    // Move the servo to the specified position
    servoObj.servo.write(position);
}


