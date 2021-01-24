#include "Pages/SplashScreenPage.h"
#include <LittleFS.h>
#include "Config.h"

SplashScreenPage::SplashScreenPage()
    : Page(nullptr),
      progressBar(10, 145, 220, 15, TFT_ORANGE, TFT_WHITE, TFT_BLUE) {
  addWidget(&progressBar);
}

void SplashScreenPage::setProgress(uint8_t percentage, String text) {
  // wait to mount filesystem
  if (percentage > 20) {
    tft.fillRect(30, 30, 180, 80, TFT_BLACK);
    static uint8_t logoNr = 0;
    String icon = FPSTR(LOGOS_ICON_PATH);
    icon.replace(F("{icon}"), String(logoNr));
    Widget::drawBmpFromFile(icon, 30, 30);
    logoNr = (logoNr + 1) == MAX_LOGOS ? 0 : logoNr + 1;
  }
  progressBar.setProgress(percentage, text);
}

void SplashScreenPage::setError(String text) { progressBar.setError(text); }

void SplashScreenPage::draw() {
  if (!isVisible)
    return;
  tft.fillScreen(TFT_BLACK);
  Widget::setTexFormat(ArialBold14, BC_DATUM, TFT_DARKGREY, TFT_BLACK,
                       tft.width());
  tft.drawString(FPSTR(INTL_ORGINAL), 120, 200);
  tft.drawString(FPSTR(INTL_ADAPTEDBY), 120, 220);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.unloadFont();
}

void SplashScreenPage::touchCalibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check if calibration file exists and size is correct
  if (LittleFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      LittleFS.remove(CALIBRATION_FILE);
    } else {
      File f = LittleFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.setCursor(20, 0);

    setProgress(40, FPSTR(INTL_TOUCH_CORNERS));

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println(F("Set REPEAT_CAL to false to stop this running again!"));
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    // store data
    File f = LittleFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
    // clear screan
    draw();

    setProgress(45, FPSTR(INTL_CALIBRATION_COMPLETE));
  }
  delay(1000);
}
