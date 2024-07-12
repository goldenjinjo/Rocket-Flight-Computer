// FlightStates.hpp
#ifndef FLIGHTSTATES_HPP
#define FLIGHTSTATES_HPP

enum class FlightState {
    PRE_LAUNCH,
    ASCENT,
    APOGEE,
    DESCENT_DROGUE,
    LOW_ALTITUDE_DETECTION,
    DESCENT_MAIN,
    LANDING,
    STAGE_SEPARATION, // Placeholder for future expansion
    FAILURE // Handle off-nominal conditions
};

#endif // FLIGHTSTATES_HPP
