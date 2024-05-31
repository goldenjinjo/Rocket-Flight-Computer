#include "positionalServo.hpp"

PositionalServo::PositionalServo() {
    initialize();
}

void PositionalServo::initialize() {
    // Assign pins to each servo
    servos[0] = {Servo(), SERVO_PIN_A};
    servos[1] = {Servo(), SERVO_PIN_B};
    servos[2] = {Servo(), SERVO_PIN_C};
    servos[3] = {Servo(), SERVO_PIN_D};

    // Activate all servos
    for (auto& servoObj : servos) {
        activate(servoObj);
    }
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
    for (auto& servoObj : servos) {
        activate(servoObj);
    }
}

void PositionalServo::deactivateAll() {
    // Deactivate all servos
    for (auto& servoObj : servos) {
        deactivate(servoObj);
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





