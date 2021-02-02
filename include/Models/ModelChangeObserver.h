#ifndef _MODEL_CHANGE_OBSERVER_H
#define _MODEL_CHANGE_OBSERVER_H

#include "Models/AqiDisplayModel.h"

class ModelChangeObserver {
public:
  virtual void notifyAqiUpdateStart(AqiDisplayModel *model) = 0;
  virtual void notifyAqiUpdated(AqiDisplayModel *model) = 0;
  virtual void notifyWeatherUpdateStart() = 0;
  virtual void notifyWeatherUpdated() = 0;
  virtual void notifyLocalTempUpdated() = 0;
  virtual void notifyAstronomyUpdated() = 0;
};

#endif
