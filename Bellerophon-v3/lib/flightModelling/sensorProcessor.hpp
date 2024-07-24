// sensorProcessor.hpp
#ifndef SENSOR_PROCESSOR_HPP
#define SENSOR_PROCESSOR_HPP

class SensorProcessor {
public:
    virtual ~SensorProcessor() = default;

    virtual void update() = 0;
    virtual float getAltitude() const = 0;
    virtual float getVerticalVelocity() const = 0;
    virtual float getAcceleration() const = 0;
};

#endif // SENSOR_PROCESSOR_HPP
