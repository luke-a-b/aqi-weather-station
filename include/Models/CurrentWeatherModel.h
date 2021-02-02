#ifndef _CURRENT_WEATHER_MODEL_H
#define _CURRENT_WEATHER_MODEL_H

#include <Arduino.h>

class CurrentWeatherModel {
public:
  virtual boolean isMetricSelected() = 0;
  virtual String getCurrentWeatherIconFileName() = 0;
  virtual char *getCurrentWeatherCityName() = 0;
  virtual float getCurrentWeatherTemp() = 0;
  virtual float getTodayMinTemp() = 0;
  virtual float getTodayMaxTemp() = 0;
  virtual float getCurrentFeelsLikeTemp() = 0;
  virtual uint8_t getCurrentClouds() = 0;
  virtual float getCurrentWindSpeed() = 0;
  virtual float getCurrentWindGust() = 0;
  virtual uint16_t getCurrentWindDeg() = 0;
  virtual float getCurrentRain() = 0;
  virtual float getCurrentSnow() = 0;
  virtual float getCurrentUvi() = 0;
  virtual uint32_t getCurrentVisiblity() = 0;
  virtual float getCurrentDewPoint() = 0;
  virtual String getCurrentWeatherDescription() = 0;
  virtual uint16_t getCurrentWeatherPressure() = 0;
  virtual uint8_t getCurrentWeatherHumidity() = 0;
  virtual boolean isLocalTempSensorEnabled() = 0;
  virtual float getLocalTemp() = 0;
  virtual float getLocalHumidity() = 0;
  virtual uint32_t getObservationTime() = 0;
  virtual String getCurrentAirQuality() = 0;
};

#endif
