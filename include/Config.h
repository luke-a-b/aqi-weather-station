#ifndef _CONFIG_H
#define _CONFIG_H

#include <Arduino.h>
#include <IotWebConf.h>
#include <IotWebConfUsing.h> 


#define NTP_MIN_VALID_EPOCH 1533081600 // August 1st, 2018
#define LONG_STRING_LEN 128
#define SHORT_STRING_LEN 32

#define MAX_AQI_STATIONS 3

const char NtpServers[] PROGMEM = "0.pool.ntp.org, 1.pool.ntp.org, 1.pool.ntp.org";

// translations
const char WeekDayNamesPL[] PROGMEM = "Niedz., Pon., Wt., Śr., Czw., Pt., Sob.";
const char WeekDayNamesEN[] PROGMEM = "Sun., Mon., Tue., Wed., Thur., Fri., Sat.";
const char MonthNamesPL[] PROGMEM = "Styczeń, Luty, Marzec, Kwiecień, Maj, Czerwiec, Lipiec, Sierpień, Wrzesień, Październik, Listopad, Grudzień";
const char MonthNamesEN[] PROGMEM = "January, February, March, April, May, June, July, August, September, October, November, December";
const char AstronomyTextsPL[] PROGMEM = "Słońce, Księżyc";
const char AstronomyTextsEN[] PROGMEM = "Sun, Moon";
const char MoonPhasesNamesPL[] PROGMEM = "Nów, Wzarastający Sierp, Pierwsza Kwadra, Poszerzony, Pełnia, Garbaty, Trzecia Kwadra, Malejący Sierp";
const char MoonPhasesNamesEN[] PROGMEM = "New Moon, Waxing Crescent, First Quarter, Waxing Gibbous, Full Moon, Waning Gibbous, Last/Third Quarter, Waning Crescent";
////////////////


#define CHECKBOX_PARAM_LEN 16
#define CHECKBOX_PARAM(name, label, default)\
        char name[CHECKBOX_PARAM_LEN] = {0};\
        IotWebConfCheckboxParameter name##Param = { label, #name, name, CHECKBOX_PARAM_LEN, default};

#define NUMBER_PARAM_LEN 16
#define NUMBER_PARAM(name, label, default, placeholder, custom)\
        char name[NUMBER_PARAM_LEN] = {0};\
        IotWebConfNumberParameter name##Param = { label, #name, name, NUMBER_PARAM_LEN, default, placeholder, custom};

#define TEXT_PARAM_LEN 128
#define TEXT_PARAM(name, label, default, placeholder)\
        char name[TEXT_PARAM_LEN] = {0};\
        IotWebConfTextParameter name##Param = { label, #name, name, TEXT_PARAM_LEN, default, placeholder};

#define SELECT_PARAM_LEN 32
#define SELECT_PARAM(name, label, values, names, default)\
        char name[SELECT_PARAM_LEN] = {0};\
        IotWebConfSelectParameter name##Param = { label, #name, name, SELECT_PARAM_LEN, (char *)values, (char *)names, sizeof(values) / SELECT_PARAM_LEN, SELECT_PARAM_LEN, default};

#define GROUP_PARAM(name, description) IotWebConfParameterGroup group##name = { description };

const char LanguageValues[][SELECT_PARAM_LEN] = {"pl", "en"};
const char LanguageNames[][SELECT_PARAM_LEN] = {"Polish", "English"};

enum LocalTempSensorValuesIdx { DHT11_IDX=1, DHT12_IDX, DHT21_IDX, DHT22_IDX };
const char LocalTempSensorValues[][SELECT_PARAM_LEN] = {"NONE", "DHT11", "DHT12", "DHT21", "DHT22"};
const char LocalTempSensorNames[][SELECT_PARAM_LEN] = {"NONE", "DHT11", "DHT12", "DHT21 (AM2301)", "DHT22 (AM2302, AM2321)"};



typedef struct
{
    CHECKBOX_PARAM(IsMetricSelected, "Is metric system?", true)
    CHECKBOX_PARAM(IsClock24hStyleSelected, "Is clock 24h style?", true)
    CHECKBOX_PARAM(IsClockSilhouetteEnabled, "Is clock silhouette enabled?", true)   
    NUMBER_PARAM(BacklightTimeout, "Backlight timeout", "0", "0..600", "min='0' max='600' step='1'")
    SELECT_PARAM(Language, "Display language", LanguageValues, LanguageNames, nullptr)

    GROUP_PARAM(Owm, "Open Weather Map settings")
    TEXT_PARAM(OwmApiKey, "Open Weather Map API key", nullptr, "Sign up here to get an API key: https://docs.thingpulse.com/how-tos/openweathermap-key/")
    /*
    Go to https://openweathermap.org/find?q= and search for a location. Go through the
    result set and select the entry closest to the actual location you want to display 
    data for. It'll be a URL like https://openweathermap.org/city/3081368. The number
    at the end is what you assign to the constant below.
    */
    TEXT_PARAM(OwmLocationId, "Open Weather Map location ID", nullptr, "Go to https://openweathermap.org/find?q= and search for a location")
    NUMBER_PARAM(OwmRefreshInterval, "Open Weather Map refresh data interval seconds", "240", "1..600", "min='1' max='600' step='1'")

    GROUP_PARAM(Aqi, "Air Quality Index stations settings")
    TEXT_PARAM(AqiStationUrl1, "AQI stattion URL 1", nullptr, "Put your first AQI station URL here")
    TEXT_PARAM(AqiStationUrl2, "AQI stattion URL 2", nullptr, "Put your second AQI station URL here")
    TEXT_PARAM(AqiStationUrl3, "AQI stattion URL 3", nullptr, "Put your third AQI station URL here")
    NUMBER_PARAM(AqiRefreshInterval, "AQI refresh data interval seconds", "120", "0..600", "min='0' max='600' step='1'")

    GROUP_PARAM(Sensor, "Local temperature/humidity sensor settings")
    SELECT_PARAM(LocalTempSensorType, "Select type of connected sensor: ", LocalTempSensorValues, LocalTempSensorNames, nullptr)
    NUMBER_PARAM(LocalTempSensorRefreshInterval, "Local sensor refresh data interval seconds", "60", "0..600", "min='0' max='600' step='1'")

} Params;

class Config
{
public:
    Config();

    void addCustomWebParams(IotWebConf *iotWebConf);

    boolean isMetricSelected()              { return params.IsMetricSelectedParam.isChecked(); }
    boolean isClock24hStyleSelected()       { return !strcmp(params.IsClock24hStyleSelected, "selected"); }
    boolean isClockSilhouetteEnabled()      { return !strcmp(params.IsClockSilhouetteEnabled, "selected"); }
    boolean isLocalTempSensorEnabled()      { return strcmp(params.LocalTempSensorType, "NONE"); }
    String getLocalTempSensorType()         { return params.LocalTempSensorType; }
    int getBacklightTimeout()               { return atoi(params.BacklightTimeout); }
    String getLanguage()                    { return params.Language; }
    String getOwmApiKey()                   { return params.OwmApiKey; }
    String getOwmLocationId()               { return params.OwmLocationId; }
    int getOwmRefreshInterval()             { return atoi(params.OwmRefreshInterval); }
    int getAqiRefreshInterval()             { return atoi(params.AqiRefreshInterval); }
    int getLocalTempSensorRefreshInterval() { return atoi(params.LocalTempSensorRefreshInterval); }
    //Params *getParams()                     { return &params; }
    String getNtpServer(uint8_t serverNo);
    String getAqiStationUrl(uint8_t no);
    String getWeekDayName(uint8_t dayNo);
    String getMonthName(uint8_t monthNo);
    String getMoonPhaseName(uint8_t moonPhase);
    String getSunTranslation();
    String getMoonTranslation();

    boolean validateWebParams(WebServer *server);

    void print();

private:
    Params params;

    boolean validateAqiUrl(String url);
    String getCommaSeparatedSubstring(String inputString, uint8_t substringIndex);

};

#endif
