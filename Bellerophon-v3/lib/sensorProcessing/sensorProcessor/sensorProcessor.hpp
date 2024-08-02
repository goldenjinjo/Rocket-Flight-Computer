    // sensorProcessor.hpp
    #ifndef SENSOR_PROCESSOR_HPP
    #define SENSOR_PROCESSOR_HPP

    #include <cstddef>

    class SensorProcessor {
    public:
        virtual ~SensorProcessor() = default;

        virtual void update() = 0;
        virtual float getAltitude() const = 0;
        virtual float getVerticalVelocity() const = 0;
        virtual float getAcceleration() const = 0;
        virtual float getGroundAltitude() const = 0;
        virtual float getMaxAltitude() const = 0;
        virtual float getMaxVelocity() const = 0;
        virtual float getMaxAcceleration() const = 0;
        virtual float* getRawData() const = 0;
        virtual size_t getNumSensorValues() const = 0;
    };

    #endif // SENSOR_PROCESSOR_HPP
