#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h>

#define CONFIG_VERSION "0.1"

#define MAX_LOGOS 5
#define TIME_LOGO_UPDATE 3 // update every 3 seconds

#define NTP_MIN_VALID_EPOCH 1533081600 // August 1st, 2018

#define MAX_AQI_STATIONS 3

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

const char NTP_SERVER_0[] PROGMEM = "0.pool.ntp.org";
const char NTP_SERVER_1[] PROGMEM = "1.pool.ntp.org";
const char NTP_SERVER_2[] PROGMEM = "2.pool.ntp.org";

// translations
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
  String name##Label = label;                                                  \
  IotWebConfCheckboxParameter name##Param = {name##Label.c_str(), #name, name, \
                                             CHECKBOX_PARAM_LEN, default};

#define NUMBER_PARAM_LEN 16
#define NUMBER_PARAM(name, label, default, placeholder, custom)                \
  char name[NUMBER_PARAM_LEN] = {0};                                           \
  String name##Label = label;                                                  \
  IotWebConfNumberParameter name##Param = {                                    \
      name##Label.c_str(), #name, name, NUMBER_PARAM_LEN, default,             \
      placeholder,         custom};

#define TEXT_PARAM_LEN 128
#define TEXT_PARAM(name, label, default, placeholder)                          \
  char name[TEXT_PARAM_LEN] = {0};                                             \
  String name##Label = label;                                                  \
  String name##Placeholder = placeholder;                                      \
  IotWebConfTextParameter name##Param = {                                      \
      name##Label.c_str(), #name,   name,                                      \
      TEXT_PARAM_LEN,      default, name##Placeholder.c_str()};

#define SELECT_PARAM_LEN 32
#define SELECT_PARAM(name, label, values, names, default)                      \
  char name[SELECT_PARAM_LEN] = {0};                                           \
  String name##Label = label;                                                  \
  IotWebConfSelectParameter name##Param = {name##Label.c_str(),                \
                                           #name,                              \
                                           name,                               \
                                           SELECT_PARAM_LEN,                   \
                                           (char *)values,                     \
                                           (char *)names,                      \
                                           sizeof(values) / SELECT_PARAM_LEN,  \
                                           SELECT_PARAM_LEN,                   \
                                           default};

#define GROUP_PARAM(name, label)                                               \
  String name##Label = label;                                                  \
  IotWebConfParameterGroup group##name = {name##Label.c_str()};

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

typedef struct {
  CHECKBOX_PARAM(IsMetricSelected, FPSTR(INTL_IS_METRIC_SYSTEM), true)
  CHECKBOX_PARAM(IsClock24hStyleSelected, FPSTR(INTL_IS_CLOCK_24H_STYLE), true)
  CHECKBOX_PARAM(IsClockSilhouetteEnabled,
                 FPSTR(INTL_IS_CLOCK_SILHOUETTE_ENABLED), true)
  NUMBER_PARAM(BacklightTimeout, FPSTR(INTL_BACKLIGHT_TIMEOUT), "0", "0..600",
               "min='0' max='600' step='1'")
  SELECT_PARAM(MeteoIcons, FPSTR(INTL_METEO_ICONS), MeteoIconsValues,
               MeteoIconsNames, MeteoIconsValues[0])

  GROUP_PARAM(Owm, FPSTR(INTL_OWM_SETTINGS))
  TEXT_PARAM(LocationName, FPSTR(INTL_LOCATAION_NAME), nullptr,
             FPSTR(INTL_LOCATAION_NAME_PLACEHOLDER))
  TEXT_PARAM(OwmApiKey, FPSTR(INTL_OWM_API_KEY), nullptr,
             FPSTR(INTL_OWM_API_KEY_PLACEHOLDER))
  NUMBER_PARAM(OwmLatitude, FPSTR(INTL_OWM_LATITUDE), nullptr, "51.06809",
               "step='0.00001'")
  NUMBER_PARAM(OwmLongitude, FPSTR(INTL_OWM_LONGITUDE), nullptr, "16.97507",
               "step='0.00001'")
  NUMBER_PARAM(OwmRefreshInterval, FPSTR(INTL_OWM_REFRESH_INTERVAL), "240",
               "1..600", "min='1' max='600' step='1'")

  GROUP_PARAM(Aqi, FPSTR(INTL_AQI_SETTINGS))
  TEXT_PARAM(AqiStationUrl1, String(FPSTR(INTL_AQI_URL)) + "1", nullptr, "")
  TEXT_PARAM(AqiStationUrl2, String(FPSTR(INTL_AQI_URL)) + "2", nullptr, "")
  TEXT_PARAM(AqiStationUrl3, String(FPSTR(INTL_AQI_URL)) + "3", nullptr, "")
  NUMBER_PARAM(AqiRefreshInterval, FPSTR(INTL_AQI_REFRESH_INTERVAL), "120",
               "0..600", "min='0' max='600' step='1'")

  GROUP_PARAM(Sensor, FPSTR(INTL_LOCAL_TEMP_SETTINGS))
  SELECT_PARAM(LocalTempSensorType, FPSTR(INTL_LOCAL_TEMP_SENSOR_TYPE),
               LocalTempSensorValues, LocalTempSensorNames, nullptr)
  NUMBER_PARAM(LocalTempSensorRefreshInterval,
               FPSTR(INTL_LOCAL_TEMP_REFRESH_INTERVAL), "60", "0..600",
               "min='0' max='600' step='1'")

} Params;

class Config {
public:
  Config() {}

  void addCustomWebParams(IotWebConf *iotWebConf);

  boolean isMetricSelected() {
    return params.IsMetricSelectedParam.isChecked();
  }
  boolean isClock24hStyleSelected() {
    return params.IsClock24hStyleSelectedParam.isChecked();
  }
  boolean isClockSilhouetteEnabled() {
    return params.IsClockSilhouetteEnabledParam.isChecked();
  }
  boolean isLocalTempSensorEnabled() {
    return strcmp(params.LocalTempSensorType, LocalTempSensorValues[0]) != 0;
  }
  char *getLocalTempSensorType() { return params.LocalTempSensorType; }
  char *getMeteoIcons() { return params.MeteoIcons; }
  int getBacklightTimeout() { return atoi(params.BacklightTimeout); }
  char *getOwmApiKey() { return params.OwmApiKey; }
  char *getOwmLocationName() { return params.LocationName; }
  char *getOwmLatitude() { return params.OwmLatitude; }
  char *getOwmLongitude() { return params.OwmLongitude; }
  int getOwmRefreshInterval() { return atoi(params.OwmRefreshInterval); }
  int getAqiRefreshInterval() { return atoi(params.AqiRefreshInterval); }
  int getLocalTempSensorRefreshInterval() {
    return atoi(params.LocalTempSensorRefreshInterval);
  }

  String getNtpServer(uint8_t serverNo);
  char *getAqiStationUrl(uint8_t no);

  bool validateWebParams(iotwebconf::WebRequestWrapper *webRequestWrapper);

  void print();

private:
  Params params;

  bool validateAqiUrl(String url);
};

#endif
