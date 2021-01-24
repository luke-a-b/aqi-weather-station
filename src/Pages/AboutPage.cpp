#include "Pages/AboutPage.h"
#include <ESP8266WiFi.h>
#include "Config.h"
#include "assert.h"

ADC_MODE(ADC_VCC);

AboutPage::AboutPage(CommandsHanler *commandsHandler) : Page(commandsHandler) {
  assert(commandsHandler != nullptr);
}

void AboutPage::update(uint32_t now) {
  if (this->isVisible) {
    if (long(now - lastUpdate) > TIME_LOGO_UPDATE * 1000) {
      lastUpdate = now;
      tft.fillRect(30, 10, 180, 80, TFT_BLACK);
      static uint8_t logoNr = 0;
      String icon = FPSTR(LOGOS_ICON_PATH);
      icon.replace(F("{icon}"), String(logoNr));
      Widget::drawBmpFromFile(icon, 30, 10);
      logoNr = (logoNr + 1) == MAX_LOGOS ? 0 : logoNr + 1;
    }
    Page::update(now);
  }
}

bool AboutPage::handleTouchOff(uint16_t x, uint16_t y, uint16_t z) {
  if (!this->isVisible)
    return false;
  commandsHandler->showMainPage();
  return true;
}

void AboutPage::draw() {
  if (!isVisible)
    return;

  Widget::setTexFormat(ArialBold14, TC_DATUM, TFT_DARKGREY, TFT_BLACK,
                       tft.width());
  tft.drawString(FPSTR(INTL_ORGINAL), 120, 95);
  tft.drawString(FPSTR(INTL_ADAPTEDBY), 120, 115);

  char time_str[15];
  const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
  const uint32_t millis_in_hour = 1000 * 60 * 60;
  const uint32_t millis_in_minute = 1000 * 60;
  uint32_t now = millis();
  uint8_t days = now / (millis_in_day);
  uint8_t hours = (now - (days * millis_in_day)) / millis_in_hour;
  uint8_t minutes = (now - (days * millis_in_day) - (hours * millis_in_hour)) /
                    millis_in_minute;
  sprintf(time_str, "%2dd%2dh%2dm", days, hours, minutes);

  drawLabelValue(9, 10, 150, FPSTR(INTL_HEAP_MEM),
                 String(ESP.getFreeHeap() / 1024) + "kb");
  drawLabelValue(10, 10, 150, FPSTR(INTL_FLASH_MEM),
                 String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
  drawLabelValue(11, 10, 150, FPSTR(INTL_IP_ADDRESS),
                 WiFi.localIP().toString());
  drawLabelValue(12, 10, 150, FPSTR(INTL_RSSI),
                 String(WiFi.RSSI()) + String(FPSTR(CONST_UNIT_DBM)));
  drawLabelValue(13, 10, 150, FPSTR(INTL_CHIP_ID), String(ESP.getChipId()));
  drawLabelValue(14, 10, 150, FPSTR(INTL_VCC),
                 String(ESP.getVcc() / 1024.0) + "V");
  drawLabelValue(15, 10, 150, FPSTR(INTL_CPU_FREQ),
                 String(ESP.getCpuFreqMHz()) + "MHz");
  drawLabelValue(16, 10, 150, FPSTR(INTL_UPTIME), time_str);

  drawLabelValue(17, 10, 150, FPSTR(INTL_LAST_RESET), "");
  drawLabelValue(18, 10, 25, "", ESP.getResetInfo());

  tft.unloadFont();
}
