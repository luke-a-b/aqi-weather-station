#ifndef _WEB_CONFIG_H
#define _WEB_CONFIG_H

#include <DoubleResetDetector.h>
#include <ESP8266WiFi.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h> 

#include "Config.h"

#define CONFIG_VERSION "lb07"

#define DRD_TIMEOUT 10
#define DRD_ADDRESS 0

#define STRING_LEN 128

class WebConfig
{
public:
    WebConfig(Config *config);
    void begin(std::function<void(IPAddress)> wifiConnectedHandler, std::function<void(const char *, const char *)> connectApHandler, std::function<void(const char *)> connectWifiHandler);
    void update();

private:
    Config *config;

    DoubleResetDetector drd;

    DNSServer dnsServer;
    WebServer server;
    IotWebConf iotWebConf;
    //boolean validateAqiUrl(String url);
    //boolean formValidator();
    boolean connectAp(const char *apName, const char *password);
    void connectWifi(const char *ssid, const char *password);
    const int32_t calcWiFiSignalQuality(int32_t rssi);
    void handleRoot();
    //void setConfig(Config *config);
    // void displayConfig();

private:
    // IotWebConfCheckboxParameter *IsMetricSelectedParam;
    // IotWebConfCheckboxParameter *IsClock24hStyleSelectedParam;
    // IotWebConfCheckboxParameter *IsClockSilhouetteEnabledParam;
    // IotWebConfNumberParameter *BacklightTimeoutParam;

    // IotWebConfTextParameter *OwmApiKeyParam;
    // IotWebConfTextParameter *OwmLocationIdParam;
    // IotWebConfSelectParameter *OwmLanguageParam;
    // IotWebConfNumberParameter *OwmRefreshIntervalParam;

    // IotWebConfTextParameter *AqiStationUrlParam1;
    // IotWebConfTextParameter *AqiStationUrlParam2;
    // IotWebConfTextParameter *AqiStationUrlParam3;
    // IotWebConfNumberParameter *AqiRefreshIntervalParam;

    // IotWebConfSelectParameter *LocalTempSensorTypeParam;
    // IotWebConfNumberParameter *LocalTempSensorRefreshIntervalParam;

    // Params *params;

};

#endif