#include "Fetcher.h"
#include <OpenWeatherMapCurrent.h>
#include "assert.h"


Fetcher::Fetcher(Config *config, DisplayModel *displayModel, ModelChangeObserver* modelChangeObserver) : config(config), displayModel(displayModel), modelChangeObserver(modelChangeObserver)
{
    assert(config != nullptr);
    assert(displayModel != nullptr);
    assert(modelChangeObserver != nullptr);
    if (config->isLocalTempSensorEnabled())
    {
        this->localTemSensorSource = new LocalTempSensorSource(config->getLocalTempSensorType(), config->isMetricSelected());
    }
}

Fetcher::~Fetcher()
{
    if (localTemSensorSource != nullptr) delete localTemSensorSource;
}

void Fetcher::begin()
{
    this->lastAqiUpdate = -(config->getAqiRefreshInterval() * 1000);
    this->lastWeatherUpdate = -(config->getOwmRefreshInterval() * 1000);
    this->lastSensorUpdate = -(config->getLocalTempSensorRefreshInterval() * 1000);
    this->lastTimeUpdate = -(TIME_UPDATE_INTERVAL * 1000);
}

void Fetcher::update(uint32_t now)
{
    if (long(now - lastWeatherUpdate) > config->getOwmRefreshInterval() * 1000)
    {
        updateOwmData();
        lastWeatherUpdate = now;
    }
    if (long(now - lastTimeUpdate) > TIME_UPDATE_INTERVAL * 1000)
    {
        updateTimeData();
        lastTimeUpdate = now;
    }
    if (long(now - lastAqiUpdate) > config->getAqiRefreshInterval() * 1000)
    {
        updateAqiData();
        lastAqiUpdate = now;
    }
    if (long(now - lastSensorUpdate) > config->getLocalTempSensorRefreshInterval() * 1000)
    {
        updateSensorsData();
        lastSensorUpdate = now;
    }

    this->currentAqiClient.update();
}

void Fetcher::updateAqiData()
{
    Serial.println(F("Updating air quality conditions..."));
    for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    {
        AqiDataModel *model = displayModel->getAqiDataModelAt(i);
        if (model->stationUrl != "")
        {
            currentAqiClient.updateCurrent(model);
        }
    }
}

void Fetcher::updateOwmData()
{
    Serial.println(F("Updating weather conditions..."));
    OpenWeatherMapCurrent *currentWeatherClient = new OpenWeatherMapCurrent();
    currentWeatherClient->setMetric(config->isMetricSelected());
    currentWeatherClient->setLanguage(config->getLanguage());
    currentWeatherClient->updateCurrentById(displayModel->getCurrentWeather(), config->getOwmApiKey(), config->getOwmLocationId());
    delete currentWeatherClient;
    currentWeatherClient = nullptr;

    Serial.println(F("Updating weather forecasts..."));
    OpenWeatherMapForecast *forecastClient = new OpenWeatherMapForecast();
    forecastClient->setMetric(config->isMetricSelected());
    forecastClient->setLanguage(config->getLanguage());
    uint8_t allowedHours[] = {12, 0};
    forecastClient->setAllowedHours(allowedHours, sizeof(allowedHours));
    forecastClient->updateForecastsById(displayModel->getForecasts(), config->getOwmApiKey(), config->getOwmLocationId(), MAX_FORECASTS);
    delete forecastClient;
    forecastClient = nullptr;
}

void Fetcher::updateTimeData()
{
    // TODO: add timezone handling
    //Serial.printf("Updating NTP time... timezone: %d\n", displayModel->getCurrentWeather()->timezone);
    Serial.print(F("Updating NTP time..."));
    time_t now;
    String NtpServer0 = config->getNtpServer(0) != "" ? config->getNtpServer(0) : "0.pool.ntp.org";
    String NtpServer1 = config->getNtpServer(1) != "" ? config->getNtpServer(1) : NtpServer0;
    String NtpServer2 = config->getNtpServer(2) != "" ? config->getNtpServer(2) : NtpServer0;
    //configTime(displayModel->getCurrentWeather()->timezone, 0, NtpServer0.c_str(), NtpServer1.c_str(), NtpServer2.c_str());
    configTime(3600, 0, NtpServer0.c_str(), NtpServer1.c_str(), NtpServer2.c_str());
    while ((now = time(nullptr)) < NTP_MIN_VALID_EPOCH)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print(F("Current time: ")); Serial.println(now);

    Serial.println(F("Updating Moon data..."));
    SunMoonCalc::Moon *moonData = displayModel->getMoonData();
    // 'now' has to be UTC, lat/lng in degrees not raadians
    SunMoonCalc *smCalc = new SunMoonCalc(now, displayModel->getCurrentWeather()->lat, displayModel->getCurrentWeather()->lon);
    *moonData = smCalc->calculateSunAndMoonData().moon;
    delete smCalc;
    smCalc = nullptr;
}

void Fetcher::updateSensorsData()
{
    if (localTemSensorSource != nullptr)
    {
        Serial.println(F("Updating local temp sensor data..."));
        modelChangeObserver->notifyLocalTempUpdated(localTemSensorSource->updateCurrent(displayModel->getLocalTempSensor()));
    }
}
