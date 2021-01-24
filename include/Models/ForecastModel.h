#ifndef _FORECAST_MODEL_H
#define _FORECAST_MODEL_H

#include <Arduino.h>

class ForecastModel {
public:
  virtual boolean isMetricSelected() = 0;
  virtual String getForecastIconFileName(uint8_t dayIndex) = 0;
  virtual float getForecastTemp(uint8_t dayIndex) = 0;
  virtual struct tm *getForecastTime(uint8_t dayIndex) = 0;
  virtual String getForecastDescription(uint8_t dayIndex) = 0;
  virtual float getForecastTempMin(uint8_t dayIndex) = 0;
  virtual float getForecastTempMax(uint8_t dayIndex) = 0;
  virtual float getForecastHumidity(uint8_t dayIndex) = 0;
  virtual float getForecastPressure(uint8_t dayIndex) = 0;
  virtual float getForecastWindSpeed(uint8_t dayIndex) = 0;
  virtual String getForecastWindDirection(uint8_t dayIndex) = 0;
};

#endif
