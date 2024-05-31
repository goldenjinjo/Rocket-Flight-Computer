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

void PositionalServo::moveServosFromSerial() {
    // Check if data is available on the Serial port
    if (Serial.available()) {
        // Read the incoming data until a newline character is encountered
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any leading/trailing whitespace

        int len = input.length(); // Get the length of the input string
        int i = 0; // Initialize the index to parse the input

        // Loop through the input string
        while (i < len) {
            char servoID = input[i]; // Get the servo identifier (e.g., 'A', 'B', 'C', 'D')
            int positionStart = ++i; // Move to the next character which should be the start of the position number

            // Find the end of the position number
            while (i < len && isDigit(input[i])) {
                i++; // Increment index until a non-digit character is found
            }

            // Convert the position substring to an integer
            int position = input.substring(positionStart, i).toInt();

            // Move the corresponding servo to the specified position
            switch (servoID) {
                case 'A':
                    move(servos[0], position); // Move servo A
                    break;
                case 'B':
                    move(servos[1], position); // Move servo B
                    break;
                case 'C':
                    move(servos[2], position); // Move servo C
                    break;
                case 'D':
                    move(servos[3], position); // Move servo D
                    break;
                default:
                    // Handle invalid servoID if necessary
                    break;
            }

            // Skip any spaces between commands
            while (i < len && input[i] == ' ') {
                i++; // Increment index to skip spaces
            }
        }
    }
}






