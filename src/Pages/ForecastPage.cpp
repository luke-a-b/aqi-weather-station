#include "Pages/ForecastPage.h"
#include "Config.h"
#include "Bitmaps/IconHumiditySmall.h"
#include "Bitmaps/IconPressureSmall.h"
#include "Bitmaps/IconTempSmall.h"
#include "Bitmaps/IconWindSmall.h"

ForecastPage::ForecastPage(ForecastModel *model,
                           CommandsHanler *commandsHandler)
    : Page(commandsHandler), model(model) {}

bool ForecastPage::handleTouchOff(uint16_t x, uint16_t y, uint16_t z) {
  if (this->isVisible) {
    if (++currentPage == MAX_FORECAST_PAGES) {
      currentPage = 0;
      commandsHandler->showMainPage();
    } else {
      draw();
    }
    return true;
  }
  return false;
}

void ForecastPage::update(uint32_t now) {
  if (this->isVisible) {
    /*if (lastObservationTime != model->getObservationTime())
    {
        tft.fillScreen(TFT_BLACK);
        draw();
    }*/
  }
  Page::update(now);
}

void ForecastPage::draw() {
  if (isVisible) {
    tft.fillScreen(TFT_BLACK);
    Widget::setTexFormat(ArialBold14, TC_DATUM, TFT_WHITE, TFT_BLACK,
                         tft.width());
    String forecasts = FPSTR(INTL_FORECASTS);
    forecasts +=
        " (" + String(currentPage + 1) + "/" + String(MAX_FORECAST_PAGES) + ")";
    tft.drawString(forecasts, tft.width() / 2, 0);

    String degree = model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS)
                                              : FPSTR(CONST_UNIT_FAHRENHEIT);
    String speed = model->isMetricSelected() ? FPSTR(CONST_UNIT_MPS)
                                             : FPSTR(CONST_UNIT_MPH);

    uint16 x = 5;
    uint16 y;

    uint8_t startDay = currentPage * 4;
    for (uint8_t i = 0; i < 4; i++) {
      uint8_t day = startDay + i;
      y = 17 + i * 75;

      tft.drawFastHLine(10, y, 220, TFT_DARKGREY);

      tft.setTextDatum(TR_DATUM);
      tft.setTextColor(TFT_ORANGE, TFT_BLACK);
      struct tm *timeNow = model->getForecastTime(day);
      tft.drawString(String(FPSTR(WEEK_DAYS_NAMES_FULL[timeNow->tm_wday])),
                     tft.width() - 5, y + 4);

      tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
      tft.drawString(model->getForecastDescription(day), tft.width() - 5,
                     y + 19);

      tft.setTextDatum(TL_DATUM);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString(String(model->getForecastTempMin(day), 0) + "/" +
                         String(model->getForecastTempMax(day), 0) + degree,
                     87, y + 37);
      Widget::drawBitmap(73, y + 34, &icontempsmall);
      tft.drawString(String(model->getForecastHumidity(day), 0) + "%", 175,
                     y + 37);
      Widget::drawBitmap(158, y + 34, &iconhumiditysmall);
      tft.drawString(String(model->getForecastPressure(day), 0) + "hPa", 87,
                     y + 57);
      Widget::drawBitmap(70, y + 54, &iconpressuresmall);
      tft.drawString(String(model->getForecastWindSpeed(day), 0) + speed + " " +
                         model->getForecastWindDirection(day),
                     175, y + 57);
      Widget::drawBitmap(158, y + 54, &iconwindsmall);

      Widget::drawBmpFromFile(model->getForecastIconFileName(day), x, y + 19);
    }

    tft.unloadFont();
  }
}
