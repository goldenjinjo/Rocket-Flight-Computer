#ifndef PIN_ASSN_HPP
#define PIN_ASSN_HPP

// Flash Memory
#define CHIP_SELECT 10

// MOSFET / PYRO pins
#define PYRO_DROGUE 15
#define PYRO_MAIN 7
// Pyro 3 and 4 disabled when connecting servo C and D
// #define PYRO_4 5
// #define PYRO_3 8    


// External UART connection
#define TX 20 // TX port
#define RX 21 // RX port


// Buzzer Pins
#define BUZZER 22

// LED pins
#define B_LED 2
#define G_LED 1
#define R_LED 0
#define pressureSensorLED 14
#define IMUSensorLED 6
#define flashMemoryLED 9

// servoMotor pins (directly soldered to Teensy in v3.5)
#define servoA 3
#define servoB 4
#define servoC 5 // NOTE: This is the same pin as PYRO 4
#define servoD 6 // NOTE: This is the same pin as PYRO 3


#endif // PIN_ASSN_HPP
