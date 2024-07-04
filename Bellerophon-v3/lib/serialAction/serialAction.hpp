#ifndef SERIAL_ACTION_HPP
#define SERIAL_ACTION_HPP

#include "serialCommunicator.hpp"
#include "constants.hpp"
#include <cstring>

/**
 * @class SerialAction
 * @brief Class to perform Serial Actions for communication across the serial platform.
 */
class SerialAction {
public:
    /**
     * @brief Constructor for SerialAction.
     * @param communicator A reference to an instance of SerialCommunicator to be used for serial actions.
     */
    SerialAction(SerialCommunicator& communicator);

    /**
     * @brief Method to check and switch between serial modes based on the received message.
     */
    void checkSerialForMode();

private:
    SerialCommunicator& communicator; ///< Reference to the SerialCommunicator instance
};

#endif // SERIAL_ACTION_HPP
