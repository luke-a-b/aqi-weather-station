#include "WebConfig.h"
#include "DataSources/LocalTempSensorSource.h"

const char RootHtml[] PROGMEM = "<!DOCTYPE html><html lang='en'><head><meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>\
    <title>ESP Weather Station</title></head><body><p>Hello to {thing_name}!</p>\
    <p>IP address: {ip}<br>\
    Subnet mask: {mask}<br>\
    Gateway: {gateway}<br>\
    MAC address: {mac}</p>\
    <p>SSID: {ssid}<br>\
    RSSI: {rssi} dBm<br>\
    Signal: {signal}%</p>\
    <p>Go to <a href='config'>configure page</a> to change settings.</p>\
    </body></html>\n";

WebConfig::WebConfig(Config *config) : 
    config(config), 
    drd(DRD_TIMEOUT, DRD_ADDRESS), 
    server(80), 
    iotWebConf("ESP Weather Station", &dnsServer, &server, NULL, CONFIG_VERSION)
{
    if (!this->drd.detectDoubleReset())
    {
        this->iotWebConf.skipApStartup();
    }
}

void WebConfig::begin(std::function<void(IPAddress)> wifiConnectedHandler, std::function<void(const char *, const char *)> connectApHandler, std::function<void(const char *)> connectWifiHandler)
{
    config->addCustomWebParams(&iotWebConf);

    this->iotWebConf.setFormValidator([this] { return this->config->validateWebParams(&this->server); });
    this->iotWebConf.setWifiConnectionCallback([wifiConnectedHandler, this] {
        Serial.print(F("Local IP: "));
        Serial.println(WiFi.localIP().toString());
        config->print();
        wifiConnectedHandler(WiFi.localIP());
    });
    this->iotWebConf.setApConnectionHandler([connectApHandler, this](const char *apName, const char *password) {
        connectApHandler(apName, password);
        return this->connectAp(apName, password);
    });
    this->iotWebConf.setWifiConnectionHandler([connectWifiHandler, this](const char *ssid, const char *password) {
        connectWifiHandler(ssid);
        return this->connectWifi(ssid, password);
    });
    this->iotWebConf.setConfigSavedCallback([] {
        ESP.restart();
    });

    this->iotWebConf.init();

    this->server.on("/", [this] { this->handleRoot(); });
    this->server.on("/config", [this] { this->iotWebConf.handleConfig(); });
    this->server.onNotFound([this]() { this->iotWebConf.handleNotFound(); });
}

void WebConfig::update()
{
    this->iotWebConf.doLoop();
    this->drd.loop();
}

// boolean WebConfig::validateAqiUrl(String url)
// {
//     return url.length() == 0 || // accepting empty url
//            (url.startsWith(F("http://")) || url.startsWith(F("https://"))) && url.endsWith(F("/data.json"));
// }

// boolean WebConfig::formValidator()
// {
//     Serial.println(F("Validating form."));
//     boolean valid = true;

//     if (this->server->arg(this->OwmApiKeyParam->getId()).length() == 0)
//     {
//         this->OwmApiKeyParam->errorMessage = "Please provide OWM API key.";
//         valid = false;
//     }

//     if (this->server->arg(this->OwmLocationIdParam->getId()).length() == 0)
//     {
//         this->OwmLocationIdParam->errorMessage = "Please provide OWM location ID.";
//         valid = false;
//     }

//     const char *aqiError = "Please profide URL in following format http://...../data.json or https://...../data.json";
//     if (!validateAqiUrl(this->server->arg(this->AqiStationUrlParam1->getId())))
//     {
//         this->AqiStationUrlParam1->errorMessage = aqiError;
//         valid = false;
//     }
//     if (!validateAqiUrl(this->server->arg(this->AqiStationUrlParam2->getId())))
//     {
//         this->AqiStationUrlParam2->errorMessage = aqiError;
//         valid = false;
//     }
//     if (!validateAqiUrl(this->server->arg(this->AqiStationUrlParam3->getId())))
//     {
//         this->AqiStationUrlParam3->errorMessage = aqiError;
//         valid = false;
//     }

//     return valid;
// }

boolean WebConfig::connectAp(const char *apName, const char *password)
{
    Serial.println(F("Creating access point"));
    Serial.print(F("SSID:     "));
    Serial.println(apName);
    Serial.print(F("Password: "));
    Serial.println(password);
    return WiFi.softAP(apName, password);
}

void WebConfig::connectWifi(const char *ssid, const char *password)
{
    Serial.print(F("Connecting to WiFi "));
    Serial.println(ssid);
    wifi_set_sleep_type(NONE_SLEEP_T); //LIGHT_SLEEP_T and MODE_SLEEP_T
    WiFi.begin(ssid, password);
}

const int32_t WebConfig::calcWiFiSignalQuality(int32_t rssi)
{
    if (rssi > -50)
    {
        rssi = -50;
    }
    if (rssi < -100)
    {
        rssi = -100;
    }
    return (rssi + 100) * 2;
}

void WebConfig::handleRoot()
{
    if (this->iotWebConf.handleCaptivePortal())
    {
        return;
    }
    String s = FPSTR(RootHtml);
    s.replace(F("{thing_name}"), String(this->iotWebConf.getThingName()));
    if (WiFi.status() == WL_CONNECTED)
    {
        s.replace(F("{ip}"), WiFi.localIP().toString());
        s.replace(F("{gateway}"), WiFi.gatewayIP().toString());
        s.replace(F("{mask}"), WiFi.subnetMask().toString());
        s.replace(F("{mac}"), WiFi.macAddress());
        s.replace(F("{ssid}"), WiFi.SSID());
        s.replace(F("{rssi}"), String(WiFi.RSSI()));
        s.replace(F("{signal}"), String(WebConfig::calcWiFiSignalQuality(WiFi.RSSI())));
    }
    this->server.send(200, "text/html", s);
}
