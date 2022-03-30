#include "TPClient.hpp"

TPClient::TPClient(const char* ssid, const char* password, const char* url, ToiletPaperRoll& tpr)
    : _ssid(ssid)
    , _password(password)
    , _url(url)
    , _tpr(tpr)
{
    WiFi.begin();
    _ws.onMessage([](websockets::WebsocketsMessage message) {
        Serial.print("Message received is text: ");
        Serial.print(message.isText());
        Serial.print(": ");
        Serial.println(message.c_str());
    });
}

void TPClient::update()
{
    if (!manageWifi() || !manageServerConnection())
        return;
    _ws.send("Hello! this is me, tpd");
    _ws.poll();
}

bool TPClient::manageServerConnection()
{
    if (_ws.available())
        return true;
    return _ws.connect(_url);
}

bool TPClient::manageWifi()
{
    Serial.print("Wifi status: ");
    Serial.println(WiFi.status());
    if (WiFi.status() != WL_CONNECTED) {
        if (_wifiCounter == 0) {
            WiFi.begin(_ssid, _password);
            _wifiCounter = 60; // 30s
        }
        _wifiCounter--;
    }
}