#ifndef _FORECAST_MODEL_H
#define _FORECAST_MODEL_H

#include <Arduino.h>

class ForecastModel
{
public:
    virtual boolean isMetricSelected() = 0;
    virtual String getForecastIconFileName(uint8_t dayIndex) = 0;
    virtual float getForecastTemp(uint8_t dayIndex) = 0;
    virtual String getForecastWeekDayName(uint8_t dayIndex) = 0;
};

#endif
