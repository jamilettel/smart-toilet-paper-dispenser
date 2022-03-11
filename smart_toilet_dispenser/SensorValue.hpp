#ifndef SENSOR_VALUE_HPP
#define SENSOR_VALUE_HPP

#include "CircularBuffer.hpp"

class SensorValue {
private:
    CircularBuffer<int> _buffer;

public:
    SensorValue(size_t bufferSize)
        : _buffer(bufferSize)
    {
    }
    ~SensorValue() = default;
    SensorValue(const SensorValue&) = delete;
    SensorValue& operator=(const SensorValue&) = delete;

    // return mean value of all the elements in the buffer
    int getValue() const;

    // add a value to the buffer
    void setValue(int value);
};

#endif // SENSOR_VALUE_HPP
