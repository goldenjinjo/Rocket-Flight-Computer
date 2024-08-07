#ifndef FLIGHT_STATE_MACHINE_HPP
#define FLIGHT_STATE_MACHINE_HPP

#include "flightStates.hpp"
#include "IMUSensor.hpp"
#include "pressureSensor.hpp"
#include "pyroController.hpp"
#include "pinAssn.hpp"
#include "configKeys.hpp"
#include "barometricProcessor.hpp"
#include "sensorFusion.hpp"
#include "buzzerFunctions.hpp"
#include "dataLogger.hpp"
#include "IMUProcessor.hpp"

/**
 * @class FlightStateMachine
 * @brief Manages the state transitions and sensor data processing 
 * for the flight.
 *
 * This class handles the state transitions during the flight, 
 * updates and processes sensor data, and controls actuators 
 * such as the pyro controllers and buzzers.
 */
class FlightStateMachine {
public:
    /**
     * @brief Constructor for FlightStateMachine.
     * 
     * @param buzzerFunc_ Reference to the BuzzerFunctions object.
     * @param logger_ Reference to the DataLogger object.
     */
    FlightStateMachine(BuzzerFunctions& buzzerFunc_, DataLogger& logger_);

    /**
     * @brief Update the flight state machine.
     * 
     * This method updates the sensor data and handles the state transitions.
     */
    void update();

    /**
     * @brief Get the current flight state.
     * 
     * @return The current flight state.
     */
    FlightState getCurrentState() const;

    /**
     * @brief Transition to a new flight state.
     * 
     * @param newState The new flight state to transition to.
     */
    void transitionToState(FlightState newState);

    /**
     * @brief Log sensor data with an optional delay.
     * 
     * @param delayTime The delay time in milliseconds before logging the data.
     */
    void logSensorData(uint16_t delayTime = 0);

private:
    FlightState currentState_; ///< The current flight state
    PressureSensor pressureSensor_; ///< The pressure sensor object
    IMUSensor imu_; ///< The IMU sensor object
    std::shared_ptr<BarometricProcessor> altitudeProcessor_; ///< The barometric processor
    std::shared_ptr<IMUProcessor> imuProcessor_; ///< The IMU processor
    PyroController pyroDrogue_; ///< The pyro controller for drogue deployment
    PyroController pyroMain_; ///< The pyro controller for main deployment
    BuzzerFunctions& buzzerFunc_; ///< Reference to the BuzzerFunctions object
    DataLogger& logger_; ///< Reference to the DataLogger object
    SensorFusion sensors_; ///< The sensor fusion object
    Timer loggingTimer_; ///< Timer for managing logging intervals
    float currentAltitude_; ///< Current altitude
    float currentVelocity_; ///< Current velocity
    float maxAltitude_; ///< Maximum recorded altitude
    float maxVelocity_; ///< Maximum recorded velocity
    float groundAltitude_; ///< Ground altitude
    const float APOGEE_VELOCITY_THRESHOLD = 0.5; ///< Velocity threshold for apogee detection (m/s)
    const float LANDING_VEL_THRESHOLD = 1; ///< Velocity threshold for landing detection (m/s)

    /**
     * @brief Initialize sensors and add them to sensor fusion.
     */
    void initializeSensors();

    /**
     * @brief Update sensor data by reading from the sensors.
     */
    void updateSensorData();

    /**
     * @brief Handle the state transitions based on sensor data
     *  and current state.
     */
    void handleStateTransition();

    /**
     * @brief Handle pre-launch state logic.
     */
    void handlePreLaunch();

    /**
     * @brief Handle ascent state logic.
     */
    void handleAscent();

    /**
     * @brief Handle apogee state logic.
     */
    void handleApogee();

    /**
     * @brief Handle descent under drogue state logic.
     */
    void handleDescentDrogue();

    /**
     * @brief Handle low altitude detection state logic.
     */
    void handleLowAltitudeDetection();

    /**
     * @brief Handle descent under main state logic.
     */
    void handleDescentMain();

    /**
     * @brief Handle landing state logic.
     */
    void handleLanding();

    /**
     * @brief Handle stage separation state logic.
     */
    void handleStageSeparation();

    /**
     * @brief Handle failure state logic.
     */
    void handleFailure();
};

#endif // FLIGHT_STATE_MACHINE_HPP
