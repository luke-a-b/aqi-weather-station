#include <assert.h>
#include "Config.h"

const char *WEEK_DAYS_NAMES[] PROGMEM = {INTL_SUND, INTL_MOND, INTL_TUED,
                                         INTL_WEDD, INTL_THUD, INTL_FRID,
                                         INTL_SATD};
const char *WEEK_DAYS_NAMES_FULL[] PROGMEM = {
    INTL_SUNDAY, INTL_MONDAY, INTL_TUEDAY, INTL_WEDDAY,
    INTL_THUDAY, INTL_FRIDAY, INTL_SATDAY};
const char *MONTHS_NAMES[] PROGMEM = {
    INTL_JANUARY,   INTL_FEBRUARY, INTL_MARCH,    INTL_APRIL,
    INTL_MAY,       INTL_JUNE,     INTL_JULY,     INTL_AUGUST,
    INTL_SEPTEMBER, INTL_OCTOBER,  INTL_NOVEMBER, INTL_DECEMBER};

const char *MOON_PHASES[] PROGMEM = {INTL_PHASE_0, INTL_PHASE_1, INTL_PHASE_2,
                                     INTL_PHASE_3, INTL_PHASE_4, INTL_PHASE_5,
                                     INTL_PHASE_6, INTL_PHASE_7};

Config::Config() {
  CREATE_CHECKBOX_PARAM(IsMetricSelected, INTL_IS_METRIC_SYSTEM, true);
  CREATE_CHECKBOX_PARAM(IsClock24hStyleSelected, INTL_IS_CLOCK_24H_STYLE, true);
  CREATE_CHECKBOX_PARAM(IsClockSilhouetteEnabled,
                        INTL_IS_CLOCK_SILHOUETTE_ENABLED, true);
  CREATE_NUMBER_PARAM(BacklightTimeout, INTL_BACKLIGHT_TIMEOUT, "0", "0..600",
                      "min='0' max='600' step='1'");
  CREATE_SELECT_PARAM(MeteoIcons, INTL_METEO_ICONS, MeteoIconsValues,
                      MeteoIconsNames, MeteoIconsValues[0]);
  CREATE_GROUP_PARAM(Owm, INTL_OWM_SETTINGS);
  CREATE_TEXT_PARAM(LocationName, INTL_LOCATAION_NAME, nullptr,
                    INTL_LOCATAION_NAME_PLACEHOLDER);
  CREATE_TEXT_PARAM(OwmApiKey, INTL_OWM_API_KEY, nullptr,
                    INTL_OWM_API_KEY_PLACEHOLDER);
  CREATE_NUMBER_PARAM(OwmLatitude, INTL_OWM_LATITUDE, nullptr, "51.06809",
                      "step='0.00001'");
  CREATE_NUMBER_PARAM(OwmLongitude, INTL_OWM_LONGITUDE, nullptr, "16.97507",
                      "step='0.00001'");
  CREATE_NUMBER_PARAM(OwmRefreshInterval, INTL_OWM_REFRESH_INTERVAL, "240",
                      "1..600", "min='1' max='600' step='1'");

  CREATE_GROUP_PARAM(Aqi, INTL_AQI_SETTINGS);
  CREATE_TEXT_PARAM(AqiStationUrl1, INTL_AQI_URL_1, nullptr, "");
  CREATE_TEXT_PARAM(AqiStationUrl2, INTL_AQI_URL_2, nullptr, "");
  CREATE_TEXT_PARAM(AqiStationUrl3, INTL_AQI_URL_3, nullptr, "");
  CREATE_NUMBER_PARAM(AqiRefreshInterval, INTL_AQI_REFRESH_INTERVAL, "120",
                      "0..600", "min='0' max='600' step='1'");

  CREATE_GROUP_PARAM(Sensor, INTL_LOCAL_TEMP_SETTINGS);
  CREATE_SELECT_PARAM(LocalTempSensorType, INTL_LOCAL_TEMP_SENSOR_TYPE,
                      LocalTempSensorValues, LocalTempSensorNames, nullptr);
  CREATE_NUMBER_PARAM(LocalTempSensorRefreshInterval,
                      INTL_LOCAL_TEMP_REFRESH_INTERVAL, "60", "0..600",
                      "min='0' max='600' step='1'");
}

void Config::addCustomWebParams(IotWebConf *iotWebConf) {
  iotWebConf->addSystemParameter(this->IsMetricSelectedParam);
  iotWebConf->addSystemParameter(this->IsClock24hStyleSelectedParam);
  iotWebConf->addSystemParameter(this->IsClockSilhouetteEnabledParam);
  iotWebConf->addSystemParameter(this->BacklightTimeoutParam);
  iotWebConf->addSystemParameter(this->MeteoIconsParam);

  this->groupOwm->addItem(this->LocationNameParam);
  this->groupOwm->addItem(this->OwmApiKeyParam);
  this->groupOwm->addItem(this->OwmLatitudeParam);
  this->groupOwm->addItem(this->OwmLongitudeParam);
  this->groupOwm->addItem(this->OwmRefreshIntervalParam);
  iotWebConf->addParameterGroup(this->groupOwm);

  this->groupAqi->addItem(this->AqiStationUrl1Param);
  this->groupAqi->addItem(this->AqiStationUrl2Param);
  this->groupAqi->addItem(this->AqiStationUrl3Param);
  this->groupAqi->addItem(this->AqiRefreshIntervalParam);
  iotWebConf->addParameterGroup(this->groupAqi);

  this->groupSensor->addItem(this->LocalTempSensorTypeParam);
  this->groupSensor->addItem(this->LocalTempSensorRefreshIntervalParam);
  iotWebConf->addParameterGroup(this->groupSensor);
}

bool Config::validateAqiUrl(String url) {
  return url.length() == 0 || // accepting empty url
         (url.startsWith(F("http://")) || url.startsWith(F("https://"))) &&
             url.endsWith(F("/data.json"));
}

bool Config::validateWebParams(
    iotwebconf::WebRequestWrapper *webRequestWrapper) {
  DEBUG_PRINTFLN("Validating form.");
  bool valid = true;
  String error;

  if (webRequestWrapper->arg(this->OwmApiKeyParam->getId()).length() == 0) {
    error = FPSTR(INTL_OWM_API_KEY_ERROR);
    this->OwmApiKeyParam->errorMessage = error.c_str();
    valid = false;
  }

  if (webRequestWrapper->arg(this->LocationNameParam->getId()).length() == 0) {
    error = FPSTR(INTL_LOCATION_ERROR);
    this->LocationNameParam->errorMessage = error.c_str();
    valid = false;
  }

  error = FPSTR(INTL_AQI_URL_ERROR);
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl1Param->getId()))) {
    this->AqiStationUrl1Param->errorMessage = error.c_str();
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl2Param->getId()))) {
    this->AqiStationUrl2Param->errorMessage = error.c_str();
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl3Param->getId()))) {
    this->AqiStationUrl3Param->errorMessage = error.c_str();
    valid = false;
  }

  return valid;
}

char *Config::getAqiStationUrl(uint8_t no) {
  assert(no < MAX_AQI_STATIONS);
  char *to_return;
  switch (no) {
  default:
  case 0:
    to_return = this->AqiStationUrl1;
    break;
  case 1:
    to_return = this->AqiStationUrl2;
    break;
  case 2:
    to_return = this->AqiStationUrl3;
  }
  return to_return;
}

void Config::print() {
  DEBUG_PRINTFLN("============== Config =================");
  DEBUG_PRINTF("Is metric?: ");
  DEBUG_PRINTLN(this->IsMetricSelected);

  DEBUG_PRINTF("Open Weather API Key: ");
  DEBUG_PRINTLN(this->OwmApiKey);
  DEBUG_PRINTF("Location Name: ");
  DEBUG_PRINTLN(this->LocationName);

  DEBUG_PRINTFLN("=======================================");
}
