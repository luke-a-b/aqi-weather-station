#ifndef _FORECAST_PAGE_H
#define _FORECAST_PAGE_H

#include "Page.h"
#include "Models/ForecastModel.h"

#define MAX_FORECAST_PAGES 2

class ForecastPage : public Page {
public:
  ForecastPage(ForecastModel *model, CommandsHanler *commandsHandler);

protected:
  virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
  virtual void notifyWeatherUpdated();

private:
  virtual void draw();
  ForecastModel *model;
  uint8_t currentPage = 0;
};

#endif
