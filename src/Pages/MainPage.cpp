#include "Pages/MainPage.h"

MainPage::MainPage(DisplayModel *model, CommandsHandler *commandsHandler)
    : Page(commandsHandler), clockWidget(model, 0),
      currentWeatherWidget(model, 85), forecastCarouselWidget(model, 184),
      astronomyWidget(model, 257)

{
  addWidget(&clockWidget);
  addWidget(&astronomyWidget);
  addWidget(&forecastCarouselWidget);
  addWidget(&currentWeatherWidget);
}

void MainPage::draw() {
  if (isVisible) {
    tft.drawFastHLine(10, 82, 220, TFT_DARKGREY);
    tft.drawFastHLine(10, 181, 220, TFT_DARKGREY);
    tft.drawFastHLine(10, 254, 220, TFT_DARKGREY);
  }
}

void MainPage::notifyWeatherUpdated() { currentWeatherWidget.update(); }

void MainPage::notifyAstronomyUpdated() { astronomyWidget.update(); }

bool MainPage::handleTouchOff(uint16_t x, uint16_t y, uint16_t z) {
  if (!this->isVisible)
    return false;
  bool to_return = Page::handleTouchOff(x, y, z);
  if (!to_return) {
    to_return = true;
    if (clockWidget.isTouchIn(x, y, z)) {
      commandsHandler->showAqiPage();
    } else if (currentWeatherWidget.isTouchIn(x, y, z)) {
      commandsHandler->showCurrentWeatherDetail();
    } else if (forecastCarouselWidget.isTouchIn(x, y, z)) {
      commandsHandler->showForecastPage();
    } else if (astronomyWidget.isTouchIn(x, y, z)) {
      commandsHandler->showAboutPage();
    } else {
      to_return = false;
    }
  }
  return to_return;
}
