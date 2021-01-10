#include "Widgets/WifiStatusWidget.h"
#include <ESP8266WiFi.h>

WifiStatusWidget::WifiStatusWidget(int32_t x, int32_t y) : Widget(x, y, 25, 12)
{
}

void WifiStatusWidget::setVisible(boolean visible)
{
    if (this->isVisible != visible) this->previousLevel = -1;
    Widget::setVisible(visible);
}

void WifiStatusWidget::update(uint32_t now)
{
    if (isVisible && now - lastDrew > 1000)
    {
        lastDrew = now;
        redraw();
    }
}

void WifiStatusWidget::redraw()
{
    float rssi = WiFi.RSSI();
    int level = map(constrain(rssi, -100, -50), -100, -50, 0, 4);
    if (level != this->previousLevel)
    {
        //tft.fillRect(x, y, width, height, TFT_BLACK);
        for (int i = 0; i < 5; i++)
        {
            uint16_t color = i <= level ? TFT_WHITE : TFT_DARKGREY;
            tft.fillRect(x + i * 4 + 3 , y - i * 2 + height - 3, 3, 3 + i * 2, color);
        }
    }
    this->previousLevel = level;
}