#ifndef _FORECAST_WIDGET_H
#define _FORECAST_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/ForecastModel.h"

class ForecastWidget : public Widget
{
public:
    ForecastWidget(ForecastModel *model, uint16_t x, uint16_t y, uint8_t dayIndex);

protected:
    virtual void redraw();
    ForecastModel *model;
    uint8_t dayIndex;
};

#endif
