#include <Arduino.h>
#include "Backlight.h"

Backlight::Backlight(unsigned long backlightTimeout)
{
    pinMode(TFT_BL, OUTPUT);
    setBacklightTimeout(backlightTimeout);
}

void Backlight::update(uint32_t now)
{
    if (this->backlightTimeout != 0 && now - this->lastUpdate > this->backlightTimeout * 1000)
    {
        setBacklight(false);
    }
}

void Backlight::setBacklightTimeout(unsigned long backlightTimeout)
{
    this->backlightTimeout = backlightTimeout;
    setBacklight(true);
}

boolean Backlight::getBacklight()
{
    return digitalRead(TFT_BL) == TFT_BACKLIGHT_ON;
}

void Backlight::setBacklight(boolean backlight)
{
    if (backlight)
    {
        digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
        this->lastUpdate = millis();
    }
    else
    {
        digitalWrite(TFT_BL, !TFT_BACKLIGHT_ON);
    }
}
