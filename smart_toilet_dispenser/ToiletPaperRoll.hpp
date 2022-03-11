#if !defined(TOILET_PAPER_ROLL_HPP)
#define TOILET_PAPER_ROLL_HPP

#include <Arduino.h>
#include <ESP32PWM.h>
#include <ESP32Servo.h>
#include <ESP32Tone.h>
#include <analogWrite.h>

class ToiletPaperRoll {
   public:
    void attach(int pin) { _servo.attach(pin); }

   private:
    Servo _servo;
};

#endif  // TOILET_PAPER_ROLL_HPP
