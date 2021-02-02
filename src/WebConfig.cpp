#include "WebConfig.h"
#include "DataSources/LocalTempSensorSource.h"

WebConfig::WebConfig(Config *config)
    : config(config), drd(DRD_TIMEOUT, DRD_ADDRESS), server(80),
      iotWebConf("AQI Weather Station", &dnsServer, &server, NULL,
                 CONFIG_VERSION) {}

void WebConfig::begin(
    std::function<void(IPAddress)> wifiConnectedHandler,
    std::function<void(const char *, const char *)> connectApHandler,
    std::function<void(const char *)> connectWifiHandler) {
  this->config->addCustomWebParams(&iotWebConf);

  this->iotWebConf.setHtmlFormatProvider(&(this->htmlFormatProvider));
  this->iotWebConf.setFormValidator(
      [this](iotwebconf::WebRequestWrapper *webRequestWrapper) {
        return this->config->validateWebParams(webRequestWrapper);
      });
  this->iotWebConf.setWifiConnectionCallback([wifiConnectedHandler, this] {
    DEBUG_PRINTF("Local IP: ");
    DEBUG_PRINTLN(WiFi.localIP().toString());
    config->print();
    wifiConnectedHandler(WiFi.localIP());
  });
  this->iotWebConf.setApConnectionHandler(
      [connectApHandler, this](const char *apName, const char *password) {
        connectApHandler(apName, password);
        return this->connectAp(apName, password);
      });
  this->iotWebConf.setWifiConnectionHandler(
      [connectWifiHandler, this](const char *ssid, const char *password) {
        connectWifiHandler(ssid);
        return this->connectWifi(ssid, password);
      });
  this->iotWebConf.setConfigSavedCallback([this] {
    DEBUG_PRINTFLN("Rebooting after 1 second.");
    this->iotWebConf.delay(1000);
    ESP.restart();
  });
  this->iotWebConf.setupUpdateServer(
      [this](const char *updatePath) {
        this->httpUpdater.setup(&server, updatePath);
      },
      [this](const char *userName, char *password) {
        this->httpUpdater.updateCredentials(userName, password);
      });

  if (!this->drd.detectDoubleReset()) {
    this->iotWebConf.skipApStartup();
  }
  this->iotWebConf.init();

  this->server.on("/", [this] { this->handleRoot(); });
  this->server.on("/config", [this] { this->iotWebConf.handleConfig(); });
  this->server.onNotFound([this]() { this->iotWebConf.handleNotFound(); });
}

void WebConfig::update() {
  this->iotWebConf.doLoop();
  this->drd.loop();
}

bool WebConfig::connectAp(const char *apName, const char *password) {
  DEBUG_PRINTFLN("Creating access point");
  DEBUG_PRINTF("SSID:     ");
  DEBUG_PRINTLN(apName);
  DEBUG_PRINTF("Password: ");
  DEBUG_PRINTLN(password);
  return WiFi.softAP(apName, password);
}

void WebConfig::connectWifi(const char *ssid, const char *password) {

  DEBUG_PRINTF("Connecting to WiFi ");
  DEBUG_PRINTLN(ssid);
  wifi_set_sleep_type(NONE_SLEEP_T); // LIGHT_SLEEP_T and MODE_SLEEP_T
  WiFi.begin(ssid, password);
}

const int32_t WebConfig::calcWiFiSignalQuality(int32_t rssi) {
  if (rssi > -50) {
    rssi = -50;
  }
  if (rssi < -100) {
    rssi = -100;
  }
  return (rssi + 100) * 2;
}

void WebConfig::handleRoot() {
  if (this->iotWebConf.handleCaptivePortal()) {
    return;
  }
  String content = htmlFormatProvider.getHead();
  String br = F("<br>");
  content.replace(F("{v}"), String(this->iotWebConf.getThingName()));
  content += htmlFormatProvider.getStyle();
  content += htmlFormatProvider.getHeadEnd();
  content += FPSTR(INTL_HTML_GREETINGS);
  content.replace(F("{v}"), String(this->iotWebConf.getThingName()));

  if (WiFi.status() == WL_CONNECTED) {
    content += String(FPSTR(INTL_IP_ADDRESS)) + WiFi.localIP().toString() + br;
    content += String(FPSTR(INTL_GATEWAY)) + WiFi.gatewayIP().toString() + br;
    content +=
        String(FPSTR(INTL_SUBNET_MASK)) + WiFi.subnetMask().toString() + br;
    content += String(FPSTR(INTL_MAC_ADDRESS)) + WiFi.macAddress() + br;
    content += String(FPSTR(INTL_SSID)) + WiFi.SSID() + br;
    content += String(FPSTR(INTL_RSSI)) + String(WiFi.RSSI()) +
               String(FPSTR(CONST_UNIT_DBM)) + br;
    content += String(FPSTR(INTL_WIFI_QALITY)) +
               String(WebConfig::calcWiFiSignalQuality(WiFi.RSSI())) + "%" + br;
  }
  content += FPSTR(INTL_HTML_CONFIG_LINK);
  content += String(F("<p><b>Credits:</b>")) + br;
  content +=
      String(F("<a href=\"https://openweathermap.org/\">OpenWeather</a>")) + br;
  content += String(F("<a "
                      "href=\"http://vclouds.deviantart.com/gallery/#/"
                      "d2ynulp\">Deviant Art</a>")) +
             br;
  content += String(F("<a href=\"https://nettigo.pl/\">Nettigo</a>")) + br;
  content +=
      String(F("<a href=\"https://sensor.community/\">sensor.community</a>")) +
      br;
  content += String(F("<a href=\"https://aqi.eco/\">aqi.eco</a>")) + br;
  content += String(F("<a href=\"https://blog.squix.org/\">Squix</a>")) + br;
  content += String(F("<a href=\"https://github.com/Bodmer\">Bodmer</a>")) + br;
  content +=
      String(F("<a href=\"https://github.com/trekawek\">Tomek Rekawek</a>")) +
      br;
  content += String(F("</p>"));
  content += htmlFormatProvider.getEnd();

  this->server.send(200, "text/html", content);
}
