#ifndef _CURRENT_WEATHER_DETAIL_PAGE_H
#define _CURRENT_WEATHER_DETAIL_PAGE_H

#include "Page.h"
#include "Models/CurrentWeatherModel.h"
#include "Widgets/CurrentWeatherWidget.h"

class CurrentWeatherDetailPage : public Page {
public:
  CurrentWeatherDetailPage(CurrentWeatherModel *model,
                           CommandsHanler *commandsHandler);

protected:
  virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
  virtual void notifyWeatherUpdated();

private:
  virtual void draw();

  CurrentWeatherModel *model;
  CurrentWeatherWidget currentWeatherWidget;
  uint32_t lastObservationTime = 0;
};

#endif
