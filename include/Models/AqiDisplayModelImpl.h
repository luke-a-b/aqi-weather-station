#ifndef _AQI_DISPLAY_MODEL_IMPL_H
#define _AQI_DISPLAY_MODEL_IMPL_H

#include <Arduino.h>
#include "Models/AqiDisplayModel.h"
#include "DataSources/AqiDataSource.h"
#include "PollutionLevels.h"
#include "Config.h"

class AqiDisplayModelImpl : public AqiDisplayModel
{
public:
    AqiDisplayModelImpl(Config *config) : config(config) {}
    virtual uint16_t getAqiPm25()            { return aqiData.pm25; }
    virtual uint16_t getAqiPercentPm25()     { return aqiData.pm25 * 100 / PollutionLevels::PM25_MAX; }
    virtual uint16_t getAqiPm10()            { return aqiData.pm10; }
    virtual uint16_t getAqiPercentPm10()     { return aqiData.pm10 * 100 / PollutionLevels::PM10_MAX; }
    virtual uint16_t getAqiPercentMax()      { return max(getAqiPercentPm25(), getAqiPercentPm10()); }
    virtual uint8_t getAqiHumidity()         { return aqiData.humidity; }
    virtual uint16_t getAqiPressure()        { return aqiData.pressure; }
    virtual float getAqiLevel()              { return aqiData.index; }
    virtual float getAqiTemp()               { return aqiData.temp; }
    virtual boolean isAqiDataValid()         { return !isnan(aqiData.index); }
    virtual String getAqiStaionName()        { return aqiData.stationUrl.substring(aqiData.stationUrl.indexOf("://") + 3, aqiData.stationUrl.indexOf("/data.json")); }
    AqiDataModel *getAqiDataModel()          { return &aqiData; }
    virtual boolean isMetricSelected()       { return config->isMetricSelected(); }
    virtual uint16_t getAqiRefreshInterval() { return config->getAqiRefreshInterval(); }
private:
    Config *config;
    AqiDataModel aqiData;
};

#endif