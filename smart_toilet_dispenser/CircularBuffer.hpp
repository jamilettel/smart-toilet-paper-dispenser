#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include <exception>
#include <string.h>

template <class T>
class CircularBuffer {
private:
    size_t _size;
    T* _values;
    size_t _index = 0;
    size_t _length = 0;

public:
    CircularBuffer(size_t size)
        : _size(size)
    {
        _values = new T[_size];
        memset((void*)_values, 0, sizeof(T) * _size);
    };
    ~CircularBuffer() { delete[] _values; };
    CircularBuffer(const CircularBuffer&) = delete;
    CircularBuffer& operator=(const CircularBuffer&) = delete;

    T& operator[](size_t ind)
    {
        if (ind >= _length)
            throw std::exception();
        return _values[(ind + _index) % _size];
    }

    const T& operator[](size_t ind) const
    {
        if (ind >= _length)
            throw std::exception(); // out of bounds
        return _values[(ind + _index) % _size];
    }

    void pushValue(const T& value)
    {
        _values[(_index + _length) % _size] = value;
        if (_length == _size) {
            _index++;
        } else {
            _length++;
        }
    }

    const T*& getValues() { return _values; }

    size_t length() const { return _length; }
};

#endif // CIRCULAR_BUFFER_HPP