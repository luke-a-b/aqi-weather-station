#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>

#define CONFIG_VERSION "0.1"

#define DEBUG_ON

#ifdef DEBUG_ON
#ifndef DEBUG_PRINTF
#define DEBUG_PRINTF(V) Serial.print(F(V))
#define DEBUG_PRINTFLN(V) Serial.println(F(V))
#define DEBUG_PRINT(V) Serial.print(V)
#define DEBUG_PRINTLN(V) Serial.println(V)
#endif
#else
#ifndef DEBUG_PRINTF
#define DEBUG_PRINTF(V)
#define DEBUG_PRINTFLN(V)
#define DEBUG_PRINT(V)
#define DEBUG_PRINTLN(V)
#endif
#endif

#define MAX_LOGOS 5
#define TIME_LOGO_UPDATE 3 // update every 3 seconds

#define NTP_MIN_VALID_EPOCH 1533081600 // August 1st, 2018

#define MAX_AQI_STATIONS 3

#define NTP_SERVER_0 "0.pool.ntp.org"
#define NTP_SERVER_1 "1.pool.ntp.org"
#define NTP_SERVER_2 "2.pool.ntp.org"

#if defined(INTL_EN)
#include "Lang/Intl_EN.h"
#elif defined(INTL_PL)
#include "Lang/Intl_PL.h"
#else
#error "Please define lanugage"
#endif

extern const char *WEEK_DAYS_NAMES_FULL[] PROGMEM;
extern const char *WEEK_DAYS_NAMES[] PROGMEM;
extern const char *MONTHS_NAMES[] PROGMEM;
extern const char *MOON_PHASES[] PROGMEM;

const char CONST_UNIT_CELCIUS[] PROGMEM = "°C";
const char CONST_UNIT_FAHRENHEIT[] PROGMEM = "°F";
const char CONST_UNIT_MPS[] PROGMEM = "m/s";
const char CONST_UNIT_MPH[] PROGMEM = "mph";
const char CONST_UNIT_DBM[] PROGMEM = "dBm";
const char CONST_UNIT_MM[] PROGMEM = "mm";
const char CONST_UNIT_INCH[] PROGMEM = "in";

const char AQI_ICON_PATH[] PROGMEM = "/aqi/{icon}.bmp";
const char MOON_ICON_PATH[] PROGMEM = "/moon/{icon}.bmp";
const char LOGOS_ICON_PATH[] PROGMEM = "/logos/{icon}.bmp";
const char WEATHER_ICON_LARGE_PATH[] PROGMEM =
    "/weather/{folder}/large/{icon}.bmp";
const char WEATHER_ICON_SMALL_PATH[] PROGMEM =
    "/weather/{folder}/small/{icon}.bmp";

////////////////

#define PARAM(name, size, class)                                               \
  char name[size] = {0};                                                       \
  class *name##Param = nullptr

#define CHECKBOX_PARAM_LEN 16
#define CHECKBOX_PARAM(name)                                                   \
  PARAM(name, CHECKBOX_PARAM_LEN, IotWebConfCheckboxParameter)
#define CREATE_CHECKBOX_PARAM(name, label, default)                            \
  this->name##Param = new IotWebConfCheckboxParameter(                         \
      label, PSTR(#name), this->name, CHECKBOX_PARAM_LEN, default)

#define NUMBER_PARAM_LEN 16
#define NUMBER_PARAM(name)                                                     \
  PARAM(name, NUMBER_PARAM_LEN, IotWebConfNumberParameter)
#define CREATE_NUMBER_PARAM(name, label, default, placeholder, custom)         \
  this->name##Param = new IotWebConfNumberParameter(                           \
      label, PSTR(#name), this->name, NUMBER_PARAM_LEN, default,               \
      PSTR(placeholder), PSTR(custom))

#define TEXT_PARAM_LEN 128
#define TEXT_PARAM(name) PARAM(name, TEXT_PARAM_LEN, IotWebConfTextParameter)
#define CREATE_TEXT_PARAM(name, label, default, placeholder)                   \
  this->name##Param = new IotWebConfTextParameter(                             \
      label, PSTR(#name), this->name, TEXT_PARAM_LEN, default, placeholder);

#define SELECT_PARAM_LEN 32
#define SELECT_PARAM(name)                                                     \
  PARAM(name, SELECT_PARAM_LEN, IotWebConfSelectParameter)
#define CREATE_SELECT_PARAM(name, label, values, names, default)               \
  this->name##Param = new IotWebConfSelectParameter(                           \
      label, PSTR(#name), this->name, SELECT_PARAM_LEN, (char *)values,        \
      (char *)names, sizeof(values) / SELECT_PARAM_LEN, SELECT_PARAM_LEN,      \
      default)

#define GROUP_PARAM(name) IotWebConfParameterGroup *group##name
#define CREATE_GROUP_PARAM(name, label)                                        \
  this->group##name = new IotWebConfParameterGroup(label)

enum LocalTempSensorValuesIdx {
  DHT11_IDX = 1,
  DHT12_IDX,
  DHT21_IDX,
  DHT22_IDX
};
const char LocalTempSensorValues[][SELECT_PARAM_LEN] = {"---", "DHT11", "DHT12",
                                                        "DHT21", "DHT22"};
const char LocalTempSensorNames[][SELECT_PARAM_LEN] = {
    "---", "DHT11", "DHT12", "DHT21 (AM2301)", "DHT22 (AM2302, AM2321)"};

const char MeteoIconsValues[][SELECT_PARAM_LEN] = {"vc", "owm"};
const char MeteoIconsNames[][SELECT_PARAM_LEN] = {"VClouds", "Open Weateher"};

class Config {
public:
  Config();

  void addCustomWebParams(IotWebConf *iotWebConf);

  boolean isMetricSelected() {
    return this->IsMetricSelectedParam->isChecked();
  }
  boolean isClock24hStyleSelected() {
    return this->IsClock24hStyleSelectedParam->isChecked();
  }
  boolean isClockSilhouetteEnabled() {
    return this->IsClockSilhouetteEnabledParam->isChecked();
  }
  boolean isLocalTempSensorEnabled() {
    return strcmp(this->LocalTempSensorType, LocalTempSensorValues[0]) != 0;
  }
  char *getLocalTempSensorType() { return this->LocalTempSensorType; }
  char *getMeteoIcons() { return this->MeteoIcons; }
  int getBacklightTimeout() { return atoi(this->BacklightTimeout); }
  char *getOwmApiKey() { return this->OwmApiKey; }
  char *getOwmLocationName() { return this->LocationName; }
  char *getOwmLatitude() { return this->OwmLatitude; }
  char *getOwmLongitude() { return this->OwmLongitude; }
  int getOwmRefreshInterval() { return atoi(this->OwmRefreshInterval); }
  int getAqiRefreshInterval() { return atoi(this->AqiRefreshInterval); }
  int getLocalTempSensorRefreshInterval() {
    return atoi(this->LocalTempSensorRefreshInterval);
  }

  char *getAqiStationUrl(uint8_t no);

  bool validateWebParams(iotwebconf::WebRequestWrapper *webRequestWrapper);

  void print();

private:
  CHECKBOX_PARAM(IsMetricSelected);
  CHECKBOX_PARAM(IsClock24hStyleSelected);
  CHECKBOX_PARAM(IsClockSilhouetteEnabled);
  NUMBER_PARAM(BacklightTimeout);
  SELECT_PARAM(MeteoIcons);

  GROUP_PARAM(Owm);
  TEXT_PARAM(LocationName);
  TEXT_PARAM(OwmApiKey);
  NUMBER_PARAM(OwmLatitude);
  NUMBER_PARAM(OwmLongitude);
  NUMBER_PARAM(OwmRefreshInterval);

  GROUP_PARAM(Aqi);
  TEXT_PARAM(AqiStationUrl1);
  TEXT_PARAM(AqiStationUrl2);
  TEXT_PARAM(AqiStationUrl3);
  NUMBER_PARAM(AqiRefreshInterval);

  GROUP_PARAM(Sensor);
  SELECT_PARAM(LocalTempSensorType);
  NUMBER_PARAM(LocalTempSensorRefreshInterval);

  bool validateAqiUrl(String url);
};

#endif
