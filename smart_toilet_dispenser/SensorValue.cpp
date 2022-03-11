#include "SensorValue.hpp"

int SensorValue::getValue() const
{
    if (_buffer.length() == 0)
        return 0;
    float total = 0;
    for (int i = 0; i < _buffer.length(); i++) {
        total += (float)_buffer[i];
    }
    total /= _buffer.length();
    // trunc total by casting into int, then turn back to float
    if (total - (float((int)total) >= 0.5))
        return total + 1;
    return total;
}

void SensorValue::setValue(int value)
{
    _buffer.pushValue(value);
}