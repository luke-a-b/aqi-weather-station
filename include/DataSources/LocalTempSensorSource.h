#ifndef _LOCAL_TEMP_SENSOR_SOURCE_H
#define _LOCAL_TEMP_SENSOR_SOURCE_H

#include <Arduino.h>
#include <DHT.h>

typedef struct LocalTempSensorDataModel {
  float temp = NAN;
  float humidity = NAN;
} LocalTempSensorDataModel;

class LocalTempSensorSource {
public:
  LocalTempSensorSource(String sensorType, boolean isMetric);
  ~LocalTempSensorSource();
  boolean updateCurrent(LocalTempSensorDataModel *data);

private:
  boolean isMetric;
  DHT *dht = nullptr;
};

#endif