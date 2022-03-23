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
    void updateRollTime(int tries = 5);

    void forwards();
    void stop();
    void backwards();

    float percentageLeft(bool adjusted = true) const;

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
    float getEmptyPerimeter() const;


private:
    Servo _servo;
    SensorValue& _ir1;
    SensorValue& _ir2;

    unsigned long _oneRollTime = 0;
    unsigned long _fullOneRollTime = 2880;
    Direction _currentDirection = STOP;

    float _fullDiameter = 109; // default height is 109mm
    float _emptyDiameter = 45; // default height is 50mm
    // float _sheetLength = 124; // default sheet length is 124;
    const float _sensorDistance = 102; // distance between the sensors is 100;
};

#endif // TOILET_PAPER_ROLL_HPP
