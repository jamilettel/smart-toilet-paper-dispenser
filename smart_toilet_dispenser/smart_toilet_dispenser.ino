#include <Ticker.h>

#include "CircularBuffer.hpp"
#include "SensorValue.hpp"
#include "TPClient.hpp"
#include "ToiletPaperRoll.hpp"

#define IR_BOTTOM_PIN 36
#define IR_TOP_PIN 34
#define LED_BUILTIN 2
#define PIR_PIN 39
#define SERVO_PIN 12

#define WIFI_SSID "Jamil"
#define WIFI_PASSWORD "Jamil123"
#define SERVER_URL "ws://167.71.9.47:3000"

SensorValue ir1(10);
SensorValue ir2(10);
SensorValue pir(5);

Ticker irUpdateTicker;
Ticker pirUpdateTicker;
// Ticker printSensorsStates;
Ticker tprTicker;
Ticker clientTicker;

ToiletPaperRoll tpr(ir1, ir2);
TPClient tpclient(WIFI_SSID, WIFI_PASSWORD, SERVER_URL, tpr);

void setup()
{
    Serial.begin(9600);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(IR_BOTTOM_PIN, INPUT);
    pinMode(IR_TOP_PIN, INPUT);
    pinMode(PIR_PIN, INPUT);
    tpr.attach(SERVO_PIN);

    irUpdateTicker.attach_ms(10, updateIRSensors);
    pirUpdateTicker.attach_ms(500, updatePIRSensors);
    // printStatesTicker.attach_ms(1000, printSensorsStates);
    tprTicker.attach_ms(TOILET_PAPER_MS_TICK, []() { tpr.update(); });

    delay(500);
    tpr.calibrate();
}

void printSensorsStates()
{
    Serial.println("IR1: " + String(ir1.getValue()) + " IR2:" + String(ir2.getValue()) + " PIR:" + String(pir.getValue()));
}

void updateIRSensors()
{
    ir1.setValue(!digitalRead(IR_BOTTOM_PIN));
    ir2.setValue(!digitalRead(IR_TOP_PIN));
}

void updatePIRSensors()
{
    pir.setValue(digitalRead(PIR_PIN));
    digitalWrite(LED_BUILTIN, pir.getValue() ? HIGH : LOW);
}

void loop()
{
    tpclient.update();
    delay(500);
}
