#ifndef _MODEL_CHANGE_OBSERVER_H
#define _MODEL_CHANGE_OBSERVER_H

#include <Arduino.h>

class ModelChangeObserver 
{
public:
    virtual void notifyAqiUpdateStart() = 0;
    virtual void notifyAqiUpdated(boolean status) = 0;
    virtual void notifyWeatherUpdateStart() = 0;
    virtual void notifyWeatherUpdated(boolean status) = 0;
    virtual void notifyLocalTempUpdated(boolean status) = 0;
};

#endif
