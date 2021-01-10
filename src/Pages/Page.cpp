#include "Pages/Page.h"

Page::Page(CommandsHanler* commandsHandler) : commandsHandler(commandsHandler)
{
    for (uint8_t i = 0; i < MAX_WIDGETS; i++)
    {
        widgets[i] = nullptr;
    }
}

Page::~Page()
{
    setVisible(false);
}

void Page::addWidget(Widget *widget)
{
    Serial.print(F("Page::addWidget -> Free mem: "));
    Serial.println(ESP.getFreeHeap());
    uint8_t i = 0;
    while (widgets[i] != nullptr && i < MAX_WIDGETS)
        ++i;
    if (i < MAX_WIDGETS)
    {
        Serial.print(F("Adding widget to Page under index: "));
        Serial.println(i);
        widgets[i] = widget;
    }
    else
    {
        Serial.println(F("Error: no free space to add widget"));
    }
}

void Page::setVisible(boolean visible)
{
    if (this->isVisible != visible)
    {
        this->isVisible = visible;
        if (visible)
        {
            draw();
        }
        else
        {
            tft.fillScreen(TFT_BLACK);
        }
        for (uint8_t i = 0; i < MAX_WIDGETS; i++)
        {
            if (widgets[i] != nullptr)
            {
                widgets[i]->setVisible(visible);
            }
        }
    }
}

bool Page::handleTouchOn(uint16_t x, uint16_t y, uint16_t z)
{
    if (!this->isVisible)
        return false;
    for (uint8_t i = 0; i < MAX_WIDGETS; i++)
    {
        if (widgets[i] != nullptr && widgets[i]->isTouchIn(x, y, z))
        {
            if (widgets[i]->handleTouchOn(x, y, z))
                return true;
        }
    }
    return false;
}

bool Page::handleTouchOff(uint16_t x, uint16_t y, uint16_t z)
{
    if (!this->isVisible)
        return false;
    for (uint8_t i = 0; i < MAX_WIDGETS; i++)
    {
        if (widgets[i] != nullptr && widgets[i]->isTouchIn(x, y, z))
        {
            if (widgets[i]->handleTouchOff(x, y, z))
                return true;
        }
    }
    return false;
}

void Page::update(uint32_t now)
{
    if (!this->isVisible)
        return;
    for (uint8_t i = 0; i < MAX_WIDGETS; i++)
    {
        if (widgets[i] != nullptr)
        {
            widgets[i]->update(now);
        }
    }
}
