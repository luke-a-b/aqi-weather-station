#include "Pages/SplashScreenPage.h"
#include <LittleFS.h>

SplashScreenPage::SplashScreenPage() : Page(nullptr), progressBar(10, 220, 220, 15, TFT_ORANGE, TFT_WHITE, TFT_BLUE)
{
    addWidget(&progressBar);
}

void SplashScreenPage::setProgress(uint8_t percentage, String text)
{
    progressBar.setProgress(percentage, text);
}

void SplashScreenPage::setError(String text)
{
    progressBar.setError(text);
}

void SplashScreenPage::draw()
{
    if (!isVisible)
        return;
    tft.fillScreen(TFT_BLACK);

    //tft.setFreeFont(&ArialRoundedMTBold_14);
    Widget::setTexFormat(ARIAL_BOLD_14, BC_DATUM, TFT_DARKGREY, TFT_BLACK, tft.width());
    tft.drawString(F("Original by: blog.squix.org"), 120, 260);
    tft.drawString(F("Adapted by: luke-a-b"), 120, 280);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.unloadFont();
}


void SplashScreenPage::touchCalibrate()
{
    uint16_t calData[5];
    uint8_t calDataOK = 0;

    // check if calibration file exists and size is correct
    if (LittleFS.exists(CALIBRATION_FILE))
    {
        if (REPEAT_CAL)
        {
            // Delete if we want to re-calibrate
            LittleFS.remove(CALIBRATION_FILE);
        }
        else
        {
            File f = LittleFS.open(CALIBRATION_FILE, "r");
            if (f)
            {
                if (f.readBytes((char *)calData, 14) == 14)
                    calDataOK = 1;
                f.close();
            }
        }
    }

    if (calDataOK && !REPEAT_CAL)
    {
        // calibration data valid
        tft.setTouch(calData);
    }
    else
    {
        // data not valid so recalibrate
        tft.setCursor(20, 0);

        setProgress(40, F("Touch corners as indicated"));

        tft.setTextFont(1);
        tft.println();

        if (REPEAT_CAL)
        {
            tft.setTextColor(TFT_RED, TFT_BLACK);
            tft.println(F("Set REPEAT_CAL to false to stop this running again!"));
        }

        tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

        // store data
        File f = LittleFS.open(CALIBRATION_FILE, "w");
        if (f)
        {
            f.write((const unsigned char *)calData, 14);
            f.close();
        }
        // clear screan
        tft.fillScreen(TFT_BLACK);

        setProgress(45, F("Calibration complete!"));
        delay(1000);
    }
}

