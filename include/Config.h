#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>

#define CONFIG_VERSION "0.1"

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

#define MAX_LOGOS 5        // number of bitmaps in logos data folder
#define TIME_LOGO_UPDATE 3 // update every 3 seconds

#define NTP_MIN_VALID_EPOCH 1609459200 // January 1st, 2021

#define TIME_UPDATE_INTERVAL 3600L // 60*60 -> update every hour

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

#define CHECKBOX_PARAM_LEN 16
#define CHECKBOX_PARAM(name, label, default)                                   \
  char name[CHECKBOX_PARAM_LEN] = {0};                                         \
  IotWebConfCheckboxParameter *name##Param
#define CREATE_CHECKBOX_PARAM(name, label, default)                            \
  name##Param = new IotWebConfCheckboxParameter(label, #name, name,            \
                                                CHECKBOX_PARAM_LEN, default)

#define NUMBER_PARAM_LEN 16
#define NUMBER_PARAM(name, label, default, placeholder, custom)                \
  char name[NUMBER_PARAM_LEN] = {0};                                           \
  IotWebConfNumberParameter *name##Param
#define CREATE_NUMBER_PARAM(name, label, default, placeholder, custom)         \
  name##Param = new IotWebConfNumberParameter(                                 \
      label, #name, name, NUMBER_PARAM_LEN, default, placeholder, custom)

#define TEXT_PARAM_LEN 128
#define TEXT_PARAM(name, label, default, placeholder)                          \
  char name[TEXT_PARAM_LEN] = {0};                                             \
  IotWebConfTextParameter *name##Param
#define CREATE_TEXT_PARAM(name, label, default, placeholder)                   \
  name##Param = new IotWebConfTextParameter(                                   \
      label, #name, name, TEXT_PARAM_LEN, default, placeholder)

#define SELECT_PARAM_LEN 32
#define SELECT_PARAM(name, label, values, names, default)                      \
  char name[SELECT_PARAM_LEN] = {0};                                           \
  IotWebConfSelectParameter *name##Param
#define CREATE_SELECT_PARAM(name, label, values, names, default)               \
  name##Param = new IotWebConfSelectParameter(                                 \
      label, #name, name, SELECT_PARAM_LEN, (char *)values, (char *)names,     \
      sizeof(values) / SELECT_PARAM_LEN, SELECT_PARAM_LEN, default)

#define GROUP_PARAM(name, label) IotWebConfParameterGroup *group##name
#define CREATE_GROUP_PARAM(name, label)                                        \
  group##name = new IotWebConfParameterGroup(label)

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

  Config() {
    CREATE_CHECKBOX_PARAM(IsMetricSelected, INTL_IS_METRIC_SYSTEM, true);
    CREATE_CHECKBOX_PARAM(IsClock24hStyleSelected, INTL_IS_CLOCK_24H_STYLE,
                          true);
    CREATE_CHECKBOX_PARAM(IsClockSilhouetteEnabled,
                          INTL_IS_CLOCK_SILHOUETTE_ENABLED, true);
    CREATE_NUMBER_PARAM(BacklightTimeout, INTL_BACKLIGHT_TIMEOUT, "0", "0..600",
                        "min='0' max='600' step='1'");
    CREATE_SELECT_PARAM(MeteoIcons, INTL_METEO_ICONS, MeteoIconsValues,
                        MeteoIconsNames, MeteoIconsValues[0]);

    CREATE_GROUP_PARAM(Owm, INTL_OWM_SETTINGS);
    CREATE_TEXT_PARAM(LocationName, INTL_LOCATAION_NAME, nullptr, "");
    CREATE_TEXT_PARAM(OwmApiKey, INTL_OWM_API_KEY, nullptr, "");
    CREATE_NUMBER_PARAM(OwmLatitude, INTL_OWM_LATITUDE, nullptr, "51.06809",
                        "step='0.00001'");
    CREATE_NUMBER_PARAM(OwmLongitude, INTL_OWM_LONGITUDE, nullptr, "16.97507",
                        "step='0.00001'");
    CREATE_NUMBER_PARAM(OwmRefreshInterval, INTL_OWM_REFRESH_INTERVAL, "1800",
                        "10..3600", "min='10' max='3600' step='10'");

    CREATE_GROUP_PARAM(Aqi, INTL_AQI_SETTINGS);
    CREATE_TEXT_PARAM(AqiStationUrl1, INTL_AQI_URL, nullptr, "");
    CREATE_TEXT_PARAM(AqiStationUrl2, INTL_AQI_URL, nullptr, "");
    CREATE_TEXT_PARAM(AqiStationUrl3, INTL_AQI_URL, nullptr, "");
    CREATE_NUMBER_PARAM(AqiRefreshInterval, INTL_AQI_REFRESH_INTERVAL, "120",
                        "10..3600", "min='10' max='3600' step='10'");

    CREATE_GROUP_PARAM(Sensor, INTL_LOCAL_TEMP_SETTINGS);
    CREATE_SELECT_PARAM(LocalTempSensorType, INTL_LOCAL_TEMP_SENSOR_TYPE,
                        LocalTempSensorValues, LocalTempSensorNames, nullptr);
    CREATE_NUMBER_PARAM(LocalTempSensorRefreshInterval,
                        INTL_LOCAL_TEMP_REFRESH_INTERVAL, "60", "10..3600",
                        "min='10' max='3600' step='10'");
  }

private:
  CHECKBOX_PARAM(IsMetricSelected, INTL_IS_METRIC_SYSTEM, true);
  CHECKBOX_PARAM(IsClock24hStyleSelected, INTL_IS_CLOCK_24H_STYLE, true);
  CHECKBOX_PARAM(IsClockSilhouetteEnabled, INTL_IS_CLOCK_SILHOUETTE_ENABLED,
                 true);

  NUMBER_PARAM(BacklightTimeout, INTL_BACKLIGHT_TIMEOUT, "0", "0..600",
               "min='0' max='600' step='1'");
  SELECT_PARAM(MeteoIcons, INTL_METEO_ICONS, MeteoIconsValues, MeteoIconsNames,
               MeteoIconsValues[0]);

  GROUP_PARAM(Owm, INTL_OWM_SETTINGS);
  TEXT_PARAM(LocationName, INTL_LOCATAION_NAME, nullptr, "");
  TEXT_PARAM(OwmApiKey, INTL_OWM_API_KEY, nullptr, "");
  NUMBER_PARAM(OwmLatitude, INTL_OWM_LATITUDE, nullptr, "51.06809",
               "step='0.00001'");
  NUMBER_PARAM(OwmLongitude, INTL_OWM_LONGITUDE, nullptr, "16.97507",
               "step='0.00001'");
  NUMBER_PARAM(OwmRefreshInterval, INTL_OWM_REFRESH_INTERVAL, "1800",
               "10..3600", "min='10' max='3600' step='10'");

  GROUP_PARAM(Aqi, INTL_AQI_SETTINGS);
  TEXT_PARAM(AqiStationUrl1, INTL_AQI_URL, nullptr, "");
  TEXT_PARAM(AqiStationUrl2, INTL_AQI_URL, nullptr, "");
  TEXT_PARAM(AqiStationUrl3, INTL_AQI_URL, nullptr, "");
  NUMBER_PARAM(AqiRefreshInterval, INTL_AQI_REFRESH_INTERVAL, "120", "10..3600",
               "min='10' max='3600' step='10'");

  GROUP_PARAM(Sensor, INTL_LOCAL_TEMP_SETTINGS);
  SELECT_PARAM(LocalTempSensorType, INTL_LOCAL_TEMP_SENSOR_TYPE,
               LocalTempSensorValues, LocalTempSensorNames, nullptr);
  NUMBER_PARAM(LocalTempSensorRefreshInterval, INTL_LOCAL_TEMP_REFRESH_INTERVAL,
               "60", "10..3600", "min='10' max='3600' step='10'");

  bool validateAqiUrl(String url);
};

#endif
