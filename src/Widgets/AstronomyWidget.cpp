#include "Widgets/AstronomyWidget.h"
#include "Config.h"

AstronomyWidget::AstronomyWidget(AstronomyModel *model, uint16_t y)
    : Widget(0, y, 240, 60), model(model) {}

void AstronomyWidget::update() { redraw(); }

void AstronomyWidget::redraw() {
  if (isVisible) {
    uint8_t center = width / 2;
    drawBmpFromFile(model->getMoonIconFileName(), x + center - 21, y + 1);

    setTexFormat(ArialBold14, BL_DATUM, TFT_ORANGE, TFT_BLACK, center - 31);
    tft.drawString(FPSTR(INTL_SUN), x + 10, y + 15);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(FPSTR(INTL_MOON), x + width - 10, y + 15);
    tft.setTextDatum(BC_DATUM);
    tft.drawString(FPSTR(MOON_PHASES[model->getMoonPhase()]), x + width / 2,
                   y + height);

    tft.setTextDatum(BL_DATUM);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    char time[8];
    bool is24hStyle = model->isClock24hSytyle();

    struct tm *timeinfo = model->getSunriseTime();
    char period[3];
    sprintf(period, "%s",
            is24hStyle ? "" : (timeinfo->tm_hour >= 12 ? "pm" : "am"));
    sprintf(time, "%s%d:%02d%s",
            is24hStyle && timeinfo->tm_hour < 10 ? "0" : "",
            is24hStyle ? timeinfo->tm_hour : (timeinfo->tm_hour + 11) % 12 + 1,
            timeinfo->tm_min, period);
    tft.drawString(time, x + 10, y + 30);

    timeinfo = model->getSunsetTime();
    sprintf(time, "%s%d:%02d%s",
            is24hStyle && timeinfo->tm_hour < 10 ? "0" : "",
            is24hStyle ? timeinfo->tm_hour : (timeinfo->tm_hour + 11) % 12 + 1,
            timeinfo->tm_min, period);
    tft.drawString(time, x + 10, y + 45);

    tft.setTextDatum(BR_DATUM);

    timeinfo = model->getMoonriseTime();
    sprintf(time, "%s%d:%02d%s",
            is24hStyle && timeinfo->tm_hour < 10 ? "0" : "",
            is24hStyle ? timeinfo->tm_hour : (timeinfo->tm_hour + 11) % 12 + 1,
            timeinfo->tm_min, period);
    tft.drawString(time, x + width - 10, y + 30);

    timeinfo = model->getMoonsetTime();
    sprintf(time, "%s%d:%02d%s",
            is24hStyle && timeinfo->tm_hour < 10 ? "0" : "",
            is24hStyle ? timeinfo->tm_hour : (timeinfo->tm_hour + 11) % 12 + 1,
            timeinfo->tm_min, period);
    tft.drawString(time, x + width - 10, y + 45);

    tft.unloadFont();
  }
}
