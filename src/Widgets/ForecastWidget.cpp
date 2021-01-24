#include "Widgets/ForecastWidget.h"
#include "Config.h"

ForecastWidget::ForecastWidget(ForecastModel *model, uint16_t x, uint16_t y,
                               uint8_t dayIndex)
    : Widget(x, y, 240, 72), model(model), dayIndex(dayIndex) {}

void ForecastWidget::redraw() {
  if (isVisible) {
    // tft.fillRect(x, y, width, height, TFT_BLUE);

    tft.loadFont(ArialBold14);
    Widget::setTexFormat(ArialBold14, TL_DATUM, TFT_WHITE, TFT_BLACK, width);
    tft.drawString(model->getForecastDescription(dayIndex), x + 5, y + 15);

    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    // tft.drawString(model->getForecastWeekDayName(dayIndex), x + 5, y);
    /*String tempString = String(model->getForecastTemp(dayIndex), 0);
    tempString += model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS) :
    FPSTR(CONST_UNIT_FAHRENHEIT);

    tft.drawString(tempString, width / 2, 13 + 13);*/

    tft.unloadFont();

    drawBmpFromFile(model->getForecastIconFileName(dayIndex), x + 5, y + 30);
  }
}
