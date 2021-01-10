#include "DataSources/AqiDataSource.h"
#include "PollutionLevels.h"
#include "assert.h"

// leave space at the beggining of each string so indexOf could be > 0
const char P10_VALUES[] PROGMEM = " SDS_P1,PMS_P1,HPM_P1";
const char P25_VALUES[] PROGMEM = " SDS_P2,PMS_P2,HPM_P2";
const char TEMP_VALUES[] PROGMEM = " BME280_temperature,BMP_temperature";
const char HUMIDITY_VALUES[] PROGMEM = " BME280_humidity";
const char PRESSURE_VALUES[] PROGMEM = " BME280_pressure,BMP_pressure";

AqiDataSource::AqiDataSource() : resolver(udp)
{
}

void AqiDataSource::update()
{
    resolver.loop();
}


void AqiDataSource::updateCurrent(AqiDataModel *data)
{
    assert(data != nullptr);
    if (parseUrl(data->stationUrl, data))
    {
        if (this->host.endsWith(F(".local")))
        {
            IPAddress ip = resolver.search(this->host.c_str());
            if (ip != INADDR_NONE)
            {
                Serial.print(F("Resolved IP for ")); Serial.println(this->host);
                this->host = ip.toString();
            }
            else
            {
                Serial.print(F("Error: cannot resolve IP for ")); Serial.println(this->host);
                return;
            }
        }
        this->data = data;
        this->data->index = NAN;
        JsonStreamingParser parser;
        parser.setListener(this);

        HTTPClient *http = new HTTPClient();
        if (this->https)
        {
            Serial.print(F("[HTTPS] "));
            BearSSL::WiFiClientSecure *httpsClient = new BearSSL::WiFiClientSecure();
            httpsClient->setInsecure();
            this->client = httpsClient;
        }
        else
        {
            Serial.print(F("[HTTP] "));
            this->client = new WiFiClient();
        }
        http->begin(*client, this->host, this->port, this->path, this->https);
        Serial.print(F("GET -> "));
        Serial.println(this->host + this->path);

        http->setReuse(false);
        // start connection and send HTTP header
        int httpCode = http->GET();

        if (httpCode == HTTP_CODE_OK)
        {
            char c = 0;
            // get tcp stream
            WiFiClient *stream = http->getStreamPtr();
            uint32_t timeout = millis();
            // read all data from server
            while (http->connected() || stream->available())
            {
                if (stream->available())
                {
                    c = stream->read();
                    parser.parse(c);
                }
                if ((millis() - timeout) > 8000UL)
                {
                    Serial.println(F("[AqiDataSource] JSON client timeout"));
                    break;
                }
                // give WiFi and TCP/IP libraries a chance to handle pending events
                yield();
            }
            if (isnan(data->index) && (data->pm25 != 0 || data->pm10 != 0))
            {
                data->index = PollutionLevels::getLevel(data->pm25, data->pm10);
            }
            Serial.print(F("AQI: "));
            Serial.println(data->index);
        }
        else
        {
            Serial.print(F("Error: recieved http response: "));
            Serial.println(httpCode);
        }

        parser.reset();
        delete http;
        delete this->client;
        this->data = nullptr;
        this->client = nullptr;
    }
}

boolean AqiDataSource::parseUrl(String url, AqiDataModel *data)
{
    assert(data != nullptr);

    int index = url.indexOf(':');
    if (index < 0)
    {
        Serial.println(F("Error: parseUrl - no protocol defined"));
        return false;
    }

    String protocol = url.substring(0, index);
    url.remove(0, (index + 3));

    if (protocol == "http")
    {
        this->port = 80;
        this->https = false;
    }
    else if (protocol == "https")
    {
        this->port = 443;
        this->https = true;
    }
    else
    {
        Serial.println(F("Error: parseUrl - unknown protocol"));
        return false;
    }

    index = url.indexOf('/');
    this->host = url.substring(0, index);
    url.remove(0, index);

    index = this->host.indexOf(':');
    if (index >= 0)
    {
        this->port = this->host.substring(index + 1).toInt();
        this->host.remove(0, (index + 1));
    }
    this->path = url;

    // Serial.println("Host: " + this->host);
    // Serial.println("Port: " + String(this->port));
    // Serial.println("Path: " + this->path);

    return true;
}

void AqiDataSource::whitespace(char c)
{
    //Serial.printf("JSON whitespace\r\n");
}

void AqiDataSource::startDocument()
{
    //Serial.printf("JSON Start Document\r\n");
}

void AqiDataSource::key(String key)
{
    //Serial.printf("JSON key %s\r\n", key.c_str());
    currentKey = String(key);
}

void AqiDataSource::value(String value)
{
    assert(data != nullptr);
    // Sensor.Community parse
    if (currentArray == "sensordatavalues")
    {
        if (currentKey == "value_type")
        {
            this->currentValueType = value;
        }
        if (currentKey == "value")
        {

            if (String(FPSTR(P10_VALUES)).indexOf(this->currentValueType) > 0)
            {
                this->data->pm10 = value.toInt();
            }
            else if (String(FPSTR(P25_VALUES)).indexOf(this->currentValueType) > 0)
            {
                this->data->pm25 = value.toInt();
            }
            else if (String(FPSTR(TEMP_VALUES)).indexOf(this->currentValueType) > 0)
            {
                this->data->temp = value.toFloat();
            }
            else if (String(FPSTR(HUMIDITY_VALUES)).indexOf(this->currentValueType) > 0)
            {
                this->data->humidity = value.toInt();
            }
            else if (String(FPSTR(PRESSURE_VALUES)).indexOf(this->currentValueType) > 0)
            {
                this->data->pressure = value.toInt() / 100;
            }
        }
    }
    // Aqi Eco parse
    if (currentParent == "last_data")
    {
        if (currentKey == "pm25")
        {
            this->data->pm25 = value.toInt();
        }
        if (currentKey == "pm10")
        {
            this->data->pm10 = value.toInt();
        }
        if (currentKey == "temperature")
        {
            this->data->temp = value.toFloat();
        }
        if (currentKey == "pressure")
        {
            this->data->pressure = value.toInt();
        }
        if (currentKey == "humidity")
        {
            this->data->humidity = value.toInt();
        }
    }
    if (currentParent == "average_1h")
    {
        if (currentKey == "index_num")
        {
            this->data->index = value.toFloat();
        }
    }
}

// boolean AqiDataSource::inArray(const String array[], String str)
// {
//     for (int i = 0; array[i].length() > 0; i++)
//     {
//         if (array[i] == str)
//         {
//             return true;
//         }
//     }
//     return false;
// }

void AqiDataSource::startArray()
{
    //Serial.printf("JSON start array\r\n");
    currentArray = currentKey;
}

void AqiDataSource::endArray()
{
    //Serial.printf("JSON end array\r\n");
    currentArray = "";
}

void AqiDataSource::startObject()
{
    //Serial.printf("JSON start object\r\n");
    currentParent = currentKey;
}

void AqiDataSource::endObject()
{
    //Serial.printf("JSON end object\r\n");
    currentParent = "";
}

void AqiDataSource::endDocument()
{
    //Serial.printf("JSON end document\r\n");
}
