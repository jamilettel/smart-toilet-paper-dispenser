#if !defined(TPCLIENT_HPP_)
#define TPCLIENT_HPP_

#include "ToiletPaperRoll.hpp"

#include <ArduinoWebsockets.h>
#include <WiFi.h>

struct CommandHandler {
    std::function<void(const std::vector<String>& args, ToiletPaperRoll& tpr)> handler;
    String command;
};

class TPClient {
public:
    TPClient(const char* ssid, const char* password, const char* url, ToiletPaperRoll& tpr);
    TPClient(const TPClient&) = delete;
    TPClient& operator=(const TPClient&) = delete;
    ~TPClient() = default;

    void update();

private:
    // returns true if wifi is connected
    bool manageWifi();
    bool manageServerConnection();

private:
    const char* _ssid;
    const char* _password;
    const char* _url;

    websockets::WebsocketsClient _ws;
    int _wifiCounter = 0;
    ToiletPaperRoll& _tpr;
};

#endif // TPCLIENT_HPP_
