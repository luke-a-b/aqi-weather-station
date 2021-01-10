#ifndef _CLOCK_MODEL_H
#define _CLOCK_MODEL_H

#include <Arduino.h>

class ClockModel
{
public:
    virtual boolean isAqiDataValid() = 0;
    virtual uint16_t getAqiPercentMax() = 0;
    virtual String getCurrentTime() = 0;
    virtual String getCurrentDate() = 0;
    virtual boolean isClock24hSytyle() = 0;
    virtual boolean isClockSilhouetteEnabled() = 0;
    virtual String getClockPeriod() = 0;
    virtual String getAqiLevelIconFileName() = 0;
    virtual String getAqiNaLevelIconFileName() = 0;
};

#endif
