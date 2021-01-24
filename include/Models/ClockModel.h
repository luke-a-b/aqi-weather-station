#ifndef _CLOCK_MODEL_H
#define _CLOCK_MODEL_H

#include <Arduino.h>

class ClockModel {
public:
  virtual boolean isAqiDataValid() = 0;
  virtual uint16_t getAqiPercentMax() = 0;
  virtual boolean isClock24hSytyle() = 0;
  virtual boolean isClockSilhouetteEnabled() = 0;
  virtual String getAqiLevelIconFileName() = 0;
};

#endif
