#include <LittleFS.h>
#include "Models/DisplayModel.h"
#include "assert.h"

DisplayModel::DisplayModel(Config *config)
    : config(config), aqiData{config, config, config} {}

struct tm *DisplayModel::getLocalTime(time_t time) {
  return localtime(&time);
}

struct tm *DisplayModel::getSunsetTime() {
  return getLocalTime(currentWeather.sunset);
}

struct tm *DisplayModel::getSunriseTime() {
  return getLocalTime(currentWeather.sunrise);
}

struct tm *DisplayModel::getMoonriseTime() {
  return getLocalTime(moonData.rise);
}

struct tm *DisplayModel::getMoonsetTime() {
  return getLocalTime(moonData.set);
}

String DisplayModel::getMoonIconFileName() {
  String path = FPSTR(MOON_ICON_PATH);
  String icon = String(moonData.age * 23.00 / 29.53, 0);
  icon.trim();
  path.replace(F("{icon}"), icon);
  return path;
}

uint8_t DisplayModel::getMoonPhase() { return moonData.phase; }

boolean DisplayModel::isAqiDataValid() {
  boolean to_return = false;
  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++) {
    to_return |= aqiData[i].isAqiDataValid();
  }
  return to_return;
}

uint16_t DisplayModel::getAqiPercentMax() {
  uint16_t to_return = 0;
  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++) {
    if (aqiData[i].isAqiDataValid()) {
      to_return = aqiData[i].getAqiPercentMax();
      break;
    }
  }
  return to_return;
}

boolean DisplayModel::isClock24hSytyle() {
  return config->isClock24hStyleSelected();
}

boolean DisplayModel::isClockSilhouetteEnabled() {
  return config->isClockSilhouetteEnabled();
}

String DisplayModel::getAqiLevelIconFileName() {
  String icon = "";
  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++) {
    if (aqiData[i].isAqiDataValid()) {
      int aqi = int(aqiData[i].getAqiLevel());
      icon = String(aqi == 0 ? 1 : aqi);
      String path = FPSTR(AQI_ICON_PATH);
      path.replace(F("{icon}"), String(aqi == 0 ? 1 : aqi));
      return path;
    }
  }
  String path = FPSTR(AQI_ICON_PATH);
  path.replace(F("{icon}"), F("na"));
  return path;
}

String DisplayModel::getWeatherIconFileName(String path, String icon) {
  path.replace(F("{folder}"), config->getMeteoIcons());
  if (icon != "") {
    path.replace(F("{icon}"), icon);
    if (!LittleFS.exists(path)) {
      String currentIcon = icon;
      icon.replace("n", "d"); // use day icon instead
      path.replace(currentIcon, icon);
      if (!LittleFS.exists(path)) {
        path.replace(icon, F("na"));
      }
    }
  } else {
    path.replace(F("{icon}"), F("na"));
  }
  return path;
}

void DisplayModel::updateWeatherIconsPaths() {
  if (currentWeather.icon.indexOf(".bmp") < 0) // check if not already done
  {
    currentWeather.icon = getWeatherIconFileName(FPSTR(WEATHER_ICON_LARGE_PATH),
                                                 currentWeather.icon);
  }
  for (uint8_t i = 0; i < MAX_DAYS; i++) {
    if (dailyForecasts.icon[i].indexOf(".bmp") < 0) // check if not already done
    {
      dailyForecasts.icon[i] = getWeatherIconFileName(
          FPSTR(WEATHER_ICON_SMALL_PATH), dailyForecasts.icon[i]);
    }
  }
}

String DisplayModel::getCurrentWeatherIconFileName() {
  return currentWeather.icon;
}

String DisplayModel::getCurrentWeatherCityName() {
  return config->getOwmLocationName();
}

float DisplayModel::getCurrentWeatherTemp() { return currentWeather.temp; }

float DisplayModel::getTodayMinTemp() { return dailyForecasts.temp_min[0]; }

float DisplayModel::getTodayMaxTemp() { return dailyForecasts.temp_max[0]; }

float DisplayModel::getCurrentFeelsLikeTemp() {
  return currentWeather.feels_like;
}

uint8_t DisplayModel::getCurrentClouds() { return currentWeather.clouds; }

float DisplayModel::getCurrentWindSpeed() { return currentWeather.wind_speed; }

float DisplayModel::getCurrentWindGust() { return currentWeather.wind_gust; }

uint16_t DisplayModel::getCurrentWindDeg() { return currentWeather.wind_deg; }

float DisplayModel::getCurrentRain() { return currentWeather.rain; }

float DisplayModel::getCurrentSnow() { return currentWeather.snow; }

float DisplayModel::getCurrentUvi() { return currentWeather.uvi; }

uint32_t DisplayModel::getCurrentVisiblity() {
  return currentWeather.visibility;
}

float DisplayModel::getCurrentDewPoint() { return currentWeather.dew_point; }

String DisplayModel::getCurrentWeatherDescription() {
  return currentWeather.description;
}

uint16_t DisplayModel::getCurrentWeatherPressure() {
  return currentWeather.pressure;
}

uint8_t DisplayModel::getCurrentWeatherHumidity() {
  return currentWeather.humidity;
}

boolean DisplayModel::isLocalTempSensorEnabled() {
  return config->isLocalTempSensorEnabled();
}

float DisplayModel::getLocalTemp() { return localTempSensor.temp; }

float DisplayModel::getLocalHumidity() { return localTempSensor.humidity; }

uint32_t DisplayModel::getObservationTime() { return currentWeather.dt; }

String DisplayModel::getCurrentAirQuality() {
  String to_return = FPSTR(INTL_NO_DATA);
  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++) {
    if (aqiData[i].isAqiDataValid()) {
      int index = int(aqiData[i].getAqiLevel());
      switch (index) {
      case 0:
      case 1:
        to_return = FPSTR(INTL_AQI_GOOD);
        break;
      case 2:
        to_return = FPSTR(INTL_AQI_MODERATE);
        break;
      case 3:
        to_return = FPSTR(INTL_AQI_UNHEALTHY_SENSITIVE);
        break;
      case 4:
        to_return = FPSTR(INTL_AQI_UNHEALTHY);
        break;
      case 5:
        to_return = FPSTR(INTL_AQI_VERY_UNHEALTHY);
        break;
      case 6:
        to_return = FPSTR(INTL_AQI_HAZARDOUS);
        break;
      default:
        break;
      }
      break;
    }
  }
  return to_return;
}

boolean DisplayModel::isMetricSelected() { return config->isMetricSelected(); }

String DisplayModel::getForecastIconFileName(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return dailyForecasts.icon[dayIndex];
}

float DisplayModel::getForecastTemp(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.temp_day[dayIndex]);
}

struct tm *DisplayModel::getForecastTime(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return getLocalTime(dailyForecasts.dt[dayIndex] +
                      currentWeather.timezone_offset);
}

String DisplayModel::getForecastDescription(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.description[dayIndex]);
}

float DisplayModel::getForecastTempMin(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.temp_min[dayIndex]);
}

float DisplayModel::getForecastTempMax(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.temp_max[dayIndex]);
}

float DisplayModel::getForecastHumidity(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.humidity[dayIndex]);
}

float DisplayModel::getForecastPressure(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.pressure[dayIndex]);
}

float DisplayModel::getForecastWindSpeed(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  return (dailyForecasts.wind_speed[dayIndex]);
}

String DisplayModel::getForecastWindDirection(uint8_t dayIndex) {
  assert(dayIndex < MAX_DAYS);
  int windAngle = (dailyForecasts.wind_deg[dayIndex] + 22.5) / 45;
  if (windAngle > 7)
    windAngle = 0;
  switch (windAngle) {
  default:
  case 0:
    return "⇑";
  case 1:
    return "⇗";
  case 2:
    return "⇒";
  case 3:
    return "⇘";
  case 4:
    return "⇓";
  case 5:
    return "⇙";
  case 6:
    return "⇐";
  case 7:
    return "⇖";
  }
}
