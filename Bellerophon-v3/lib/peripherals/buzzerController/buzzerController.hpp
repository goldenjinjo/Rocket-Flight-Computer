#ifndef BUZZERCONTROLLER_HPP
#define BUZZERCONTROLLER_HPP

#include <Arduino.h>
#include <queue>
#include "PinController.hpp"
#include "Timer.hpp"

/**
 * @class BuzzerController
 * @brief A class to control a buzzer with non-blocking functionality using the tone library.
 *
 * This class provides methods to turn a buzzer on, off, and beep it with a specified duration
 * using non-blocking delays. It also supports multiple buzzes of different frequencies in succession.
 */
class BuzzerController {
public:
    /**
     * @brief Constructor to initialize the BuzzerController with a specific pin.
     * @param pin The pin connected to the buzzer.
     * @param maxQueueSize The maximum size of the beep command queue.
     */
    BuzzerController(uint8_t pin, size_t maxQueueSize = 10);

    /**
     * @brief Destructor to ensure the buzzer is turned off.
     */
    ~BuzzerController();

    /**
     * @brief Starts the buzzer beeping with a specified duration and frequency.
     * @param beepDuration The duration (in milliseconds) for which the buzzer should beep.
     * @param frequency The frequency (in Hertz) of the beep sound.
     * @return True if the command was added to the queue, false if the queue is full.
     */
    bool beep(uint32_t beepDuration, uint16_t frequency);

    /**
     * @brief Keeps the buzzer silent for a specified duration.
     * @param duration The duration (in milliseconds) for which the buzzer should be silent.
     * @return True if the command was added to the queue, false if the queue is full.
     */
    bool silent(uint32_t duration);

    /**
     * @brief Checks if the buzzer is currently on.
     * @return True if the buzzer is on, false otherwise.
     */
    bool isOn() const;

    /**
     * @brief Updates the buzzer state. This should be called in the main loop to ensure non-blocking behavior.
     */
    void update();

    /**
     * @brief Returns the current size of the beep queue.
     * @return The current size of the beep queue.
     */
    size_t getQueueSize() const;

    /**
     * @brief Returns the max size of the beep queue.
     * @return The max size of the beep queue.
     */
    size_t getMaxQueueSize() const;

    /**
     * @brief Checks if the queue is full.
     * @return True if the queue is full, false otherwise.
     */
    bool isQueueFull() const;

    /**
     * @brief Sets the reset flag to start clearing the queue incrementally.
     */
    void reset();

private:
    uint8_t _pin;              // Pin connected to the buzzer
    Timer _beepTimer;          // Timer for the beep duration
    bool _isPowered;           // Indicates if the buzzer is currently powered on
    bool _resetting;           // Indicates if the buzzer is in the process of resetting
    size_t _maxQueueSize;      // Maximum size of the beep command queue

    struct BeepCommand {
        uint32_t duration;
        uint16_t frequency;
    };

    std::queue<BeepCommand> _beepQueue; // Queue of beep commands

    /**
     * @brief Initializes the BuzzerController.
     */
    void initialize();

    /**
     * @brief Turns the buzzer on with a specific frequency.
     * @param frequency The frequency (in Hertz) of the beep sound.
     */
    void turnOn(uint16_t frequency);

    /**
     * @brief Turns the buzzer off.
     */
    void turnOff();

    /**
     * @brief Adds a command to the beep queue.
     * @param beepDuration The duration (in milliseconds) for which the buzzer should beep or be silent.
     * @param frequency The frequency (in Hertz) of the beep sound, 0 for silence.
     * @return True if the command was added to the queue, false if the queue is full.
     */
    bool pushCommand(uint32_t beepDuration, uint16_t frequency);

    /**
     * @brief Clears one element from the beep queue.
     */
    void clearQueue();

    /**
     * @brief Handles the buzzer state when it is powered.
     */
    void handlePoweredState();

    /**
     * @brief Processes the next command in the beep queue.
     */
    void processNextCommand();
};

#endif // BUZZERCONTROLLER_HPP


