#ifndef _ASTRONOMY_MODEL_H
#define _ASTRONOMY_MODEL_H

#include <Arduino.h>

class AstronomyModel {
public:
  virtual bool isClock24hSytyle() = 0;
  virtual struct tm *getSunsetTime() = 0;
  virtual struct tm *getSunriseTime() = 0;
  virtual struct tm *getMoonriseTime() = 0;
  virtual struct tm *getMoonsetTime() = 0;
  virtual String getMoonIconFileName() = 0;
  virtual uint8_t getMoonPhase() = 0;
};

#endif
