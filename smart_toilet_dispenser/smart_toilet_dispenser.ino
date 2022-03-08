#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#define IR1_PIN 36
#define LED_BUILTIN 2
#define PIR_PIN 39
#define SERVO_PIN 12

Servo servo;

void setup() {
  // put your setup code here, to run once:
  // pinMode(IR1_PIN, INPUT);
  // pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(PIR_PIN, INPUT);
  servo.attach(SERVO_PIN);
  Serial.begin(9600);
}

void loop() {
  // int ir1_act = digitalRead(IR1_PIN);
  // int pir_act = digitalRead(PIR_PIN);
  // Serial.print(ir1_act);
  // Serial.print(" ");
  // Serial.println(pir_act);
  // digitalWrite(LED_BUILTIN, ir1_act ? LOW : HIGH);
  // digitalWrite(SERVO_PIN, HIGH);
  // delay(1700);
  // digitalWrite(SERVO_PIN, LOW);
  // delay(500);
//  servo.writeMicroseconds(2000); // FULL FORWARD
  delay(2000);
  servo.writeMicroseconds(1500); // STOP
  delay(2000);
}
