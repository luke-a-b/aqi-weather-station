#include "Pages/AboutPage.h"
#include <ESP8266WiFi.h>
#include "assert.h"

ADC_MODE(ADC_VCC);


AboutPage::AboutPage(CommandsHanler* commandsHandler) : Page(commandsHandler)
{
    assert(commandsHandler != nullptr);
}

bool AboutPage::handleTouchOff(uint16_t x, uint16_t y, uint16_t z)
{
    if (!this->isVisible) return false;    
    commandsHandler->showMainPage();
    return true;
}


void AboutPage::draw()
{
    if (!isVisible) return;
    Widget::setTexFormat(ARIAL_BOLD_14, BC_DATUM, TFT_WHITE, TFT_BLACK, 0);

    drawLabelValue(6, "Heap Mem:", String(ESP.getFreeHeap() / 1024) + "kb");
    drawLabelValue(7, "Flash Mem:", String(ESP.getFlashChipRealSize() / 1024 / 1024) + "MB");
    drawLabelValue(8, "IP address:", WiFi.localIP().toString());
    drawLabelValue(9, "WiFi Strength:", String(WiFi.RSSI()) + "dB");
    drawLabelValue(10, "Chip ID:", String(ESP.getChipId()));
    drawLabelValue(11, "VCC: ", String(ESP.getVcc() / 1024.0) + "V");
    drawLabelValue(12, "CPU Freq.: ", String(ESP.getCpuFreqMHz()) + "MHz");
    char time_str[15];
    const uint32_t millis_in_day = 1000 * 60 * 60 * 24;
    const uint32_t millis_in_hour = 1000 * 60 * 60;
    const uint32_t millis_in_minute = 1000 * 60;
    uint8_t days = millis() / (millis_in_day);
    uint8_t hours = (millis() - (days * millis_in_day)) / millis_in_hour;
    uint8_t minutes = (millis() - (days * millis_in_day) - (hours * millis_in_hour)) / millis_in_minute;
    sprintf(time_str, "%2dd%2dh%2dm", days, hours, minutes);
    drawLabelValue(13, "Uptime: ", time_str);

    tft.setTextDatum(BL_DATUM);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString("Last Reset: ", 15, 250);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(ESP.getResetInfo(), 15, 265);

    tft.unloadFont();
}

void AboutPage::drawLabelValue(uint8_t line, String label, String value)
{
    const uint8_t labelX = 15;
    const uint8_t valueX = 150;
    tft.setTextDatum(BL_DATUM);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawString(label, labelX, 30 + line * 15);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(value, valueX, 30 + line * 15);
}
