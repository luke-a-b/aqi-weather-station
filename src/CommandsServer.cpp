#include <WiFiClient.h>
#include "Config.h"
#include "CommandsServer.h"

CommandsServer::CommandsServer(CommandsHandler *commandHandler)
    : commandHandler(commandHandler), wifiServer(8080) {
  wifiServer.begin();
}

void CommandsServer::update() {
  WiFiClient client = wifiServer.available();
  uint32_t timeout = millis();
  JSON_Decoder parser;
  parser.setListener(this);
  String body = "";
  uint8_t buf;
#ifdef SHOW_JSON
  int ccount = 0;
#endif
  while (client.available()) {
    size_t length = 1;
    client.read(&buf, length);
    parser.parse(buf);
#ifdef SHOW_JSON
    if (buf == '{' || buf == '[' || buf == '}' || buf == ']')
      DEBUG_PRINTFLN("");
    DEBUG_PRINT(buf);
    if (ccount++ > 100 && buf == ',') {
      ccount = 0;
      DEBUG_PRINTFLN("");
    }
#endif
    if ((millis() - timeout) > 1000UL) {
      DEBUG_PRINTFLN("[CommandsServer] JSON client timeout");
      break;
    }
    // give WiFi and TCP/IP libraries a chance to handle pending events
    yield();
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

void CommandsServer::error(const char *message) {
  DEBUG_PRINTF("JSON error: ");
  DEBUG_PRINTLN(message);
}
#endif

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
  if (currentKey == "show_page") {
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
    }
  } else if (currentKey == "backlight") {
    if (value == "on") {
      commandHandler->switchOnBacklight();
    } else if (value == "off") {
      commandHandler->switchOffBacklight();
    } else {
      DEBUG_PRINTF("Error: backlight value unsupported: ");
      DEBUG_PRINTLN(value);
    }
  } else if (currentKey == "backlight_timeout") {
    commandHandler->setBacklightTimeout(value.toInt());
  } else {
    DEBUG_PRINTF("Error: command not supported: ");
    DEBUG_PRINTLN(currentKey);
  }
}
