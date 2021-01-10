#ifndef _PAGE_H
#define _PAGE_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Widgets/Widget.h"

#include "Models/ModelChangeObserver.h"
#include "CommandsHanler.h"

#define MAX_WIDGETS 16

class Page : public ModelChangeObserver
{
public:
    Page(CommandsHanler* commandsHandler);
    virtual ~Page();
    virtual void setVisible(boolean visible);
    void addWidget(Widget *widget);
    virtual void update(uint32_t now);
    virtual bool handleTouchOn(uint16_t x, uint16_t y, uint16_t z);
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
    virtual bool handleSwipeRight() { return false; }
    virtual bool handleSwipeLeft()  { return false; }
    virtual bool handleSwipeDown()  { return false; }
    virtual bool handleSwipeUp()    { return false; }

public: // implements ModelChangeObserver
    virtual void notifyAqiUpdateStart() {}
    virtual void notifyAqiUpdated(boolean status) {}
    virtual void notifyWeatherUpdateStart() {}
    virtual void notifyWeatherUpdated(boolean status) {}
    virtual void notifyLocalTempUpdated(boolean status) {}

protected:
    virtual void draw() {}
    boolean isVisible = false;
    Widget *widgets[MAX_WIDGETS];
    CommandsHanler* commandsHandler;
};

#endif
