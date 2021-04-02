#include <LittleFS.h>

#include <TFT_eSPI.h>

#include "WebConfig.h"
#include "Backlight.h"
#include "WebConfig.h"
#include "Fetcher.h"
#ifdef DEBUG_ON
#include "FsSupport.h"
#include "MemChecker.h"
#endif
#include "PagesController.h"
#include "TouchInterface.h"
#include "CommandsServer.h"

#include "Fonts/Fonts.h"

TFT_eSPI tft;
Backlight backlight;
Config config;
WebConfig webConfig(&config);
DisplayModel model(&config);
PagesController pagesCtrl(&backlight, &model);
TouchInterface touchInterface(&tft, &pagesCtrl);
Fetcher fetcher(&config, &model, &pagesCtrl);
CommandsServer cmdServer(&pagesCtrl, &model);

#ifdef DEBUG_ON
MemChecker memChecker;
#endif

SplashScreenPage *splashScreen;

bool connected = false;

void wifiConnected(IPAddress localIp);
void connectAp(const char *apName, const char *password);
void connectWifi(const char *ssid);

void setup() {
  Serial.begin(115200);
  DEBUG_PRINTFLN("Initializing LCD...");
  backlight.setBacklightTimeout(0);
  tft.begin();
  tft.setRotation(0);

  splashScreen = new SplashScreenPage();
  splashScreen->setVisible(true);

  DEBUG_PRINTLN(FPSTR(INTL_MOUNTING_FILE_SYSTEM));
  splashScreen->setProgress(10, FPSTR(INTL_MOUNTING_FILE_SYSTEM));
  bool isFSMounted = LittleFS.begin();
  // Uncomment if you want to erase File System and update all internet
  // resources, this takes some time!
  // isFSMounted = false;
  if (!isFSMounted) {
    DEBUG_PRINTLN(FPSTR(INTL_FORMATTING_FILE_SYSTEM));
    splashScreen->setProgress(20, FPSTR(INTL_FORMATTING_FILE_SYSTEM));
    LittleFS.format();
  }
#ifdef DEBUG_ON
  FsSupport::listFiles();
#endif
  // for (uint8_t i = 0; i < FILESYSTEM_FONTS_MAX; i++)
  // {
  //     if (LittleFS.exists("/" + FilesystemFonts[i] + ".vlw") == false)
  //     {
  //         // splashScreen->setError("Error: Font missing in Flash FS!");
  //         while (1) yield();
  //     }
  // }

  splashScreen->setProgress(30, FPSTR(INTL_SETTING_TOUCH_CALIBRATION));
  splashScreen->touchCalibrate();

  splashScreen->setProgress(40, FPSTR(INTL_PREPARING_OBJECTS));
  delay(2000);

  webConfig.begin(&wifiConnected, &connectAp, &connectWifi);

  for (uint8_t i = 0; i < MAX_AQI_STATIONS; i++) {
    char *url = config.getAqiStationUrl(i);
    model.getAqiDataModelAt(i)->stationUrl = url;
  }
}

void loop() {
  uint32_t now = millis();
  webConfig.update();
  backlight.update(now);
  if (connected) {
    touchInterface.update();
    fetcher.update(now);
    pagesCtrl.update(now);
    cmdServer.update();
  }
#ifdef DEBUG_ON
  memChecker.update(now);
#endif
}

void wifiConnected(IPAddress localIp) {
  if (connected) {
    return;
  }
  splashScreen->setProgress(60,
                            String(FPSTR(INTL_CONNECTED)) + localIp.toString());
  delay(2000);

  splashScreen->setProgress(70, FPSTR(INTL_UPDATING_DATA));

  fetcher.updateAll();

  splashScreen->setProgress(100, FPSTR(INTL_DONE));
  delay(2000);

  splashScreen->setVisible(false);
  delete splashScreen;
  splashScreen = nullptr;

  pagesCtrl.showMainPage();
  backlight.setBacklightTimeout(config.getBacklightTimeout());
  connected = true;
}

void connectAp(const char *apName, const char *password) {
  if (splashScreen == nullptr)
    splashScreen = new SplashScreenPage();
  if (connected) {
    connected = false;
    pagesCtrl.hideCurrentPage();
    splashScreen->setVisible(true);
  }
  backlight.setBacklightTimeout(0);
  splashScreen->setError(FPSTR(INTL_NO_CONNECTION));
}

void connectWifi(const char *ssid) {
  if (splashScreen == nullptr)
    splashScreen = new SplashScreenPage();
  if (connected) {
    connected = false;
    pagesCtrl.hideCurrentPage();
    splashScreen->setVisible(true);
  }
  backlight.setBacklightTimeout(0);
  splashScreen->setProgress(50,
                            String(FPSTR(INTL_CONNECTING_TO)) + String(ssid));
}
