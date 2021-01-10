#include <LittleFS.h>

#include <TFT_eSPI.h>

#include "WebConfig.h"
#include "Backlight.h"
#include "WebConfig.h"
#include "Fetcher.h"
#include "FsSupport.h"
#include "PagesController.h"
#include "TouchInterface.h"

#include "Fonts/Fonts.h"

#include "MemChecker.h"

TFT_eSPI tft;
Backlight backlight;
Config config;
WebConfig webConfig(&config);
DisplayModel model(&config);
PagesController pagesCtrl(&backlight, &model);
TouchInterface touchInterface(&tft, &pagesCtrl);
Fetcher fetcher(&config, &model, &pagesCtrl);

MemChecker memChecker;

SplashScreenPage *splashScreen;

bool connected = false;

void wifiConnected(IPAddress localIp);
void connectAp(const char *apName, const char *password);
void connectWifi(const char *ssid);

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Initializing LCD..."));
    tft.begin();
    tft.setRotation(0);

    splashScreen = new SplashScreenPage();
    splashScreen->setVisible(true);

    Serial.println(F("Mounting file system..."));
    splashScreen->setProgress(10, F("Mounting file system"));
    bool isFSMounted = LittleFS.begin();
    delay(1000);
    //Uncomment if you want to erase File System and update all internet resources, this takes some time!
    //isFSMounted = false;
    if (!isFSMounted)
    {
        Serial.println(F("Formatting file system..."));
        splashScreen->setProgress(20, F("Formatting file system"));
        LittleFS.format();
    }
    //FsSupport::listFiles();

    // for (uint8_t i = 0; i < FILESYSTEM_FONTS_MAX; i++)
    // {
    //     if (LittleFS.exists("/" + FilesystemFonts[i] + ".vlw") == false)
    //     {
    //         // splashScreen->setError("Error: Font missing in Flash FS!");
    //         while (1) yield();
    //     }
    // }

    splashScreen->setProgress(30, F("Setting touch calibrations"));
    splashScreen->touchCalibrate();

    splashScreen->setProgress(40, F("Preparing ojects"));

    webConfig.begin(&wifiConnected, &connectAp, &connectWifi);

    fetcher.begin();

    for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++)
    {
        String url = config.getAqiStationUrl(i);
        model.getAqiDataModelAt(i)->stationUrl = url;

        //AqiDataSource::parseUrl(url, model.getAqiDataModelAt(i));
        delay(10);
    }
}


void loop()
{
    uint32_t now = millis();
    webConfig.update();
    backlight.update(now);
    if (connected)
    {
        touchInterface.update();
        fetcher.update(now);
        pagesCtrl.update(now);
    }
    memChecker.update(now);
}

void wifiConnected(IPAddress localIp)
{
    if (connected)
    {
        return;
    }
    //cmdServer = new CmdServer(backlight);
    splashScreen->setProgress(60, String(F("Connected: ")) + localIp.toString());
    delay(1000);

    splashScreen->setProgress(70, F("Updating data..."));

    fetcher.update(millis());

    splashScreen->setProgress(100, F("Done"));
    splashScreen->setVisible(false);
    delete splashScreen;
    splashScreen = nullptr;

    pagesCtrl.showMainPage();
    backlight.setBacklightTimeout(config.getBacklightTimeout());
    connected = true;
}

void connectAp(const char *apName, const char *password)
{
    if (splashScreen == nullptr) splashScreen = new SplashScreenPage();
    if (connected)
    {
        connected = false;
        pagesCtrl.hideCurrentPage();
        splashScreen->setVisible(true);
    }
    backlight.setBacklightTimeout(0);
    splashScreen->setError(F("No connection - check config!"));
}

void connectWifi(const char *ssid)
{
    if (splashScreen == nullptr) splashScreen = new SplashScreenPage();
    if (connected)
    {
        connected = false;
        pagesCtrl.hideCurrentPage();
        splashScreen->setVisible(true);
    }
    backlight.setBacklightTimeout(0);
    splashScreen->setProgress(50, String(F("Connecting to ")) + String(ssid));
}

