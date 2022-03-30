#include "TPClient.hpp"
#include <vector>

static CommandHandler handlers[] = {
    { [](const std::vector<String>& args, ToiletPaperRoll& tpr) {

     },
        "calibrate" },
    { [](const std::vector<String>& args, ToiletPaperRoll& tpr) {

     },
        "get-percentage-left" },
    { [](const std::vector<String>& args, ToiletPaperRoll& tpr) {

     },
        "stop" },
    { [](const std::vector<String>& args, ToiletPaperRoll& tpr) {

     },
        "continue" },
};

static std::vector<String> strsplit(String str, char delimiter = ' ')
{
    std::vector<String> words;
    int current = 0;
    while (current < str.length() && str[current] == delimiter)
        current++;
    int next = current;
    while (current < str.length()) {
        while (next < str.length() && str[next] == delimiter)
            next++;
        current = next;
        while (next < str.length() && str[next] != delimiter)
            next++;
        if (next >= str.length() || str[next] != delimiter)
            return words;
        if (next != current) {
            words.push_back(str.substring(current, next));
            current = next;
        }
    }
    return words;
}

TPClient::TPClient(const char* ssid, const char* password, const char* url, ToiletPaperRoll& tpr)
    : _ssid(ssid)
    , _password(password)
    , _url(url)
    , _tpr(tpr)
{
    WiFi.begin();
    _ws.onMessage([](websockets::WebsocketsMessage message) {
        if (message.isText()) {
            auto command = strsplit(message.c_str());
            for (const auto& str : command) {
            }
        }
    });
}

void TPClient::update()
{
    if (!manageWifi() || !manageServerConnection())
        return;
    _ws.poll();
    _ws.send("Hello tout le monde    comment ca va??? ");
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