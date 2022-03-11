#include "ToiletPaperRoll.hpp"

void ToiletPaperRoll::attach(int pin)
{
    _servo.attach(pin);
}

ToiletPaperRoll::ToiletPaperRoll(SensorValue& ir1, SensorValue& ir2)
    : _ir1(ir1)
    , _ir2(ir2)
{
}

void ToiletPaperRoll::forwards()
{
    if (_currentDirection == FORWARDS)
        return;
    _currentDirection = FORWARDS;
    _servo.writeMicroseconds(1600);
}

void ToiletPaperRoll::stop()
{
    if (_currentDirection == STOP)
        return;
    _currentDirection = STOP;
    _servo.writeMicroseconds(1500);
}

void ToiletPaperRoll::backwards()
{
    if (_currentDirection == BACKWARDS)
        return;
    _currentDirection = BACKWARDS;
    _servo.writeMicroseconds(1380);
}

void ToiletPaperRoll::waitDelay()
{
    delay(20);
}

void ToiletPaperRoll::changeDirectionDelay()
{
    delay(500);
}

unsigned long ToiletPaperRoll::getSheetTime()
{
    // positioning the toilet paper in the right spot
    while (!_ir2.getValue()) {
        forwards();
        waitDelay();
    }
    stop();
    changeDirectionDelay();
    while (_ir2.getValue()) {
        backwards();
        delay(25);
    }
    stop();
    changeDirectionDelay();

    // measuring
    unsigned long time = millis();
    while (!_ir1.getValue()) {
        forwards();
        waitDelay();
    }
    stop();
    changeDirectionDelay();
    time = millis() - time;

    // retracting
    while (_ir2.getValue()) {
        backwards();
        waitDelay();
    }
    stop();
    changeDirectionDelay();

    Serial.println("One_sheet:" + String(time));
    return time;
}

void ToiletPaperRoll::calibrate(int tries)
{
    unsigned long time = 0;
    for (int i = 0; i < tries; i++) {
        time += getSheetTime();
    }
    _fullOneRollTime = time / tries;
    Serial.println("One_full_sheet_time:" + String(_fullOneRollTime));
}