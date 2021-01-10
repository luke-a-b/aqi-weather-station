#include <Arduino.h>

#include "PollutionLevels.h"
const uint8_t PollutionLevels::MAX_LEVELS = 5;
const uint8_t PollutionLevels::PM25_LEVELS[] = {0, 15, 30, 55, 110};
const uint8_t PollutionLevels::PM10_LEVELS[] = {0, 25, 50, 90, 180};
const uint8_t PollutionLevels::PM25_MAX = 25;
const uint8_t PollutionLevels::PM10_MAX = 50;

float PollutionLevels::getLevel(uint16_t pm25, uint16_t pm10)
{
    float pm25Level = findThreshold(pm25, PM25_LEVELS);
    float pm10Level = findThreshold(pm10, PM10_LEVELS);
    return max(pm25Level, pm10Level);
}

float PollutionLevels::findThreshold(float value, const uint8_t levels[])
{
    float prevLevel = 0;
    for (int i = 1; i < MAX_LEVELS; i++)
    {
        float currentLevel = levels[i];
        if (currentLevel > value)
        {
            return ((value - prevLevel) / (currentLevel - prevLevel) + i - 1) / (1 - 1 / MAX_LEVELS);
        }
        prevLevel = currentLevel;
    }
    return MAX_LEVELS;
}
