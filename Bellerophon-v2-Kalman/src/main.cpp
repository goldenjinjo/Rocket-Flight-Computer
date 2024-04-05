/*
 * Main File to be uploaded to the UNSW Rocketry Flight Computer
 * 
 * 
 * 
 */

// Import Libraries
#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <SPI.h>
#include "SdFat.h"
#include "sdios.h"
#include <Adafruit_Sensor.h>
#include "pinAssn.hpp"
#include "deviceFunctions.hpp"
#include "constants.hpp"
#include "config.hpp"
#include "Adafruit_BNO055.h"
#include <BasicLinearAlgebra.h>


using namespace BLA;

//Create an instance of the objects
MPL3115A2 baro;
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
int state = 0;
/*
    State 0: Pre-launch -> Launch Detect
    State 1: Inflight -> Apogee Detect
    State 2: First Deploy -> Deploy Command Sent
    State 3: Second Deploy -> Deploy Command Sent
    State 4: Descent -> Ground Detect
    Stage 5: Ground
*/

BLA::Matrix<2,3> H;
BLA::Matrix<3,3> Q; 
BLA::Matrix<2,2> R; 

BLA::Matrix<3,3> P;
BLA::Matrix<3,2> K;

BLA::Matrix<3> estimate;
//Currently unused
//unsigned long startUpTime;

unsigned long loopStartTime;

unsigned long previousTime;

float drogueTime;
float mainTime;
float apogeeTime;

float filteredAcc;
float altitudeBuffer[2];
int altitudeBufferIndex;
unsigned long altitudeBufferTime;

float measureAltitude();
float measureAcceleration();
void logFlightEvent(const char* message, const unsigned long time);
void logFlightData(const unsigned long time);

void setup() {
    //Currently Unused
    //startUpTime = millis();
    
    pinMode(R_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);
  
    pinMode(BUZZER, OUTPUT);

    // Set LEDS to low in case previous program left it HIGH
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, LOW);
  
    // play start up sequence
    startUp();


    Wire1.begin(); // Join i2c bus
    Serial.begin(9600);

    // track number of successful activations
    int successCounter = 0;

    if (!sd.begin(CHIP_SELECT, SPI_SPEED)) {
        buzzerFailure();
    }

    // Delete old versions of file if deleteFile condition is true
    if (deleteFile) {
        if (sd.exists(logFileName)) {
            sd.remove(logFileName);
        } 

        if (sd.exists(dataFileName)) {
            sd.remove(dataFileName);
        }
    }
    // create data file and start writing
    file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);

    // Format file to distingish between new data sets
    file.println("");
    file.println("-----FLIGHT LOG-----");
    file.println("");
    file.println("-----BEGIN INITIALIZATION SEQUENCE-----");
    file.println("");
  
    file.println("Bellerophon v2 Online!");
    file.println("Flash Chip Found...");
    successCounter++; 

    // Try to initialize!
    if(!bno.begin()) {
        file.println("BNO055 Not Dectected.");
        buzzerFailure();
    } else {
        file.println("BNO055 Found...");
        successCounter++;
    }

    if(!baro.begin(Wire1)) {
        file.println("MPL3115A2 Not Dectected.");
        buzzerFailure();
    } else {
        file.println("MPL3115A2 Found...");
        successCounter++;
    }

    // Communicate all sensors have activated
    if (successCounter == 3){
        buzzerSuccess();
        digitalWrite(B_LED, HIGH);
        delay(500);
        digitalWrite(B_LED, LOW);
        file.println("All Sensors Responding Nominally.");
    }

    file.close();
    
    file.open(dataFileName, O_RDWR | O_CREAT | O_AT_END);
    file.println("t,s,v,a,ax,ay,az,gx,gy,gz,mx,my,mz,p,T");
    file.close();
    
    // Configure the sensors
    baro.setOversampleRate(7); // Set Oversample to the recommended 128
    baro.enableEventFlags(); // Enable all three pressure and temp event flags 
  
    bno.setExtCrystalUse(true);
    bno.set16GRange();


    previousTime = 0;
    altitudeBufferTime = 0;

    // Initialize kalman filter
    estimate.Fill(0);

    H = {
        1, 0, 0,
        0, 0, 1
    };

    Q = {
        0, 0, 0,
        0, 0, 0,
        0, 0, SIGMA_M*SIGMA_M
    };

    R = {
        SIGMA_S*SIGMA_S, 0,
        0, SIGMA_A*SIGMA_A
    };

    altitudeBufferIndex = 0;
    altitudeBuffer[0] = 0;
    altitudeBuffer[1] = 0;
}

// MAIN LOOP
void loop()
{
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - previousTime) / 1000.0;
    if (previousTime == 0) {
        deltaTime = 0;
    }
    
    previousTime = currentTime;

    if (!loopStartTime) {
        loopStartTime = currentTime;
    }

    float h_measure = measureAltitude();
    
    // Disable kalman filter on ground
    if (state != 6) {
        BLA::Matrix<3,3> phi = {
            1, deltaTime, deltaTime*deltaTime/2,
            0, 1, deltaTime,
            0, 0, 1
        };

        BLA::Matrix<3,3> P_minus = phi*P*(~phi) + Q;

        BLA::Matrix<2,2> V = H*P_minus*(~H) + R;
        Invert(V);

        K = P_minus * ~H * V;

        P = (BLA::Identity<3>() - K*H) * P_minus;

        float a_measure = measureAcceleration();
        BLA::Matrix<2> z = {h_measure, a_measure};

        BLA::Matrix<3> predict = phi * estimate;
        estimate = predict + K * (z - H*predict);
    
        logFlightData(loopStartTime - currentTime);
    }

    if (state == 0) {
        // Pre-launch
        if (altitudeBufferTime - currentTime >= ALTITUDE_BUFFER_PERIOD) {
            altitudeBufferTime = currentTime;
            altitudeBuffer[altitudeBufferIndex] = h_measure;
            altitudeBufferIndex++;
            altitudeBufferIndex %= 2;
        }
        
        // Detect Launch
        if (estimate(1) >= LAUNCH_VEL_THRESHOLD && estimate(2) > LAUNCH_ACC_THRESHOLD) {
            state = 1;
            
            String event = "Launch Detected! Launch Altitude: ";
            altitudeBufferIndex++;
            altitudeBufferIndex %= 2;
            event.concat(altitudeBuffer[altitudeBufferIndex]);
            logFlightEvent(event.c_str(), loopStartTime - currentTime);
        }
    } else if (state == 1) {
        // Inflight
        
        // Detect Apogee
        if (estimate(1) > 0) {
            // While going up, set the apogeeTime to the current time
            apogeeTime = millis();
        }

        // If more than a small amount of time has passed, since the apogeeTime, consider apogee detected
        if ((millis() - apogeeTime) > APOGEE_TIMER) {
            state = 2;
            drogueTime = millis();
            logFlightEvent("Apogee Detected!", loopStartTime - currentTime);
        }
    } else if (state == 2) {
        // First Deploy

        if ((millis() - drogueTime) >= (drogueDelay - EPSILON) * 1000) {
            digitalWrite(dualDeploy ? PYRO_DROGUE : PYRO_MAIN, HIGH);
            state = dualDeploy ? 3 : 5;
            logFlightEvent("Fired first deployment charge!", loopStartTime - currentTime);
        }
    } else if (state == 3) {
        // Detect Main Altitude

        //Only rely on barometer for main deployment as accelerometer is likely wrong
        if (h_measure <= mainAltitude + altitudeBuffer[altitudeBufferIndex]) {
            state = 4;
            mainTime = millis();
            logFlightEvent("Main Deployment Altitude Detected!", loopStartTime - currentTime);
        }
    } else if (state == 4) {
        // Second Deploy

        if ((millis() - mainTime) >= (mainDelay - EPSILON) * 1000) {
            digitalWrite(PYRO_MAIN, HIGH);
            state = 5;
            logFlightEvent("Fired second deployment charge!", loopStartTime - currentTime);
        }
    } else if (state == 5) {
        // Decsent

        if (abs(estimate(1)) < LANDING_VEL_THERSHOLD) {
            state = 6;
            logFlightEvent("Landing Detected!", loopStartTime - currentTime);
        }
    } else if (state == 6) {
        // Ground

        // Disable pyro lines
        digitalWrite(PYRO_DROGUE, LOW);
        digitalWrite(PYRO_MAIN, LOW);

        // Activate Buzzer
        tone(BUZZER, 500);
        delay(1000);
        noTone(BUZZER);
    } 
    
}

float measureAltitude() {
    float P = baro.readPressure();
    float T = baro.readTemp();
    return pow(P_0/P, EXP)*(T + C_TO_K)/L_B;
}

float measureAcceleration() {
    imu::Vector<3> accel = bno.getVector(bno.VECTOR_ACCELEROMETER);

    // Assume accelerometer is inverted after apogee
    int direction = state < 2 ? 1 : -1;
    
    //Only the x direction is taken, simplifies filter by assuming only vertical flight
    float verticalAccel = direction * accel[0] - G_OFFSET;

    //Initialize recursive filter if not done already.
    if (!filteredAcc) {
        filteredAcc = verticalAccel;
    } else {
        //Recursive filter to smooth out noise from accelerometer
        filteredAcc = (1 - RECURSIVE_CONSTANT) * filteredAcc + RECURSIVE_CONSTANT * verticalAccel;
    }

    return filteredAcc;
}

void logFlightEvent(const char* message, const unsigned long time) {
    file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
    file.print(time);
    file.print(": ");
    file.print(message);
    file.println("");
    file.close();
}

void logFlightData(const unsigned long time) {
    file.open(logFileName, O_RDWR | O_CREAT | O_AT_END);
    
    file.print(time);
    file.print(",");

    //Kalman Filter estimates
    file.print(estimate(0));
    file.print(",");
    file.print(estimate(1));
    file.print(",");
    file.print(estimate(2));
    file.print(",");

    //Accelerometer Data    
    imu::Vector<3> data = bno.getVector(bno.VECTOR_ACCELEROMETER);
    file.print(data[0]);
    file.print(",");
    file.print(data[1]);
    file.print(",");
    file.print(data[2]);
    file.print(",");

    //Gyroscope Date
    data = bno.getVector(bno.VECTOR_GYROSCOPE);
    file.print(data[0]);
    file.print(",");
    file.print(data[1]);
    file.print(",");
    file.print(data[2]);
    file.print(",");
    
    //Magnetometer Data
    data = bno.getVector(bno.VECTOR_MAGNETOMETER);
    file.print(data[0]);
    file.print(",");
    file.print(data[1]);
    file.print(",");
    file.print(data[2]);
    file.print(",");
    
    //Barometer Data
    file.print(baro.readPressure());
    file.print(",");
    file.print(baro.readTemp());
    file.println("");

    file.close();
}