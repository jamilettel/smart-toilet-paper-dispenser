#include <iostream>
#include "CircularBuffer.hpp"
int main() {
    CircularBuffer<int> buffer(100);
    for (int i = 0; i < 50; i++)
        buffer.pushValue(i);

    for (int i = 0; i < buffer.length(); i++)
        std::cout << buffer[i] << ' ';
    std::cout << std::endl;

    for (int i = 0; i < 50; i++)
        buffer.pushValue(i);

    for (int i = 0; i < buffer.length(); i++)
        std::cout << buffer[i] << ' ';
    std::cout << std::endl;

    buffer.pushValue(1);

    for (int i = 0; i < buffer.length(); i++)
        std::cout << buffer[i] << ' ';
    std::cout << std::endl;
}
