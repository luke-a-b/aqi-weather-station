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
  // String error;

  if (webRequestWrapper->arg(this->OwmApiKeyParam->getId()).length() == 0) {
    // error = FPSTR(INTL_OWM_API_KEY_ERROR);
    this->OwmApiKeyParam->errorMessage = INTL_OWM_API_KEY_ERROR;
    valid = false;
  }

  if (webRequestWrapper->arg(this->LocationNameParam->getId()).length() == 0) {
    // error = FPSTR(INTL_LOCATION_ERROR);
    this->LocationNameParam->errorMessage = INTL_LOCATION_ERROR;
    valid = false;
  }

  // error = FPSTR(INTL_AQI_URL_ERROR);
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl1Param->getId()))) {
    this->AqiStationUrl1Param->errorMessage = INTL_AQI_URL_ERROR;
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl2Param->getId()))) {
    this->AqiStationUrl2Param->errorMessage = INTL_AQI_URL_ERROR;
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(this->AqiStationUrl3Param->getId()))) {
    this->AqiStationUrl3Param->errorMessage = INTL_AQI_URL_ERROR;
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
