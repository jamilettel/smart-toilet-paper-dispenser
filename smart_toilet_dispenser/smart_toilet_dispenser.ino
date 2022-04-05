#include <Ticker.h>

#include "CircularBuffer.hpp"
#include "SensorValue.hpp"
#include "TPClient.hpp"
#include "ToiletPaperRoll.hpp"
#include "settings.hpp"

SensorValue ir1(10);
SensorValue ir2(10);

Ticker irUpdateTicker;
// Ticker printStatesTicker;
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
    tpr.attach(SERVO_PIN);

    irUpdateTicker.attach_ms(10, updateIRSensors);
    // printStatesTicker.attach_ms(500, printSensorsStates);
    tprTicker.attach_ms(TOILET_PAPER_MS_TICK, []() { tpr.update(); });

    delay(500);
}

// void printSensorsStates()
// {
//     Serial.println("IR1:" + String(ir1.getValue()) + " IR2:" + String(ir2.getValue()));
// }

void updateIRSensors()
{
    ir1.setValue(!digitalRead(IR_BOTTOM_PIN));
    ir2.setValue(!digitalRead(IR_TOP_PIN));
}

void loop()
{
    tpclient.update();
    delay(500);
}
