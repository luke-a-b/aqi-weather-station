#ifndef _WIFI_STATUS_WIDGET_H
#define _WIFI_STATUS_WIDGET_H

#include "Widgets/Widget.h"

class WifiStatusWidget: public Widget
{
public:
    WifiStatusWidget(int32_t x, int32_t y);
    virtual void setVisible(boolean visible);
protected:
    int previousLevel = -1;
    uint32_t lastDrew = -60 * 1000;
    virtual void redraw();
    virtual void update(uint32_t now);
};

#endif
