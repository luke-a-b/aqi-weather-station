#ifndef _DISPLAY_MODEL_H
#define _DISPLAY_MODEL_H

#include <OpenWeatherMapCurrent.h>
#include <OpenWeatherMapForecast.h>
#include <SunMoonCalc.h>
#include "Config.h"
#include "Models/AstronomyModel.h"
#include "Models/CurrentWeatherModel.h"
#include "Models/ForecastModel.h"
#include "Models/ClockModel.h"
#include "Models/AqiDisplayModelImpl.h"
#include "DataSources/LocalTempSensorSource.h"
#include "assert.h"

#define MAX_FORECASTS 10

class DisplayModel : public AstronomyModel, public CurrentWeatherModel, public ForecastModel, public ClockModel
{
public:
    DisplayModel(Config *config);
    AqiDataModel *getAqiDataModelAt(uint8_t no)       { assert(no < MAX_AQI_STATIONS); return aqiData[no].getAqiDataModel(); }
    AqiDisplayModel *getAqiDisplayModelAt(uint8_t no) { assert(no < MAX_AQI_STATIONS); return &aqiData[no]; }
    OpenWeatherMapCurrentData *getCurrentWeather()    { return &currentWeather; }
    OpenWeatherMapForecastData *getForecasts()        { return forecasts; }
    SunMoonCalc::Moon *getMoonData()                  { return &moonData; }
    LocalTempSensorDataModel *getLocalTempSensor()    { return &localTempSensor; }

public: // implement AstronomyModel
    virtual String getSunTranslation();
    virtual String getMoonTranslation();
    virtual String getSunsetTime();
    virtual String getSunriseTime();
    virtual String getMoonriseTime();
    virtual String getMoonsetTime();
    virtual String getMoonIconFileName();
    virtual String getMoonPhaseName();

public: // implement ClockModel
    virtual boolean isAqiDataValid();
    virtual uint16_t getAqiPercentMax();
    virtual String getCurrentTime();
    virtual String getCurrentDate();
    virtual boolean isClock24hSytyle();
    virtual boolean isClockSilhouetteEnabled();
    virtual String getClockPeriod();
    virtual String getAqiLevelIconFileName();
    virtual String getAqiNaLevelIconFileName();

public: // implement CurrentWeatherModel
    virtual String getCurrentWeatherIconFileName();
    virtual String getCurrentWeatherCityName();
    virtual float getCurrentWeatherTemp();
    virtual String getCurrentWeatherDescription();
    virtual uint16_t getCurrentWeatherPressure();
    virtual uint8_t getCurrentWeatherHumidity();
    virtual boolean isLocalTempSensorEnabled();
    virtual float getLocalTemp();
    virtual float getLocalHumidity();
    virtual uint32_t getObservationTime();


public: //implement ForecastModel
    virtual boolean isMetricSelected();
    virtual String getForecastIconFileName(uint8_t dayIndex);
    virtual float getForecastTemp(uint8_t dayIndex);
    virtual String getForecastWeekDayName(uint8_t dayIndex);

private:
    String getTimeString(time_t time);
    AqiDisplayModelImpl aqiData[MAX_AQI_STATIONS];
    LocalTempSensorDataModel localTempSensor;
    OpenWeatherMapCurrentData currentWeather;
    OpenWeatherMapForecastData forecasts[MAX_FORECASTS];
    SunMoonCalc::Moon moonData;
    Config *config;
};

#endif
