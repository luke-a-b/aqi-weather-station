#ifndef _LOCAL_SENSOR_MODEL_H
#define _LOCAL_SENSOR_MODEL_H

#include <Arduino.h>

class LocalSensorModel {
public:
  virtual boolean isLocalTempSensorEnabled() = 0;
  virtual float getLocalTemp() = 0;
  virtual float getLocalHumidity() = 0;
};

#endif
