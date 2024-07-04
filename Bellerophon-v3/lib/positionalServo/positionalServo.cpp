#include "PositionalServo.hpp"

PositionalServo::PositionalServo() {
    initialize();
}

PositionalServo::~PositionalServo() {
    deactivateAll();
}

void PositionalServo::initialize() {
    // Assign pins and IDs to each servo and populate the map
    servoMap['A'] = {Servo(), SERVO_PIN_A};
    servoMap['B'] = {Servo(), SERVO_PIN_B};
    servoMap['C'] = {Servo(), SERVO_PIN_C};
    servoMap['D'] = {Servo(), SERVO_PIN_D};

    // Activate all servos
    activateAll();
}

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
    // Activate servo in case it was deactivated
    activate(servoObj);

    // Move the servo to the specified position
    servoObj.servo.write(position);
}

void PositionalServo::stop(ServoObject& servoObj) {
    // Move the servo to the zero position
    move(servoObj, servoZeroValue);
}

void PositionalServo::moveServoByID(char id, int position) {
    auto it = servoMap.find(id);
    if (it != servoMap.end()) {
        move(it->second, position);
    } else {
        if (DEBUG) {
            Serial.print("Invalid servo ID: ");
            Serial.println(id);
        }
    }
}









