#include "ToiletPaperRoll.hpp"

ToiletPaperRoll::Action::Action(
    const std::function<bool(const Action&)>& stpCondition,
    Direction dir)
    : stopCondition(stpCondition)
    , direction(dir)
{
}

void ToiletPaperRoll::attach(int pin)
{
    _servo.attach(pin);
}

ToiletPaperRoll::ToiletPaperRoll(SensorValue& ir1, SensorValue& ir2)
    : _ir1(ir1)
    , _ir2(ir2)
{
}

void ToiletPaperRoll::setDirection(Direction dir)
{
    if (_currentDirection == dir)
        return;
    switch (dir) {
    case Direction::FORWARDS:
        _servo.writeMicroseconds(1650);
        break;
    case Direction::STOP:
        _servo.writeMicroseconds(1500);
        break;
    case Direction::BACKWARDS:
        _servo.writeMicroseconds(1300);
        break;
    }
    _currentDirection = dir;
}

void ToiletPaperRoll::getSheetTime(const std::function<void(unsigned long)>& setter)
{
    // // positioning the toilet paper in the right spot
    setPaperInPosition();

    // measuring
    unsigned long time = millis();
    _actions.emplace_back([this, time, setter](const Action&) {
        if (_ir1.getValue() == true) {
            setter(millis() - time);
            return true;
        }
        return false;
    },
        ToiletPaperRoll::FORWARDS);

}

void ToiletPaperRoll::setPaperInPosition(const std::function<void()>& after)
{
    _actions.emplace_back([this](const Action&) {
        return _ir2.getValue() == true;
    },
        ToiletPaperRoll::FORWARDS);
    _actions.emplace_back([this, after](const Action&) {
        if (_ir2.getValue() == false) {
            after();
            return true;
        }
        return false;
    },
        ToiletPaperRoll::BACKWARDS);
}

void ToiletPaperRoll::calibrate(int tries)
{
    _calibrationTime = 0;
    for (int i = 0; i < tries; i++) {
        getSheetTime([this](unsigned long time) {
            _calibrationTime += time;
        });
    }
    setPaperInPosition([this, tries]() {
        _fullOneRollTime = _calibrationTime / tries;
        if (_oneRollTime == 0)
            _oneRollTime = _fullOneRollTime;
    });
}

void ToiletPaperRoll::updateRollTime(int tries)
{
    _calibrationTime = 0;
    for (int i = 0; i < tries; i++) {
        getSheetTime([this](unsigned long time) {
            _calibrationTime += time;
        });
    }
    setPaperInPosition([this, tries]() {
        _oneRollTime = _calibrationTime / tries;
    });
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

float ToiletPaperRoll::getEmptyPerimeter() const
{
    return _emptyDiameter * M_PI;
}

float ToiletPaperRoll::percentageLeft(bool adjusted) const
{
    if (_fullOneRollTime == 0 || _oneRollTime == 0)
        return -1;
    float emptyPerim = getEmptyPerimeter();
    float perim = (getCurrentPerimeter() - emptyPerim) / (getFullPerimeter() - emptyPerim) * 100.0;
    if (adjusted)
        return min(100.0, max(perim * 100.0, 0.0));
    return perim; // adjusted == false for debug purposes only
}

void ToiletPaperRoll::update()
{
    if (_timeBeforeAction > 0) {
        _timeBeforeAction -= TOILET_PAPER_MS_TICK;
        return;
    }
    if (_actions.size()) {
        setDirection(_actions.front().direction);
        _actions.front().timePassed += TOILET_PAPER_MS_TICK;
        if (_actions.front().stopCondition(_actions.front())) {
            _actions.pop_front();
            _timeBeforeAction = TOILET_PAPER_AFTER_ACTION_WAIT;
            setDirection(STOP);
        }
    }
}
