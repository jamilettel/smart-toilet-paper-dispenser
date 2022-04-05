#include "TPClient.hpp"
#include "settings.hpp"
#include <vector>

static CommandHandler handlers[] = {
    { &TPClient::calibrate, "calibrate" },
    { &TPClient::stop, "stop" },
    { &TPClient::continueNormal, "start" },
    { &TPClient::measure, "measure" },
    { &TPClient::setValue, "value" },
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
        if (next >= str.length()) {
            if (str[next - 1] == delimiter)
                return words;
            words.push_back(str.substring(current));
            return words;
        }
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
    _ws.onMessage([this](websockets::WebsocketsMessage message) {
        if (message.isText()) {
            auto args = strsplit(message.c_str());
            if (args.size() == 0) {
                Serial.println("Received empty command");
                return;
            }
            for (const auto& handler : handlers) {
                if (handler.command == args[0]) {
                    Serial.println("Executing command: " + args[0]);
                    (this->*handler.handler)(args);
                    return;
                }
            }
            Serial.println("Command not found: " + args[0]);
        }
    });
    tpr.onStateChange = [this](ToiletPaperRoll::State) {
        sendState();
    };
}

void TPClient::update()
{
    static bool blink = false;
    if (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, LOW);
    } else if (!_ws.available()) {
        digitalWrite(LED_BUILTIN, blink ? HIGH : LOW);
        blink = !blink;
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }

    if (!manageWifi() || !manageServerConnection())
        return;
    _ws.poll();
}

bool TPClient::manageServerConnection()
{
    static int timer = 0;
    static bool gotSettings = false;

    if (_ws.available()) {
        if (--timer == 0) {
            timer = 30;
            sendState();
        }
        if (!gotSettings) {
            _ws.send("get full-one-sheet-time");
            _ws.send("get one-sheet-time");
            gotSettings = true;
        }
        return true;
    }
    return _ws.connect(_url);
}

bool TPClient::manageWifi()
{
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
    if (WiFi.status() != WL_CONNECTED) {
        if (_wifiCounter == 0) {
            WiFi.begin(_ssid, _password);
            _wifiCounter = 60; // 30s
        }
        _wifiCounter--;
    }
}

void TPClient::calibrate(const std::vector<String>& args)
{
    _tpr.calibrate();
}

void TPClient::stop(const std::vector<String>& args)
{
    _tpr.setState(ToiletPaperRoll::STOPPED);
}

void TPClient::continueNormal(const std::vector<String>& args)
{
    _tpr.clearActions();
    _tpr.setState(ToiletPaperRoll::WORKING);
}

void TPClient::measure(const std::vector<String>& args)
{
    _tpr.updateRollTime();
}

void TPClient::setValue(const std::vector<String>& args)
{
    if (args.size() != 3)
        return;
    if (!args.at(1).compareTo("one-sheet-time")) {
        long long ost = atoll(args.at(2).c_str());
        _tpr.setOneSheetTime(ost);
    } else if (!args.at(1).compareTo("full-one-sheet-time")) {
        long long fost = atoll(args.at(2).c_str());
        _tpr.setFullOneSheetTime(fost);
    }
}

void TPClient::sendState()
{
    if (_ws.available()) {
        _ws.send("status " + ToiletPaperRoll::stateToString(_tpr.getState()));
        float percentage = _tpr.percentageLeft();
        if (percentage >= 0) {
            _ws.send("set-percentage " + String(percentage));
            _ws.send("save one-sheet-time " + String(_tpr.getOneSheetTime()));
            _ws.send("save full-one-sheet-time " + String(_tpr.getFullOneSheetTime()));
        }
    }
}
