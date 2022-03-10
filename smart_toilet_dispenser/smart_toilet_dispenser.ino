#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>
#include <Ticker.h>

#include "CircularBuffer.hpp"
#include "SensorValue.hpp"

#define IR1_PIN 36
#define IR2_PIN 34
#define LED_BUILTIN 2
#define PIR_PIN 39
#define SERVO_PIN 12

SensorValue ir1(20);
SensorValue ir2(20);
SensorValue pir(5);

Servo servo;
Ticker irUpdateTicker;
Ticker pirUpdateTicker;
Ticker printStatesTicker;

void setup()
{
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IR1_PIN, INPUT);
  pinMode(IR2_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  servo.attach(SERVO_PIN);

  irUpdateTicker.attach_ms(50, updateIRSensors);
  pirUpdateTicker.attach_ms(500, updatePIRSensors);
  printStatesTicker.attach_ms(1000, printSensorsStates);
}

void printSensorsStates()
{
  Serial.println("IR1: " + String(ir1.getValue()) + " IR2:" + String(ir2.getValue()) + " PIR:" + String(pir.getValue()));
}

void updateIRSensors()
{
  ir1.setValue(!digitalRead(IR1_PIN));
  ir2.setValue(!digitalRead(IR2_PIN));
}

void updatePIRSensors() {
  pir.setValue(digitalRead(PIR_PIN));
  digitalWrite(LED_BUILTIN, pir.getValue() ? HIGH : LOW);
}

void loop()
{
}
