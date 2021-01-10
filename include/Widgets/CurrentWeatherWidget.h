#ifndef _CURRENT_WEATHER_WIDGET_H
#define _CURRENT_WEATHER_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/CurrentWeatherModel.h"

class CurrentWeatherWidget : public Widget
{
public:
    CurrentWeatherWidget(CurrentWeatherModel *model, uint16_t y);

protected:
    virtual void redraw();
    virtual void update(uint32_t now);

private:
    CurrentWeatherModel *model;
    uint32_t lastObservationTime;
    float lastLocalTemp = NAN;
    float lastCurrentTemp = NAN;

    void redrawLocalTemp();
    void redrawCurrentTemp();

};

#endif
