#ifndef BUZZERCONTROLLER_HPP
#define BUZZERCONTROLLER_HPP

#include <Arduino.h>
#include "PinController.hpp"
#include "Timer.hpp"

/**
 * @class BuzzerController
 * @brief A class to control a buzzer with non-blocking functionality using the tone library.
 *
 * This class provides methods to turn a buzzer on, off, and beep it with a specified duration
 * using non-blocking delays.
 */
class BuzzerController {
public:
    /**
     * @brief Constructor to initialize the BuzzerController with a specific pin.
     * @param pin The pin connected to the buzzer.
     */
    BuzzerController(uint8_t pin);

    /**
     * @brief Destructor to ensure the buzzer is turned off.
     */
    ~BuzzerController();

    /**
     * @brief Starts the buzzer beeping with a specified duration and frequency.
     * @param beepDuration The duration (in milliseconds) for which the buzzer should beep.
     * @param frequency The frequency (in Hertz) of the beep sound.
     */
    void beep(uint32_t beepDuration, uint16_t frequency);

    /**
     * @brief Checks if the buzzer is currently on.
     * @return True if the buzzer is on, false otherwise.
     */
    bool isOn() const;

    /**
     * @brief Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
     */
    void update();



private:
    uint8_t _pin;              // Pin connected to the buzzer
    Timer _beepTimer;          // Timer for the beep duration
    bool _isPowered;           // Indicates if the buzzer is currently powered on

    /**
     * @brief Initializes the BuzzerController.
     */
    void initialize();

    /**
     * @brief Turns the buzzer off.
     */
    void turnOff();

    void turnOn(uint16_t frequency);
};

#endif // BUZZERCONTROLLER_HPP
