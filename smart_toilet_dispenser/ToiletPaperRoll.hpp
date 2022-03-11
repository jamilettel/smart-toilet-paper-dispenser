#if !defined(TOILET_PAPER_ROLL_HPP)
#define TOILET_PAPER_ROLL_HPP

#include <Arduino.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <analogWrite.h>

#include "SensorValue.hpp"

class ToiletPaperRoll {
public:
    ToiletPaperRoll(SensorValue& ir1, SensorValue& ir2);
    void attach(int pin);
    void calibrate(int tries = 5);

    void forwards();
    void stop();
    void backwards();

private:
    void waitDelay();
    void changeDirectionDelay();

    unsigned long getSheetTime();

    enum Direction {
        FORWARDS,
        STOP,
        BACKWARDS,
    };

private:
    Servo _servo;
    SensorValue& _ir1;
    SensorValue& _ir2;

    unsigned long _oneRollTime = 0;
    unsigned long _fullOneRollTime = 0;
    Direction _currentDirection = STOP;
};

#endif // TOILET_PAPER_ROLL_HPP
