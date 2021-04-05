#ifndef _COMMANDS_SERVER_H
#define _COMMANDS_SERVER_H

#include <JSON_Listener.h>
#include <JSON_Decoder.h>
#include <ESP8266WebServer.h>
#include "CommandsHandler.h"
#include "Models\LocalSensorModel.h"

class CommandsServer : public JsonListener {
public:
  CommandsServer(CommandsHandler *commandHandler,
                 LocalSensorModel *localSensor);
  void update();

protected: // implements: JsonListener
#ifdef SHOW_JSON
  virtual void whitespace(char c);
  virtual void startDocument();
  virtual void endArray();
  virtual void endObject();
  virtual void endDocument();
  virtual void startArray();
  virtual void startObject();
#endif
  virtual void error(const char *message);
  virtual void key(const char *key);
  virtual void value(const char *val);

private:
  CommandsHandler *commandHandler;
  LocalSensorModel *localSensor;
  String currentKey;
  ESP8266WebServer webServer;

  bool parseResult;

  void handleBacklight();
  void handleCommand();
  void handleLocalSensor();
  void sendOk();
  void sendError();
};

#endif
