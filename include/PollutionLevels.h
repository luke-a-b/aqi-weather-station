#ifndef _POLLUTION_H
#define _POLLUTION_H

#include <Arduino.h>

class PollutionLevels
{
public:
    static const uint8_t MAX_LEVELS;
    static const uint8_t PM25_LEVELS[];
    static const uint8_t PM10_LEVELS[];
    static const uint8_t PM25_MAX;
    static const uint8_t PM10_MAX;

    static float getLevel(uint16_t pm25, uint16_t pm10);

private:
    static float findThreshold(float value, const uint8_t levels[]);
};

#endif