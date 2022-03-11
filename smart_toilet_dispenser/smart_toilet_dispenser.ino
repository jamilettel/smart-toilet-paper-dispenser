#include <Ticker.h>

#include "CircularBuffer.hpp"
#include "SensorValue.hpp"
#include "ToiletPaperRoll.hpp"

#define IR1_PIN 36
#define IR2_PIN 34
#define LED_BUILTIN 2
#define PIR_PIN 39
#define SERVO_PIN 12

SensorValue ir1(10);
SensorValue ir2(10);
SensorValue pir(5);

Ticker irUpdateTicker;
Ticker pirUpdateTicker;
Ticker printStatesTicker;

ToiletPaperRoll tpr;

void setup() {
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(IR1_PIN, INPUT);
    pinMode(IR2_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    tpr.attach(SERVO_PIN);

    irUpdateTicker.attach_ms(10, updateIRSensors);
    pirUpdateTicker.attach_ms(500, updatePIRSensors);
    printStatesTicker.attach_ms(1000, printSensorsStates);
}

void printSensorsStates() {
    Serial.println("IR1: " + String(ir1.getValue()) + " IR2:" +
                   String(ir2.getValue()) + " PIR:" + String(pir.getValue()));
}

void updateIRSensors() {
    ir1.setValue(!digitalRead(IR1_PIN));
    ir2.setValue(!digitalRead(IR2_PIN));
}

void updatePIRSensors() {
    pir.setValue(digitalRead(PIR_PIN));
    digitalWrite(LED_BUILTIN, pir.getValue() ? HIGH : LOW);
}

void loop() {
    static int servoState = 0;

    if (servoState == 0 && ir1.getValue() == 1) {
        //   servoState = 1;
        //   servo.writeMicroseconds(1500); // stop a little bit
        //   delay(500);
    } else if (servoState == 1 && ir2.getValue() == 0) {
        //   servoState = 2;
        //   servo.writeMicroseconds(1500);
        //   delay(500);
    }

    if (servoState == 0) {
        //   servo.writeMicroseconds(1600);
    } else if (servoState == 1) {
        //   servo.writeMicroseconds(1380);
    } else {
        //   servo.writeMicroseconds(1500);
    }

    delay(20);
}
