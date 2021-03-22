#ifndef _PAGES_CONTROLLER_H
#define _PAGES_CONTROLLER_H

#include <Arduino.h>
#include "Backlight.h"
#include "TouchInterface.h"
#include "Models/ModelChangeObserver.h"
#include "Pages/MainPage.h"
#include "Pages/AqiPage.h"
#include "Pages/SplashScreenPage.h"
#include "Pages/AboutPage.h"
#include "Pages/CurrentWeatherDetailPage.h"
#include "Pages/ForecastPage.h"
#include "CommandsHandler.h"

class PagesController : public TouchObserver,
                        public ModelChangeObserver,
                        public CommandsHandler {
public:
  PagesController(Backlight *backlight, DisplayModel *model);
  void hideCurrentPage();
  void update(uint32_t now);
  virtual bool handleTouchOn(uint16_t x, uint16_t y, uint16_t z);
  virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
  virtual bool handleSwipeRight();
  virtual bool handleSwipeLeft();
  virtual bool handleSwipeDown();
  virtual bool handleSwipeUp();

public: // implements CommandsHandler
  virtual void showMainPage();
  virtual void showAqiPage();
  virtual void showCurrentWeatherDetail();
  virtual void showForecastPage();
  virtual void showAboutPage();
  virtual void setBacklightTimeout(unsigned long timeout);
  virtual void switchOffBacklight();
  virtual void switchOnBacklight();

public: // implements ModelChangeObserver
  virtual void notifyAqiUpdateStart(AqiDisplayModel *model);
  virtual void notifyAqiUpdated(AqiDisplayModel *model);
  virtual void notifyWeatherUpdateStart();
  virtual void notifyWeatherUpdated();
  virtual void notifyLocalTempUpdated();
  virtual void notifyAstronomyUpdated();

private:
  Backlight *backlight;
  DisplayModel *model;
  Page *currentPage;

  MainPage mainPage;
  AqiPage aqiPage;
  AboutPage aboutPage;
  CurrentWeatherDetailPage currentWeatherDetailPage;
  ForecastPage forecastPage;

  uint8_t currentAqiPage = 0;

  void deleteCurrentPage();
  void showPage(Page *page);
};

#endif
