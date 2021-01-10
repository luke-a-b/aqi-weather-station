#ifndef _ASTRONOMY_MODEL_H
#define _ASTRONOMY_MODEL_H

#include <Arduino.h>

class AstronomyModel
{
public:
    virtual String getSunTranslation() = 0;
    virtual String getMoonTranslation () = 0;
    virtual String getSunsetTime() = 0;
    virtual String getSunriseTime() = 0;
    virtual String getMoonriseTime() = 0;
    virtual String getMoonsetTime() = 0;
    virtual String getMoonIconFileName() = 0;
    virtual String getMoonPhaseName() = 0;
};

#endif
