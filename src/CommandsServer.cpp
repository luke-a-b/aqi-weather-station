#include <WiFiClient.h>
#include "Config.h"
#include "CommandsServer.h"

CommandsServer::CommandsServer(CommandsHandler *commandHandler,
                               LocalSensorModel *localSensor)
    : commandHandler(commandHandler), localSensor(localSensor),
      webServer(8080) {
  webServer.begin();
  webServer.on("/backlight", [this] { this->handleBacklight(); });
  webServer.on("/showpage", [this] { this->handleCommand(); });
  webServer.on("/localsensor", [this] { this->handleLocalSensor(); });
  webServer.onNotFound([this] { this->sendError(); });
}

void CommandsServer::update() { webServer.handleClient(); }

void CommandsServer::handleBacklight() {
  if (webServer.method() == HTTP_GET) {
    sendOk();
  } else {
    handleCommand();
  }
}

void CommandsServer::handleCommand() {
  JSON_Decoder parser;
  parser.setListener(this);
  String body = webServer.arg("plain");

#ifdef SHOW_JSON
  DEBUG_PRINTLN(body);
#endif

  parseResult = body.length() > 0 ? true : false;

  for (uint8_t i = 0; i < body.length(); i++) {
    parseResult &= parser.parse(body[i]);
    if (!parseResult)
      break;
    yield();
  }

  if (parseResult) {
    sendOk();
  } else {
    sendError();
  }
}

void CommandsServer::sendOk() {
  String response = FPSTR(RESPONSE_DATA_JSON);
  response.replace("{B}", commandHandler->isBacklightActive() ? "\"true\""
                                                              : "\"false\"");
  webServer.send(200, "application/json", response);
}

void CommandsServer::sendError() {
  webServer.send(404, "text/plain", F("Not supported\n"));
}

void CommandsServer::handleLocalSensor() {
  if (localSensor->isLocalTempSensorEnabled()) {
    String response = FPSTR(LOCAL_SENSOR_DATA_JSON);
    response.replace("{T}", String(localSensor->getLocalTemp()));
    response.replace("{H}", String(localSensor->getLocalHumidity()));
    webServer.send(200, "application/json", response);
  } else {
    sendError();
  }
}

#ifdef SHOW_JSON
void CommandsServer::whitespace(char c) { DEBUG_PRINTF("JSON whitespace"); }

void CommandsServer::startDocument() { DEBUG_PRINTF("JSON Start Document"); }

void CommandsServer::startArray() { DEBUG_PRINTFLN("JSON start array"); }

void CommandsServer::endArray() { DEBUG_PRINTFLN("JSON end array"); }

void CommandsServer::startObject() { DEBUG_PRINTFLN("JSON start object"); }

void CommandsServer::endObject() { DEBUG_PRINTFLN("JSON end object"); }

void CommandsServer::endDocument() { DEBUG_PRINTFLN("JSON end document"); }
#endif

void CommandsServer::error(const char *message) {
  DEBUG_PRINTF("JSON error: ");
  DEBUG_PRINTLN(message);
  parseResult = false;
}

void CommandsServer::key(const char *key) {
#ifdef SHOW_JSON
  DEBUG_PRINTF("JSON key: ");
  DEBUG_PRINTLN(key);
#endif
  currentKey = key;
}

void CommandsServer::value(const char *val) {
  String value = val;
#ifdef SHOW_JSON
  DEBUG_PRINTF("JSON key: ");
  DEBUG_PRINT(currentKey);
  DEBUG_PRINTF(", value: ");
  DEBUG_PRINTLN(value);
#endif
  if (currentKey == "page") {
    if (value == "main") {
      commandHandler->showMainPage();
    } else if (value == "aqi") {
      commandHandler->showAqiPage();
    } else if (value == "current_weather") {
      commandHandler->showCurrentWeatherDetail();
    } else if (value == "forecast") {
      commandHandler->showForecastPage();
    } else if (value == "about") {
      commandHandler->showAboutPage();
    } else {
      DEBUG_PRINTF("Error: show_page value unsupported: ");
      DEBUG_PRINTLN(value);
      parseResult = false;
    }
  } else if (currentKey == "active") {
    if (value == "true") {
      commandHandler->switchOnBacklight();
    } else if (value == "false") {
      commandHandler->switchOffBacklight();
    } else {
      DEBUG_PRINTF("Error: backlight value unsupported: ");
      DEBUG_PRINTLN(value);
      parseResult = false;
    }
  } else if (currentKey == "timeout") {
    commandHandler->setBacklightTimeout(value.toInt());
  } else {
    DEBUG_PRINTF("Error: command not supported: ");
    DEBUG_PRINTLN(currentKey);
    parseResult = false;
  }
}
