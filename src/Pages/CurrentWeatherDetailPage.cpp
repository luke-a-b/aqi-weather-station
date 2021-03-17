#include "Pages/CurrentWeatherDetailPage.h"
#include "Config.h"

CurrentWeatherDetailPage::CurrentWeatherDetailPage(
    CurrentWeatherModel *model, CommandsHanler *commandsHandler)
    : Page(commandsHandler), model(model), currentWeatherWidget(model, 5) {
  addWidget(&currentWeatherWidget);
}

bool CurrentWeatherDetailPage::handleTouchOff(uint16_t x, uint16_t y,
                                              uint16_t z) {
  if (!this->isVisible)
    return false;
  commandsHandler->showMainPage();
  return true;
}

void CurrentWeatherDetailPage::notifyWeatherUpdated() {
  if (this->isVisible) {
    if (lastObservationTime != model->getObservationTime()) {
      setVisible(false);
      setVisible(true);
    }
  }
}

void CurrentWeatherDetailPage::draw() {
  if (this->isVisible) {
    lastObservationTime = model->getObservationTime();
    tft.drawFastHLine(10, 100, 220, TFT_DARKGREY);
    Widget::setTexFormat(ArialBold14, TC_DATUM, TFT_WHITE, TFT_BLACK,
                         tft.width());

    tft.drawString(FPSTR(INTL_CURRENT_CONDITIONS), tft.width() / 2, 110);

    String degree = model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS)
                                              : FPSTR(CONST_UNIT_FAHRENHEIT);
    String speed = model->isMetricSelected() ? FPSTR(CONST_UNIT_MPS)
                                             : FPSTR(CONST_UNIT_MPH);
    String fall = model->isMetricSelected() ? FPSTR(CONST_UNIT_MM)
                                            : FPSTR(CONST_UNIT_INCH);

    drawLabelValue(9, 10, 150, FPSTR(INTL_TEMPERATURE_MINMAX),
                   String(model->getTodayMinTemp(), 0) + "/" +
                       String(model->getTodayMaxTemp(), 0) + degree);
    drawLabelValue(10, 10, 150, FPSTR(INTL_TEMPERATURE_FEELSLIKE),
                   String(model->getCurrentFeelsLikeTemp(), 0) + degree);
    drawLabelValue(11, 10, 150, FPSTR(INTL_TEMPERATURE_DEW_POINT),
                   String(model->getCurrentDewPoint(), 0) + degree);
    drawLabelValue(12, 10, 150, FPSTR(INTL_CLOUDS),
                   String(model->getCurrentClouds()) + "%");
    drawLabelValue(13, 10, 150, FPSTR(INTL_UVI),
                   String(model->getCurrentUvi()));
    drawLabelValue(14, 10, 150, FPSTR(INTL_VISIBILITY),
                   String(model->getCurrentVisiblity()) + "km");
    drawLabelValue(15, 10, 150, FPSTR(INTL_WIND_SPEED),
                   String(model->getCurrentWindSpeed(), 1) + "(" +
                       String(model->getCurrentWindGust(), 1) + ")" + speed);
    drawLabelValue(16, 10, 150, FPSTR(INTL_WIND_DEGREE),
                   String(model->getCurrentWindDeg()) + "°");
    drawLabelValue(17, 10, 150, FPSTR(INTL_RAIN_SNOW),
                   String(model->getCurrentRain(), 0) + "/" +
                       String(model->getCurrentSnow(), 0) + fall);
    drawLabelValue(18, 10, 150, FPSTR(INTL_AIR_QUALITY), "");
    drawLabelValue(19, 10, 25, "", model->getCurrentAirQuality());

    tft.unloadFont();
  }
}
