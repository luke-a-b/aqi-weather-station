#ifndef _SPLASH_SCREEN_PAGE_H
#define _SPLASH_SCREEN_PAGE_H

// This is the file name used to store the calibration data
// You can change this to create new calibration files.
// The file name must start with "/".
#define CALIBRATION_FILE "/TouchCalData1"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

#include "Pages/Page.h"
#include "Widgets/ProgressBarWidget.h"

class SplashScreenPage : public Page
{
public:
    SplashScreenPage();
    void setProgress(uint8_t percentage, String text);
    void setError(String text);
    void touchCalibrate();

protected:
    virtual void draw();

private:
    ProgressBarWidget progressBar;
};

#endif
