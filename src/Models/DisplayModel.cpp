#include "Models/DisplayModel.h"
#include "assert.h"

DisplayModel::DisplayModel(Config *config) : config(config), aqiData { config, config, config }
{
}

String DisplayModel::getTimeString(time_t time)
{
    struct tm *timeinfo = localtime(&time);
    String hour = isClock24hSytyle() ? String(timeinfo->tm_hour) : String((timeinfo->tm_hour + 11) % 12 + 1); // take care of noon and midnight
    String minutes = timeinfo->tm_min >= 10 ? String(timeinfo->tm_min) : "0" + String(timeinfo->tm_min);
    return hour + ":" + minutes;
}

String DisplayModel::getSunTranslation()
{
    return config->getSunTranslation();
}

String DisplayModel::getMoonTranslation()
{
    return config->getMoonTranslation();
}


String DisplayModel::getSunsetTime()
{
    return getTimeString(currentWeather.sunset);
}

String DisplayModel::getSunriseTime()
{
    return getTimeString(currentWeather.sunrise);
}

String DisplayModel::getMoonriseTime()
{
    return getTimeString(moonData.rise);
}

String DisplayModel::getMoonsetTime()
{
    return getTimeString(moonData.set);
}

String DisplayModel::getMoonIconFileName()
{
    float iconsCount = 23.00;
    // approximate moon age
    String iconNo = String(moonData.age * iconsCount / 29.53, 0);
    iconNo.trim();
    return "/moon/moon" + iconNo + ".bmp";
}

String DisplayModel::getMoonPhaseName()
{
    return config->getMoonPhaseName(moonData.phase.index);
}

String DisplayModel::getCurrentTime()
{
    return getTimeString(time(nullptr));
}

boolean DisplayModel::isAqiDataValid()
{
    boolean to_return = false;
    for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    {
        to_return |= aqiData[i].isAqiDataValid();
    }
    return to_return;
}

uint16_t DisplayModel::getAqiPercentMax()
{
    uint16_t to_return;
    for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    {
        if (aqiData[i].isAqiDataValid()) 
        {
            to_return = aqiData[i].getAqiPercentMax();
            break;
        }
    }
    return to_return;
}

String DisplayModel::getCurrentDate()
{
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    return config->getWeekDayName(timeinfo->tm_wday) + " " +
           timeinfo->tm_mday + " " +
           config->getMonthName(timeinfo->tm_mon) + " " +
           String(timeinfo->tm_year + 1900);
}

boolean DisplayModel::isClock24hSytyle()
{
    return config->isClock24hStyleSelected();
}

boolean DisplayModel::isClockSilhouetteEnabled()
{
    return config->isClockSilhouetteEnabled();
}

String DisplayModel::getClockPeriod()
{
    time_t now = time(nullptr);
    struct tm *timeinfo = localtime(&now);
    return timeinfo->tm_hour >= 12 ? "pm" : "am";
}

String DisplayModel::getAqiLevelIconFileName()
{
    String icon = "";
    for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    {
        if (aqiData[i].isAqiDataValid()) 
        {
            int aqi = int(aqiData[i].getAqiLevel());
            icon = String(aqi == 0 ? 1 : aqi);
            break;
        }
    }
    return "/aqi/" + icon + ".bmp";
}

String DisplayModel::getAqiNaLevelIconFileName()
{
    return "/aqi/na.bmp";
}

String DisplayModel::getCurrentWeatherIconFileName()
{
    return "/weather/115x79/" + currentWeather.icon + ".bmp";
}

String DisplayModel::getCurrentWeatherCityName()
{
    return currentWeather.cityName;
}

float DisplayModel::getCurrentWeatherTemp()
{
    return aqiData[0].isAqiDataValid() ? aqiData[0].getAqiTemp() : currentWeather.temp;
}

String DisplayModel::getCurrentWeatherDescription()
{
    return currentWeather.description;
}

uint16_t DisplayModel::getCurrentWeatherPressure()
{
    return currentWeather.pressure;
}

uint8_t DisplayModel::getCurrentWeatherHumidity()
{
    return currentWeather.humidity;
}

boolean DisplayModel::isLocalTempSensorEnabled()
{
    return config->isLocalTempSensorEnabled();
}

float DisplayModel::getLocalTemp()
{
    return localTempSensor.temp;
}

float DisplayModel::getLocalHumidity()
{
    return localTempSensor.humidity;
}

uint32_t DisplayModel::getObservationTime()
{
    return currentWeather.observationTime;
}

boolean DisplayModel::isMetricSelected()
{
    return config->isMetricSelected();
}

String DisplayModel::getForecastIconFileName(uint8_t dayIndex)
{
    assert(dayIndex < MAX_FORECASTS);
    return "/weather/60x41/" + String(dayIndex < MAX_FORECASTS ? forecasts[dayIndex].icon : "") + ".bmp";
}

float DisplayModel::getForecastTemp(uint8_t dayIndex)
{
    assert(dayIndex < MAX_FORECASTS);
    return (forecasts[dayIndex].temp);
}

String DisplayModel::getForecastWeekDayName(uint8_t dayIndex)
{
    assert(dayIndex < MAX_FORECASTS);
    time_t time = forecasts[dayIndex].observationTime; // TODO: + currentWeather.timezone;
    struct tm *timeinfo = localtime(&time);
    return config->getWeekDayName(timeinfo->tm_wday);
}
