#ifndef _COMMANDS_SERVER_H
#define _COMMANDS_SERVER_H

#include <JSON_Listener.h>
#include <JSON_Decoder.h>
#include <WiFiServer.h>
#include "CommandsHandler.h"

class CommandsServer : public JsonListener {
public:
  CommandsServer(CommandsHandler *commandHandler);
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
  virtual void error(const char *message);
#endif
  virtual void key(const char *key);
  virtual void value(const char *val);

private:
  CommandsHandler *commandHandler;
  String currentKey;
  WiFiServer wifiServer;
};

#endif
