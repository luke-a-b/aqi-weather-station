#ifndef _DISPLAY_MODEL_H
#define _DISPLAY_MODEL_H

#include "Config.h"
#include "Models/AstronomyModel.h"
#include "Models/CurrentWeatherModel.h"
#include "Models/ForecastModel.h"
#include "Models/ClockModel.h"
#include "Models/AqiDisplayModelImpl.h"
#include "DataSources/LocalTempSensorSource.h"
#include "assert.h"
#include <OpenWeather.h>
#include <SunMoonCalc.h>

class DisplayModel : public AstronomyModel,
                     public CurrentWeatherModel,
                     public ForecastModel,
                     public ClockModel {
public:
  DisplayModel(Config *config);
  AqiDataModel *getAqiDataModelAt(uint8_t no) {
    assert(no < MAX_AQI_STATIONS);
    return aqiData[no].getAqiDataModel();
  }
  AqiDisplayModel *getAqiDisplayModelAt(uint8_t no) {
    assert(no < MAX_AQI_STATIONS);
    return &aqiData[no];
  }
  OW_current *getCurrentWeather() { return &currentWeather; }
  OW_daily *getDailyForecasts() { return &dailyForecasts; }
  SunMoonCalc::Moon *getMoonData() { return &moonData; }
  LocalTempSensorDataModel *getLocalTempSensor() { return &localTempSensor; }
  void updateWeatherIconsPaths();

public: // implement AstronomyModel
  virtual struct tm *getSunsetTime();
  virtual struct tm *getSunriseTime();
  virtual struct tm *getMoonriseTime();
  virtual struct tm *getMoonsetTime();
  virtual String getMoonIconFileName();
  virtual uint8_t getMoonPhase();

public: // implement ClockModel
  virtual boolean isAqiDataValid();
  virtual uint16_t getAqiPercentMax();
  virtual boolean isClock24hSytyle();
  virtual boolean isClockSilhouetteEnabled();
  virtual String getAqiLevelIconFileName();

public: // implement CurrentWeatherModel
  virtual String getCurrentWeatherIconFileName();
  virtual String getCurrentWeatherCityName();
  virtual float getCurrentWeatherTemp();
  virtual float getTodayMinTemp();
  virtual float getTodayMaxTemp();
  virtual float getCurrentFeelsLikeTemp();
  virtual uint8_t getCurrentClouds();
  virtual float getCurrentWindSpeed();
  virtual float getCurrentWindGust();
  virtual uint16_t getCurrentWindDeg();
  virtual float getCurrentRain();
  virtual float getCurrentSnow();
  virtual float getCurrentUvi();
  virtual uint32_t getCurrentVisiblity();
  virtual float getCurrentDewPoint();
  virtual String getCurrentWeatherDescription();
  virtual uint16_t getCurrentWeatherPressure();
  virtual uint8_t getCurrentWeatherHumidity();
  virtual boolean isLocalTempSensorEnabled();
  virtual float getLocalTemp();
  virtual float getLocalHumidity();
  virtual uint32_t getObservationTime();
  virtual String getCurrentAirQuality();

public: // implement ForecastModel
  virtual boolean isMetricSelected();
  virtual String getForecastIconFileName(uint8_t dayIndex);
  virtual float getForecastTemp(uint8_t dayIndex);
  virtual struct tm *getForecastTime(uint8_t dayIndex);
  virtual String getForecastDescription(uint8_t dayIndex);
  virtual float getForecastTempMin(uint8_t dayIndex);
  virtual float getForecastTempMax(uint8_t dayIndex);
  virtual float getForecastHumidity(uint8_t dayIndex);
  virtual float getForecastPressure(uint8_t dayIndex);
  virtual float getForecastWindSpeed(uint8_t dayIndex);
  virtual String getForecastWindDirection(uint8_t dayIndex);

private:
  Config *config;
  AqiDisplayModelImpl aqiData[MAX_AQI_STATIONS];
  LocalTempSensorDataModel localTempSensor;
  OW_current currentWeather;
  OW_daily dailyForecasts;

  SunMoonCalc::Moon moonData;

  struct tm *getLocalTime(time_t time);
  String getWeatherIconFileName(String path, String icon);
};

#endif
