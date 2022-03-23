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
    _servo.writeMicroseconds(1650);
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
    _servo.writeMicroseconds(1300);
}

void ToiletPaperRoll::waitDelay()
{
    delay(5);
}

void ToiletPaperRoll::changeDirectionDelay()
{
    delay(200);
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
        waitDelay();
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

    return time;
}

void ToiletPaperRoll::calibrate(int tries)
{
    unsigned long time = 0;
    for (int i = 0; i < tries; i++) {
        time += getSheetTime();
    }
    _fullOneRollTime = time / tries;
}

void ToiletPaperRoll::updateRollTime(int tries)
{
    unsigned long time = 0;
    for (int i = 0; i < tries; i++) {
        time += getSheetTime();
    }
    _oneRollTime = time / tries;
}

float ToiletPaperRoll::getFullPerimeter() const
{
    return _fullDiameter * M_PI;
}

/** Compute current permiter of the toilet paper roll.
 * Uses values from user (like sheet length and the full roll's diameter),
 * and values from the calibration (like the time it takes to roll out 1 sheet of paper).
 * _oneRollTime must be up to date.
 */
float ToiletPaperRoll::getCurrentPerimeter() const
{
    float full1SheetAngle = _sensorDistance * 360.0 / getFullPerimeter();
    float angleNow = _oneRollTime * full1SheetAngle / _fullOneRollTime;
    return 360.0 * _sensorDistance / angleNow;
}

float ToiletPaperRoll::getEmptyPerimeter() const {
    return _emptyDiameter * M_PI;
}

float ToiletPaperRoll::percentageLeft(bool adjusted) const
{
    float emptyPerim = getEmptyPerimeter();
    float perim = (getCurrentPerimeter() - emptyPerim) / (getFullPerimeter() - emptyPerim) * 100.0;
    if (adjusted)
        return min(100.0, max(perim * 100.0, 0.0));
    return perim;
}
