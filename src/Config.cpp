#include <assert.h>
#include "Config.h"

const char *NTP_SERVERS[] PROGMEM = {NTP_SERVER_0, NTP_SERVER_1, NTP_SERVER_2};

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
  iotWebConf->addSystemParameter(&params.IsMetricSelectedParam);
  iotWebConf->addSystemParameter(&params.IsClock24hStyleSelectedParam);
  iotWebConf->addSystemParameter(&params.IsClockSilhouetteEnabledParam);
  iotWebConf->addSystemParameter(&params.BacklightTimeoutParam);
  iotWebConf->addSystemParameter(&params.MeteoIconsParam);

  params.groupOwm.addItem(&params.LocationNameParam);
  params.groupOwm.addItem(&params.OwmApiKeyParam);
  params.groupOwm.addItem(&params.OwmLatitudeParam);
  params.groupOwm.addItem(&params.OwmLongitudeParam);
  params.groupOwm.addItem(&params.OwmRefreshIntervalParam);
  iotWebConf->addParameterGroup(&params.groupOwm);

  params.groupAqi.addItem(&params.AqiStationUrl1Param);
  params.groupAqi.addItem(&params.AqiStationUrl2Param);
  params.groupAqi.addItem(&params.AqiStationUrl3Param);
  params.groupAqi.addItem(&params.AqiRefreshIntervalParam);
  iotWebConf->addParameterGroup(&params.groupAqi);

  params.groupSensor.addItem(&params.LocalTempSensorTypeParam);
  params.groupSensor.addItem(&params.LocalTempSensorRefreshIntervalParam);
  iotWebConf->addParameterGroup(&params.groupSensor);
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

  if (webRequestWrapper->arg(params.OwmApiKeyParam.getId()).length() == 0) {
    error = FPSTR(INTL_OWM_API_KEY_ERROR);
    params.OwmApiKeyParam.errorMessage = error.c_str();
    valid = false;
  }

  if (webRequestWrapper->arg(params.LocationNameParam.getId()).length() == 0) {
    error = FPSTR(INTL_LOCATION_ERROR);
    params.LocationNameParam.errorMessage = error.c_str();
    valid = false;
  }

  error = FPSTR(INTL_AQI_URL_ERROR);
  if (!validateAqiUrl(
          webRequestWrapper->arg(params.AqiStationUrl1Param.getId()))) {
    params.AqiStationUrl1Param.errorMessage = error.c_str();
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(params.AqiStationUrl2Param.getId()))) {
    params.AqiStationUrl2Param.errorMessage = error.c_str();
    valid = false;
  }
  if (!validateAqiUrl(
          webRequestWrapper->arg(params.AqiStationUrl3Param.getId()))) {
    params.AqiStationUrl3Param.errorMessage = error.c_str();
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
    to_return = params.AqiStationUrl1;
    break;
  case 1:
    to_return = params.AqiStationUrl2;
    break;
  case 2:
    to_return = params.AqiStationUrl3;
  }
  return to_return;
}

String Config::getNtpServer(uint8_t serverNo) {
  assert(serverNo < 3);
  return FPSTR(NTP_SERVERS[serverNo]);
}

void Config::print() {
  DEBUG_PRINTFLN("============== Config =================");
  DEBUG_PRINTF("Is metric?: ");
  DEBUG_PRINTLN(params.IsMetricSelected);

  DEBUG_PRINTF("Open Weather API Key: ");
  DEBUG_PRINTLN(params.OwmApiKey);
  DEBUG_PRINTF("Location Name: ");
  DEBUG_PRINTLN(params.LocationName);

  DEBUG_PRINTF("NTP server 0: ");
  DEBUG_PRINTLN(getNtpServer(0).c_str());
  DEBUG_PRINTF("NTP server 1: ");
  DEBUG_PRINTLN(getNtpServer(1).c_str());
  DEBUG_PRINTF("NTP server 2: ");
  DEBUG_PRINTLN(getNtpServer(2).c_str());
  DEBUG_PRINTFLN("=======================================");
}
