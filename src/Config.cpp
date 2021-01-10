#include <assert.h>
#include "Config.h"


Config::Config()
{

}

void Config::addCustomWebParams(IotWebConf *iotWebConf)
{
    iotWebConf->addSystemParameter(&params.LanguageParam);
    iotWebConf->addSystemParameter(&params.IsMetricSelectedParam);
    iotWebConf->addSystemParameter(&params.IsClock24hStyleSelectedParam);
    iotWebConf->addSystemParameter(&params.IsClockSilhouetteEnabledParam);
    iotWebConf->addSystemParameter(&params.BacklightTimeoutParam);

    params.groupOwm.addItem(&params.OwmApiKeyParam);
    params.groupOwm.addItem(&params.OwmLocationIdParam);
    params.groupOwm.addItem(&params.OwmRefreshIntervalParam);
    iotWebConf->addParameterGroup(&params.groupOwm);
    
    params.groupAqi.addItem(&params.AqiStationUrl1Param);
    params.groupAqi.addItem(&params.AqiStationUrl2Param);
    params.groupAqi.addItem(&params.AqiStationUrl3Param);
    params.groupAqi.addItem(&params.AqiRefreshIntervalParam);
    iotWebConf->addParameterGroup(&params.groupAqi);

    params.groupSensor.addItem(&params.LocalTempSensorTypeParam);
    params.groupSensor.addItem(&params.LocalTempSensorRefreshIntervalParam);
    iotWebConf->addParameterGroup(&params.groupSensor);
}

boolean Config::validateAqiUrl(String url)
{
    return url.length() == 0 || // accepting empty url
           (url.startsWith(F("http://")) || url.startsWith(F("https://"))) && url.endsWith(F("/data.json"));
}

boolean Config::validateWebParams(WebServer *server)
{
    Serial.println(F("Validating form."));
    boolean valid = true;

    if (server->arg(params.OwmApiKeyParam.getId()).length() == 0)
    {
        params.OwmApiKeyParam.errorMessage = "Please provide OWM API key.";
        valid = false;
    }

    if (server->arg(params.OwmLocationIdParam.getId()).length() == 0)
    {
        params.OwmLocationIdParam.errorMessage = "Please provide OWM location ID.";
        valid = false;
    }

    const char *aqiError = "Please profide URL in following format http://...../data.json or https://...../data.json";
    if (!validateAqiUrl(server->arg(params.AqiStationUrl1Param.getId())))
    {
        params.AqiStationUrl1Param.errorMessage = aqiError;
        valid = false;
    }
    if (!validateAqiUrl(server->arg(params.AqiStationUrl2Param.getId())))
    {
        params.AqiStationUrl2Param.errorMessage = aqiError;
        valid = false;
    }
    if (!validateAqiUrl(server->arg(params.AqiStationUrl3Param.getId())))
    {
        params.AqiStationUrl3Param.errorMessage = aqiError;
        valid = false;
    }

    return valid;
}


String Config::getAqiStationUrl(uint8_t no)
{
    assert(no < MAX_AQI_STATIONS);
    String to_return;
    switch (no)
    {
    default:
    case 0:
        to_return = params.AqiStationUrl1;
        break;
    case 1:
        to_return = params.AqiStationUrl2;
        break;
    case 2:
        to_return = params.AqiStationUrl3;
    }
    return to_return;
}

String Config::getNtpServer(uint8_t serverNo)
{
    return getCommaSeparatedSubstring(String(FPSTR(NtpServers)), serverNo);
}

String Config::getWeekDayName(uint8_t dayNo)
{
    return getCommaSeparatedSubstring(String(params.Language) == "pl" ? FPSTR(WeekDayNamesPL) : FPSTR(WeekDayNamesEN), dayNo);
}

String Config::getMonthName(uint8_t monthNo)
{
    return getCommaSeparatedSubstring(String(params.Language) == "pl" ? FPSTR(MonthNamesPL) : FPSTR(MonthNamesEN), monthNo);
}

String Config::getMoonPhaseName(uint8_t moonPhase)
{
    return getCommaSeparatedSubstring(String(params.Language) == "pl" ? FPSTR(MoonPhasesNamesPL) : FPSTR(MoonPhasesNamesEN), moonPhase);
}

String Config::getSunTranslation()
{
    return getCommaSeparatedSubstring(String(params.Language) == "pl" ? FPSTR(AstronomyTextsPL) : FPSTR(AstronomyTextsEN), 0);
}

String Config::getMoonTranslation()
{
    return getCommaSeparatedSubstring(String(params.Language) == "pl" ? FPSTR(AstronomyTextsPL) : FPSTR(AstronomyTextsEN), 1);
}


String Config::getCommaSeparatedSubstring(String inputString, uint8_t substringIndex)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = inputString.length();

    for (int i = 0; i <= maxIndex && found <= substringIndex; i++)
    {
        if (inputString.charAt(i) == ',' || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    String toRetrun = found > substringIndex ? inputString.substring(strIndex[0], strIndex[1]) : "";
    toRetrun.trim();
    return toRetrun;
}

void Config::print()
{
    Serial.println();
    Serial.println("============== Config =================");
    Serial.printf("Is metric?: %s\n", params.IsMetricSelected);

    Serial.printf("Open Weather API Key: %s\n", params.OwmApiKey);
    Serial.printf("Open Weather location ID: %s\n", params.OwmLocationId);
    Serial.printf("Open Weather language: %s\n", params.Language);
    //Serial.printf("Display city name: %s\n", params.DisplayCityName.c_str());

    //Serial.printf("UTC Offset: %s\n", params.UtcOffset);

    Serial.printf("NTP server 0: %s\n", getNtpServer(0).c_str());
    Serial.printf("NTP server 1: %s\n", getNtpServer(1).c_str());
    Serial.printf("NTP server 2: %s\n", getNtpServer(2).c_str());
    Serial.println("=======================================");
}
