#include "DataSources/AqiDataSource.h"
#include "PollutionLevels.h"
#include "assert.h"
#include "Config.h"

// leave space at the beggining of each string so indexOf could be > 0
const char P10_VALUES[] PROGMEM = "SDS_P1,PMS_P1,HPM_P1";
const char P25_VALUES[] PROGMEM = "SDS_P2,PMS_P2,HPM_P2";
const char TEMP_VALUES[] PROGMEM = "BME280_temperature,BMP_temperature";
const char HUMIDITY_VALUES[] PROGMEM = "BME280_humidity";
const char PRESSURE_VALUES[] PROGMEM = "BME280_pressure,BMP_pressure";

AqiDataSource::AqiDataSource() : resolver(udp) {}

void AqiDataSource::update() { resolver.loop(); }

void AqiDataSource::updateCurrent(AqiDataModel *data) {
  assert(data != nullptr);
  if (parseUrl(data->stationUrl, data)) {
    if (this->host.endsWith(F(".local"))) {
      IPAddress ip = resolver.search(this->host.c_str());
      if (ip != INADDR_NONE) {
        DEBUG_PRINTF("Resolved IP for ");
        DEBUG_PRINTLN(this->host);
        this->host = ip.toString();
      } else {
        DEBUG_PRINTF("Error: cannot resolve IP for ");
        DEBUG_PRINTLN(this->host);
        return;
      }
    }
    this->data = data;
    this->data->index = NAN;
    JSON_Decoder parser;
    parser.setListener(this);

    HTTPClient *http = new HTTPClient();
    if (this->https) {
      DEBUG_PRINTF("[HTTPS] ");
      BearSSL::WiFiClientSecure *httpsClient = new BearSSL::WiFiClientSecure();
      httpsClient->setInsecure();
      this->client = httpsClient;
    } else {
      DEBUG_PRINTF("[HTTP] ");
      this->client = new WiFiClient();
    }
    DEBUG_PRINTF("GET -> ");
    DEBUG_PRINTLN(this->host + this->path);
    http->begin(*client, this->host, this->port, this->path, this->https);

    http->setReuse(false);
    // start connection and send HTTP header
    int httpCode = http->GET();

    if (httpCode == HTTP_CODE_OK) {
      char c = 0;
#ifdef SHOW_JSON
      int ccount = 0;
#endif
      // get tcp stream
      WiFiClient *stream = http->getStreamPtr();
      uint32_t timeout = millis();
      // read all data from server
      while (http->connected() || stream->available()) {
        if (stream->available()) {
          c = stream->read();
          parser.parse(c);
#ifdef SHOW_JSON
          if (c == '{' || c == '[' || c == '}' || c == ']')
            DEBUG_PRINTFLN("");
          DEBUG_PRINT(c);
          if (ccount++ > 100 && c == ',') {
            ccount = 0;
            DEBUG_PRINTFLN("");
          }
#endif
        }
        if ((millis() - timeout) > 8000UL) {
          DEBUG_PRINTFLN("[AqiDataSource] JSON client timeout");
          break;
        }
        // give WiFi and TCP/IP libraries a chance to handle pending events
        yield();
      }
      if (isnan(data->index) && (data->pm25 != 0 || data->pm10 != 0)) {
        data->index = PollutionLevels::getLevel(data->pm25, data->pm10);
      }
      DEBUG_PRINTF("AQI: ");
      DEBUG_PRINTLN(data->index);
    } else {
      DEBUG_PRINTF("Error: recieved http response: ");
      DEBUG_PRINTLN(httpCode);
    }

    parser.reset();
    delete http;
    delete this->client;
    this->data = nullptr;
    this->client = nullptr;
  }
}

boolean AqiDataSource::parseUrl(String url, AqiDataModel * /*data*/) {
  // assert(data != nullptr);

  int index = url.indexOf(':');
  if (index < 0) {
    DEBUG_PRINTFLN("Error: parseUrl - no protocol defined");
    return false;
  }

  String protocol = url.substring(0, index);
  url.remove(0, (index + 3));

  if (protocol == "http") {
    this->port = 80;
    this->https = false;
  } else if (protocol == "https") {
    this->port = 443;
    this->https = true;
  } else {
    DEBUG_PRINTFLN("Error: parseUrl - unknown protocol");
    return false;
  }

  index = url.indexOf('/');
  this->host = url.substring(0, index);
  url.remove(0, index);

  index = this->host.indexOf(':');
  if (index >= 0) {
    this->port = this->host.substring(index + 1).toInt();
    this->host.remove(0, (index + 1));
  }
  this->path = url;
  return true;
}

void AqiDataSource::key(const char *key) {
#ifdef SHOW_JSON
  DEBUG_PRINTF("JSON key: ");
  DEBUG_PRINTLN(key);
#endif
  currentKey = key;
}

void AqiDataSource::value(const char *val) {
  String value = val;
#ifdef SHOW_JSON
  DEBUG_PRINTF("JSON key: ");
  DEBUG_PRINT(currentKey);
  DEBUG_PRINTF(", value: ");
  DEBUG_PRINTLN(value);
#endif
  assert(data != nullptr);
  // Sensor.Community parse
  if (currentArray == "sensordatavalues") {
    if (currentKey == "value_type") {
      this->currentValueType = value;
    }
    if (currentKey == "value") {

      if (String(FPSTR(P10_VALUES)).indexOf(this->currentValueType) > -1) {
        this->data->pm10 = value.toInt();
      } else if (String(FPSTR(P25_VALUES)).indexOf(this->currentValueType) >
                 -1) {
        this->data->pm25 = value.toInt();
      } else if (String(FPSTR(TEMP_VALUES)).indexOf(this->currentValueType) >
                 -1) {
        this->data->temp = value.toFloat();
      } else if (String(FPSTR(HUMIDITY_VALUES))
                     .indexOf(this->currentValueType) > -1) {
        this->data->humidity = value.toInt();
      } else if (String(FPSTR(PRESSURE_VALUES))
                     .indexOf(this->currentValueType) > -1) {
        this->data->pressure = value.toInt() / 100;
      }
    }
  }
  // Aqi Eco parse
  if (currentParent == "last_data") {
    if (currentKey == "pm25") {
      this->data->pm25 = value.toInt();
    }
    if (currentKey == "pm10") {
      this->data->pm10 = value.toInt();
    }
    if (currentKey == "temperature") {
      this->data->temp = value.toFloat();
    }
    if (currentKey == "pressure") {
      this->data->pressure = value.toInt();
    }
    if (currentKey == "humidity") {
      this->data->humidity = value.toInt();
    }
  }
  if (currentParent == "average_1h") {
    if (currentKey == "index_num") {
      this->data->index = value.toFloat();
    }
  }
}

void AqiDataSource::startArray() {
#ifdef SHOW_JSON
  DEBUG_PRINTFLN("JSON start array");
#endif
  currentArray = currentKey;
}

void AqiDataSource::endArray() {
#ifdef SHOW_JSON
  DEBUG_PRINTFLN("JSON end array");
#endif
  currentArray = "";
}

void AqiDataSource::startObject() {
#ifdef SHOW_JSON
  DEBUG_PRINTFLN("JSON start object");
#endif
  currentParent = currentKey;
}

void AqiDataSource::endObject() {
#ifdef SHOW_JSON
  DEBUG_PRINTFLN("JSON end object");
#endif
  currentParent = "";
}

#ifdef SHOW_JSON
void AqiDataSource::whitespace(char c) { DEBUG_PRINTF("JSON whitespace"); }

void AqiDataSource::startDocument() { DEBUG_PRINTF("JSON Start Document"); }

void AqiDataSource::endDocument() { DEBUG_PRINTFLN("JSON end document"); }

void AqiDataSource::error(const char *message) {
  DEBUG_PRINTF("JSON error: ");
  DEBUG_PRINTLN(message);
}
#endif
