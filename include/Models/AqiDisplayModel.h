#ifndef _AQI_DISPLAY_MODEL_H
#define _AQI_DISPLAY_MODEL_H

#include <Arduino.h>

class AqiDisplayModel
{
public:
    virtual uint16_t getAqiPm25() = 0;
    virtual uint16_t getAqiPercentPm25() = 0;
    virtual uint16_t getAqiPm10() = 0;
    virtual uint16_t getAqiPercentPm10() = 0;
    virtual uint16_t getAqiPercentMax() = 0;
    virtual uint8_t getAqiHumidity() = 0;
    virtual uint16_t getAqiPressure() = 0;
    virtual float getAqiLevel() = 0;
    virtual float getAqiTemp() = 0;
    virtual bool isAqiDataValid() = 0;
    virtual String getAqiStaionName() = 0;
    virtual boolean isMetricSelected() = 0;
    virtual uint16_t getAqiRefreshInterval() = 0;
 };

#endif
