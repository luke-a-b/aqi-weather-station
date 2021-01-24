#ifndef _WEB_CONFIG_H
#define _WEB_CONFIG_H

#include <DoubleResetDetector.h>
#include <ESP8266WiFi.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>
#ifdef ESP8266
# include <ESP8266HTTPUpdateServer.h>
#elif defined(ESP32)
# include <IotWebConfESP32HTTPUpdateServer.h>
#endif
#include "Config.h"

#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0

const char INTL_HTML_HEAD[] PROGMEM = "<!DOCTYPE html><html lang=\"{l}\"><head><meta charset=\"utf-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>\n";

class HtmlFormatProvider : public iotwebconf::HtmlFormatProvider
{
public:
    virtual String getHead()
    {
        String to_return = FPSTR(INTL_HTML_HEAD);
        to_return.replace(F("{l}"), FPSTR(INTL_LANG_ID));
        return to_return;
    }
    virtual String getFormEnd() { return FPSTR(INTL_HTML_FORM_END); }
    virtual String getFormSaved() { return FPSTR(INTL_HTML_SAVED); }
    virtual String getUpdate() { return FPSTR(INTL_HTML_UPDATE); }
    virtual String getConfigVer() { return FPSTR(INTL_HTML_CONFIG_VER); }

};

class WebConfig
{
public:
    WebConfig(Config *config);
    void begin(std::function<void(IPAddress)> wifiConnectedHandler, std::function<void(const char *, const char *)> connectApHandler, std::function<void(const char *)> connectWifiHandler);
    void update();

private:
    Config *config;
    DoubleResetDetector drd;

    HtmlFormatProvider htmlFormatProvider;

    DNSServer dnsServer;
    WebServer server;
    IotWebConf iotWebConf;

    #ifdef ESP8266
        ESP8266HTTPUpdateServer httpUpdater;
    #elif defined(ESP32)
        HTTPUpdateServer httpUpdater;
    #endif

    bool connectAp(const char *apName, const char *password);
    void connectWifi(const char *ssid, const char *password);
    const int32_t calcWiFiSignalQuality(int32_t rssi);
    void handleRoot();
};

#endif