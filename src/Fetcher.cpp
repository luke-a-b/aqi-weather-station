#include "Fetcher.h"
//#include <OpenWeatherMapCurrent.h>
#include <OpenWeather.h>
#include <SunMoonCalc.h>
#include "assert.h"

Fetcher::Fetcher(Config *config, DisplayModel *displayModel,
                 ModelChangeObserver *modelChangeObserver)
    : config(config), displayModel(displayModel),
      modelChangeObserver(modelChangeObserver) {
  assert(config != nullptr);
  assert(displayModel != nullptr);
  assert(modelChangeObserver != nullptr);
}

Fetcher::~Fetcher() {
  if (localTemSensorSource != nullptr)
    delete localTemSensorSource;
}

void Fetcher::updateAll() {
  updateOwmData();
  uint8_t tryNo = 0;
  while (!updateTimeData() && tryNo < 3)
    tryNo++;
  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    updateAqiData(i);
  if (this->localTemSensorSource == nullptr &&
      config->isLocalTempSensorEnabled()) {
    this->localTemSensorSource = new LocalTempSensorSource(
        config->getLocalTempSensorType(), config->isMetricSelected());
  }
  updateSensorsData();
  uint32_t now = millis();
  this->lastAqiUpdate = now;
  this->lastWeatherUpdate = now;
  this->lastSensorUpdate = now;
  this->lastTimeUpdate = now;
}

void Fetcher::update(uint32_t now) {
  if (long(now - lastWeatherUpdate) > config->getOwmRefreshInterval() * 1000) {
    updateOwmData();
    lastWeatherUpdate = now;
  } else if (long(now - lastTimeUpdate) > TIME_UPDATE_INTERVAL * 1000) {
    updateTimeData();
    lastTimeUpdate = now;
  } else if (long(now - lastAqiUpdate) >
             config->getAqiRefreshInterval() * 1000) {
    updateAqiData(lastAqiStation);
    if (++lastAqiStation == MAX_AQI_STATIONS) {
      lastAqiStation = 0;
      lastAqiUpdate = now;
    }
  } else if (long(now - lastSensorUpdate) >
             config->getLocalTempSensorRefreshInterval() * 1000) {
    updateSensorsData();
    lastSensorUpdate = now;
  }

  this->currentAqiClient.update();
}

void Fetcher::updateAqiData(uint8_t station) {
  AqiDataModel *model = displayModel->getAqiDataModelAt(station);
  if (model->stationUrl[0] != 0) {
    modelChangeObserver->notifyAqiUpdateStart(
        displayModel->getAqiDisplayModelAt(station));
    DEBUG_PRINTF("Updating AQI: ");
    DEBUG_PRINTLN(model->stationUrl);
    currentAqiClient.updateCurrent(model);
    modelChangeObserver->notifyAqiUpdated(
        displayModel->getAqiDisplayModelAt(station));
  }
}

void Fetcher::updateOwmData() {
  DEBUG_PRINTFLN("Updating weather conditions from OpenWeather...");
  modelChangeObserver->notifyWeatherUpdateStart();
  OW_Weather *ow = new OW_Weather(); // Weather forecast library instance
  // On the ESP8266 (only) the library by default uses BearSSL, another option
  // is to use AXTLS
  // For problems with ESP8266 stability, use AXTLS by adding a false parameter
  // thus       vvvvv
  // ow.getForecast(current, hourly, daily, api_key, latitude, longitude, units,
  // language, false);
  ow->getForecast(displayModel->getCurrentWeather(), nullptr,
                  displayModel->getDailyForecasts(), config->getOwmApiKey(),
                  config->getOwmLatitude(), config->getOwmLongitude(),
                  config->isMetricSelected() ? "metric" : "imperial",
                  FPSTR(INTL_LANG_ID), false);
  // Delete to free up space and prevent fragmentation as strings change in
  // length
  delete ow;
  ow = nullptr;

  displayModel->updateWeatherIconsPaths();
  modelChangeObserver->notifyWeatherUpdated();
}

bool Fetcher::updateTimeData() {
  DEBUG_PRINTF("Updating NTP time...");
  time_t now;
  configTime(displayModel->getCurrentWeather()->timezone_offset, 0,
             NTP_SERVER_0, NTP_SERVER_1, NTP_SERVER_2);
  uint8_t timeout = 0;
  while ((now = time(nullptr)) < NTP_MIN_VALID_EPOCH && timeout < 100) {
    DEBUG_PRINTF(".");
    delay(100);
    timeout++;
  }

  DEBUG_PRINTFLN("");
  if (timeout >= 100) {
    DEBUG_PRINTFLN("Error: timeout while getting NTP time");
    return false;
  }

  DEBUG_PRINTF("Current time: ");
  DEBUG_PRINTLN(now);
  DEBUG_PRINTF("Current timezone: ");
  DEBUG_PRINTLN(displayModel->getCurrentWeather()->timezone_offset);

  DEBUG_PRINTFLN("Updating Moon data...");
  // 'now' has to be UTC, lat/lng in degrees not raadians
  SunMoonCalc *smCalc =
      new SunMoonCalc(now, displayModel->getCurrentWeather()->lat,
                      displayModel->getCurrentWeather()->lon);
  SunMoonCalc::Moon moonData = smCalc->calculateSunAndMoonData().moon;
  MoonData *moon = displayModel->getMoonData();
  moon->age = moonData.age;
  moon->phase = moonData.phase;
  moon->rise = moonData.rise;
  moon->set = moonData.set;
  delete smCalc;
  smCalc = nullptr;

  modelChangeObserver->notifyAstronomyUpdated();

  return true;
}

void Fetcher::updateSensorsData() {
  if (localTemSensorSource != nullptr) {
    if (localTemSensorSource->updateCurrent(
            displayModel->getLocalTempSensor())) {
      modelChangeObserver->notifyLocalTempUpdated();
    }
  }
}
