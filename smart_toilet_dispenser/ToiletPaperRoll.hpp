#if !defined(TOILET_PAPER_ROLL_HPP)
#define TOILET_PAPER_ROLL_HPP

#include <Arduino.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <analogWrite.h>
#include <math.h>

#include "SensorValue.hpp"

class ToiletPaperRoll {
public:
    ToiletPaperRoll(SensorValue& ir1, SensorValue& ir2);
    void attach(int pin);
    void calibrate(int tries = 5);
    void getRollTime(int tries = 5);

    void forwards();
    void stop();
    void backwards();

    float percentageLeft() const;

private:
    void waitDelay();
    void changeDirectionDelay();

    unsigned long getSheetTime();

    enum Direction {
        FORWARDS,
        STOP,
        BACKWARDS,
    };

    float getFullPerimeter() const;
    float getCurrentPerimeter() const;

private:
    Servo _servo;
    SensorValue& _ir1;
    SensorValue& _ir2;

    unsigned long _oneRollTime = 0;
    unsigned long _fullOneRollTime = 2880;
    Direction _currentDirection = STOP;

    float _fullDiameter = 109; // default height is 109mm
    float _emptyDiameter = 50; // default height is 50mm
    float _sheetLength = 124; // default sheet length is 124;
};

#endif // TOILET_PAPER_ROLL_HPP
