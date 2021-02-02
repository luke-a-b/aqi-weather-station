#include "Widgets/CurrentWeatherWidget.h"
#include "Config.h"

CurrentWeatherWidget::CurrentWeatherWidget(CurrentWeatherModel *model,
                                           uint16_t y)
    : Widget(0, y, 240, 93), model(model) {}

void CurrentWeatherWidget::update() {
  if (lastCurrentTemp != model->getCurrentWeatherTemp() ||
      lastObservationTime != model->getObservationTime()) {
    redraw();
  } else if (!isnan(lastLocalTemp) && lastLocalTemp != model->getLocalTemp()) {
    redrawLocalTemp();
  }
}

void CurrentWeatherWidget::redrawLocalTemp() {
  if (isVisible && model->isLocalTempSensorEnabled()) {
    String degree = model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS)
                                              : FPSTR(CONST_UNIT_FAHRENHEIT);
    uint8_t margin = x + width - 10;
    uint8_t temp_margin = margin - 35;

    setTexFormat(ArialBold14, BR_DATUM, TFT_ORANGE, TFT_BLACK, 0);
    lastLocalTemp = model->getLocalTemp();
    String tempStr;
    if (isnan(lastLocalTemp)) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tempStr = "--" + degree;
    } else {
      tempStr = String(lastLocalTemp, 0) + degree;
    }
    tft.drawString(tempStr, temp_margin, y + 14 + 37 + 14);

    float humiditiy = model->getLocalHumidity();
    String humiditiyStr;
    if (isnan(humiditiy)) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      humiditiyStr = "--%";
    } else {
      tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
      humiditiyStr = String(humiditiy, 0) + "%";
    }
    tft.drawString(humiditiyStr, margin, y + 14 + 37 + 14);
    tft.unloadFont();
  }
}

void CurrentWeatherWidget::redraw() {
  if (isVisible) {
    lastObservationTime = model->getObservationTime();

    String degree = model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS)
                                              : FPSTR(CONST_UNIT_FAHRENHEIT);
    uint8_t margin = x + width - 10;
    uint8_t temp_margin = margin - 35;

    tft.fillRect(x, y, width, height, TFT_BLACK);

    drawBmpFromFile(model->getCurrentWeatherIconFileName(), x + 4, y);

    setTexFormat(ArialBold14, BR_DATUM, TFT_ORANGE, TFT_BLACK, 0);
    tft.drawString(model->getCurrentWeatherCityName(), margin, y + 14);
    tft.drawString(model->getCurrentWeatherDescription(), margin,
                   y + 14 + 37 + 14 + 14 + 14);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(String(model->getCurrentWeatherPressure()) + "hPa", margin,
                   y + 14 + 37 + 14 + 14);

    tft.setTextColor(TFT_SKYBLUE, TFT_BLACK);
    tft.drawString(String(model->getCurrentWeatherHumidity()) + "%", margin,
                   y + 14 + 33);

    lastCurrentTemp = model->getCurrentWeatherTemp();
    String temp = String(lastCurrentTemp, 1);
    int point = temp.indexOf('.');
    String decimal = "";
    if (point > 0) {
      decimal = temp.substring(point, point + 2);
      temp = temp.substring(0, point);
    }

    if (decimal != "") {
      tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
      tft.drawString(decimal, temp_margin - 30, y + 14 + 33);
    }

    setTexFormat(ArialBold38, BR_DATUM, TFT_WHITE, TFT_TRANSPARENT, 0);
    tft.drawString(temp + degree, temp_margin /* - 50*/, y + 14 + 39);

    tft.unloadFont();

    redrawLocalTemp();
  }
}