#include "DataSources/LocalTempSensorSource.h"
#include "Config.h"
#include "assert.h"

#define DHTPIN RX

LocalTempSensorSource::LocalTempSensorSource(String sensorType, boolean isMetric) : isMetric(isMetric)
{
    if (sensorType == LocalTempSensorValues[DHT11_IDX] ||
        sensorType == LocalTempSensorValues[DHT12_IDX] ||
        sensorType == LocalTempSensorValues[DHT21_IDX] ||
        sensorType == LocalTempSensorValues[DHT22_IDX])
    {
        Serial.print(F("Creating internal sensor: DHT")); Serial.println(sensorType.substring(3));
        dht = new DHT(DHTPIN, sensorType.substring(3).toInt());
        dht->begin();
    }
}

LocalTempSensorSource::~LocalTempSensorSource()
{
    if (dht != nullptr) delete dht;
}

boolean LocalTempSensorSource::updateCurrent(LocalTempSensorDataModel *data)
{
    assert(data != nullptr);
    data->temp = NAN;
    data->humidity = NAN;
    if (dht != nullptr)
    {
        Serial.println(F("Updating local DHT sensor data..."));
        data->temp = dht->readTemperature(!isMetric);
        data->humidity = dht->readHumidity();
    }
    return (!isnan(data->temp) && !isnan(data->humidity));
}
