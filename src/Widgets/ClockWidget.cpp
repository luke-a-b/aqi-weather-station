#include "Widgets/ClockWidget.h"
#include "Config.h"

ClockWidget::ClockWidget(ClockModel *model, uint16_t y)
    : Widget(0, y, 240, 79), model(model) {}

void ClockWidget::update(uint32_t now) {
  if (isVisible && now - lastDrew > 1000) {
    lastDrew = now;
    static bool display = true;
    display = !display;
    setTexFormat(&DSEG7ClassicBold44, BC_DATUM,
                 display ? TFT_CLOCKGREEN : TFT_SILHOUTTEGREEN, TFT_TRANSPARENT,
                 0);
    uint8_t margin = model->isClock24hSytyle() ? 0 : 10;
    tft.setCursor(x + 135 - margin, y + 71);
    tft.print(":");
    time_t tnow = time(NULL);
    if (localtime(&tnow)->tm_sec == 0) {
      redraw();
    }
  }
}

void ClockWidget::redraw() {
  if (isVisible) {
    // tft.fillRect(x, y, width, height, TFT_BLUE);
    bool is24hStyle = model->isClock24hSytyle();
    time_t now = time(nullptr);
    struct tm *timeNow = localtime(&now);
    char time[8];

    // String time = model->getCurrentTime();
    uint8_t margin = is24hStyle ? 0 : 10;

    // draw current data
    setTexFormat(ArialBold14, BC_DATUM, TFT_WHITE, TFT_BLACK, width);
    String date = FPSTR(WEEK_DAYS_NAMES[timeNow->tm_wday]);
    date += " ";
    date += timeNow->tm_mday;
    date += " ";
    date += FPSTR(MONTHS_NAMES[timeNow->tm_mon]);
    date += String(timeNow->tm_year + 1900);

    tft.drawString(date, x + 135 - margin, y + 14);

    // draw AQI data
    drawBmpFromFile(model->getAqiLevelIconFileName(), x + 8, y + 19);
    if (model->isAqiDataValid()) {
      uint16_t color = tft.readPixel(30, 17 + 7);
      setTexFormat(ArialBold14, BC_DATUM, color, TFT_BLACK, 50);
      tft.drawString(String(model->getAqiPercentMax()) + "%", x + 8 + 26,
                     y + height);
    } else {
      setTexFormat(ArialBold14, BC_DATUM, TFT_RED, TFT_BLACK, 50);
      tft.drawString("--%", x + 8 + 26, y + height);
    }

    // draw clock
    setTexFormat(BL_DATUM, TFT_CLOCKGREEN, TFT_BLACK, width / 2);
    if (!is24hStyle) {
      tft.drawString(timeNow->tm_hour >= 12 ? "pm" : "am", x + 221 - margin,
                     y + 72);
    }
    tft.unloadFont();

    String Silhouette = is24hStyle ? "88:88" : "18:88";
    setTexFormat(&DSEG7ClassicBold44, BL_DATUM,
                 model->isClockSilhouetteEnabled() ? TFT_SILHOUTTEGREEN
                                                   : TFT_BLACK,
                 TFT_TRANSPARENT, 0);
    tft.setCursor(x + 211 - (38 * 4) - margin, y + 71);
    tft.print(Silhouette);

    sprintf(time, "%s%d:%02d", is24hStyle && timeNow->tm_hour < 10 ? "0" : "",
            is24hStyle ? timeNow->tm_hour : (timeNow->tm_hour + 11) % 12 + 1,
            timeNow->tm_min);
    tft.setTextColor(TFT_CLOCKGREEN);
    tft.setCursor(x + 211 - (38 * (strlen(time) - 1)) - margin, y + 71);
    tft.print(time);
  }
}
