#ifndef _MAIN_PAGE_H
#define _MAIN_PAGE_H

#include "Page.h"
#include "Models/DisplayModel.h"
#include "Widgets/ClockWidget.h"
#include "Widgets/CurrentWeatherWidget.h"
#include "Widgets/ForecastCarouselWidget.h"
#include "Widgets/AstronomyWidget.h"
#include "CommandsHanler.h"

class MainPage : public Page
{
public:
    MainPage(DisplayModel *model, CommandsHanler* commandsHandler);
protected:
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);

private:
    virtual void draw();
    ClockWidget clockWidget;
    CurrentWeatherWidget currentWeatherWidget;
    ForecastCarouselWidget forecastCarouselWidget;
    AstronomyWidget astronomyWidget;
};

#endif
