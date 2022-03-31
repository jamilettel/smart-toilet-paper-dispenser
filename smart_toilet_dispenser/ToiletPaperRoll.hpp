#if !defined(TOILET_PAPER_ROLL_HPP)
#define TOILET_PAPER_ROLL_HPP

#include <Arduino.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <analogWrite.h>
#include <math.h>
#define __cplusplus 201103L // TODO remove
#include <functional>
#include <list>

#include "SensorValue.hpp"

#define TOILET_PAPER_MS_TICK 5
#define TOILET_PAPER_AFTER_ACTION_WAIT 200

class ToiletPaperRoll {
private:
    enum Direction {
        FORWARDS,
        STOP,
        BACKWARDS,
    };

    struct Action {
        Action(
            const std::function<bool(const Action&)>& stopCondition,
            Direction direction, int timeout = -1);
        ~Action() = default;
        Action(const Action&) = delete;
        Action& operator=(const Action&) = delete;

        const Direction direction;
        float timePassed = 0;
        int timeout; // in milliseconds
        const std::function<const bool(Action&)> stopCondition;
    };

public:
    enum State {
        MEASURING, // calibrating or measuring
        EMPTY, // Toilet paper roll is empty, or error
        EXPANDED, // When user is in the room
        IDLE, // Retracted, waiting for user to get in the room
        STOPPED, // Ordered to stop by user, will go back to normal when user leaves
    };

    static String stateToString(State state);

    ToiletPaperRoll(SensorValue& ir1, SensorValue& ir2);
    void attach(int pin);
    void calibrate(int tries = 5);
    void updateRollTime(int tries = 5);

    void setPaperInPosition(const std::function<void()>& after = []() {});

    float percentageLeft(bool adjusted = true) const;

    void update();

    std::function<void(State state)> onStateChange = [](State _state) {};
    State getState() const;

private:
    void setDirection(Direction dir);

    void getSheetTime(const std::function<void(unsigned long)>& setter);

    float getFullPerimeter() const;
    float getCurrentPerimeter() const;
    float getEmptyPerimeter() const;

    void setState(State state);

private:
    Servo _servo;
    SensorValue& _ir1;
    SensorValue& _ir2;

    unsigned long _oneRollTime = 0;
    unsigned long _fullOneRollTime = 0;
    Direction _currentDirection = STOP;

    float _fullDiameter = 109; // default height is 109mm
    float _emptyDiameter = 45; // default height is 50mm
    float _sheetLength = 124; // default sheet length is 124;

    const float _sensorDistance = 102; // distance between the sensors is 102: DO NOT CHANGE
    std::list<Action> _actions;
    unsigned long _timeBeforeAction = 0;

    unsigned long _calibrationTime = 0;
    State _state = STOPPED;
};

#endif // TOILET_PAPER_ROLL_HPP
