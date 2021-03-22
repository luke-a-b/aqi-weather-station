#ifndef _AQI_DATA_SOURCE_H
#define _AQI_DATA_SOURCE_H

#include <Arduino.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include <JSON_Listener.h>
#include <JSON_Decoder.h>
#include <WiFiUdp.h>
#include <mDNSResolver.h>

typedef struct AqiDataModel {
  char *stationUrl = {0};
  uint16_t pm25 = 0;
  uint16_t pm10 = 0;
  float temp = 0;
  uint16_t humidity = 0;
  uint16_t pressure = 0;
  float index = NAN;
} AqiDataModel;

using namespace mDNSResolver;

class AqiDataSource : public JsonListener {
private:
  WiFiClient *client;
  String currentKey;
  String currentParent;
  String currentArray;
  String currentValueType;

  AqiDataModel *data;

public:
  AqiDataSource();
  void updateCurrent(AqiDataModel *data);
  void update();

protected: // implements: JsonListener
  virtual void key(const char *key);
  virtual void value(const char *val);
  virtual void endArray();
  virtual void endObject();
  virtual void startArray();
  virtual void startObject();
#ifdef SHOW_JSON
  virtual void whitespace(char c);
  virtual void startDocument();
  virtual void endDocument();
  virtual void error(const char *message);
#endif

private:
  boolean parseUrl(String url, AqiDataModel *data);

  WiFiUDP udp;
  Resolver resolver;

  String host;
  int port;
  String path;
  boolean https;
};

#endif
