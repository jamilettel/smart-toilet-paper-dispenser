#include "ToiletPaperRoll.hpp"

ToiletPaperRoll::Action::Action(
    const std::function<bool(const Action&)>& stpCondition,
    Direction dir,
    int timeoutMs)
    : stopCondition(stpCondition)
    , direction(dir)
    , timeout(timeoutMs)
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
    _resetTime = true;

    // measuring
    _actions.emplace_back([this, setter](const Action&) {
        if (_resetTime == true) {
            _resetTime = false;
            _sheetTime = millis();
        }
        if (_ir1.getValue() == true) {
            setter(millis() - _sheetTime);
            _resetTime = true;
            return true;
        }
        return false;
    },
        ToiletPaperRoll::FORWARDS, 10000);
}

void ToiletPaperRoll::setPaperInPosition(const std::function<void()>& after, int timeout)
{
    _actions.emplace_back([this](const Action&) {
        return _ir2.getValue() == true;
    },
        ToiletPaperRoll::FORWARDS, 10000);
    _actions.emplace_back([this, after](const Action&) {
        if (_ir2.getValue() == false) {
            after();
            return true;
        }
        return false;
    },
        ToiletPaperRoll::BACKWARDS, timeout);
}

void ToiletPaperRoll::calibrate(int tries)
{
    if (_state == State::MEASURING)
        return;
    setState(MEASURING);
    _calibrationTime = 0;
    for (int i = 0; i < tries; i++) {
        getSheetTime([this](unsigned long time) {
            _calibrationTime += time;
        });
    }
    setPaperInPosition([this, tries]() {
        _fullOneRollTime = _calibrationTime / tries;
        _oneRollTime = _fullOneRollTime;
        setState(WORKING);
    });
}

void ToiletPaperRoll::updateRollTime(int tries)
{
    if (_state == State::MEASURING)
        return;
    setState(MEASURING);
    _calibrationTime = 0;
    for (int i = 0; i < tries; i++) {
        getSheetTime([this](unsigned long time) {
            _calibrationTime += time;
        });
    }
    setPaperInPosition([this, tries]() {
        _oneRollTime = _calibrationTime / tries;
        setState(WORKING);
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
    double emptyPerim = getEmptyPerimeter();
    double perim = (getCurrentPerimeter() - emptyPerim) / (getFullPerimeter() - emptyPerim) * 100.0;
    if (adjusted)
        return min(100.0, max(perim, 0.0));
    return perim; // adjusted == false for debug purposes only
}

void ToiletPaperRoll::update()
{
    if (_timeBeforeAction > 0) {
        _timeBeforeAction -= TOILET_PAPER_MS_TICK;
        return;
    }
    if (_measureIn > 0) {
        _measureIn -= TOILET_PAPER_MS_TICK;
        if (_measureIn <= 0 && _actions.size() == 0 && getState() == WORKING)
            updateRollTime();
        else if (_measureIn <= 0)
            _measureIn = TOILET_PAPER_MEASURE_AFTER;
    }
    if (_actions.size()) {
        setDirection(_actions.front().direction);
        _actions.front().timePassed += TOILET_PAPER_MS_TICK;
        if (_actions.front().stopCondition(_actions.front())) {
            _actions.pop_front();
            _timeBeforeAction = TOILET_PAPER_AFTER_ACTION_WAIT;
            setDirection(STOP);
        } else if (_actions.front().timeout != -1 && _actions.front().timePassed > _actions.front().timeout) {
            _actions.clear();
            setDirection(STOP);
            setState(State::EMPTY);
        }
    } else if (getState() == WORKING && _ir1.getValue() == false) {
        _actions.emplace_back([this](const Action&) {
            if (_ir1.getValue() == true) {
                _measureIn = TOILET_PAPER_MEASURE_AFTER;
                return true;
            }
            return false;
        },
            FORWARDS, 20000);
    }
}

void ToiletPaperRoll::setState(State state)
{
    if (_state != state) {
        _state = state;
        onStateChange(state);
        if (state == STOPPED) {
            _actions.clear();
            setPaperInPosition([]() {}, 20000);
        }
    }
}

String ToiletPaperRoll::stateToString(State state)
{
    switch (state) {
    case State::EMPTY:
        return "error";
    case State::WORKING:
        return "working";
    case State::MEASURING:
        return "calibrating";
    case State::STOPPED:
        return "stopped";
    default:
        return "unknown";
    }
}

ToiletPaperRoll::State ToiletPaperRoll::getState() const
{
    return _state;
}

unsigned long ToiletPaperRoll::getOneSheetTime() const
{
    return _oneRollTime;
}

unsigned long ToiletPaperRoll::getFullOneSheetTime() const
{
    return _fullOneRollTime;
}

void ToiletPaperRoll::setOneSheetTime(unsigned long time)
{
    _oneRollTime = time;
    if (_oneRollTime && _fullOneRollTime) {
        setState(WORKING);
    }
}

void ToiletPaperRoll::setFullOneSheetTime(unsigned long time)
{
    _fullOneRollTime = time;
    if (_oneRollTime && _fullOneRollTime) {
        setState(WORKING);
    }
}

void ToiletPaperRoll::clearActions()
{
    _actions.clear();
}