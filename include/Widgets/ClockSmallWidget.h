#ifndef _CLOCK_SMALL_WIDGET_H
#define _CLOCK_SMALL_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/ClockModel.h"

class ClockSmallWidget : public Widget
{
public:
    ClockSmallWidget(ClockModel *model, uint16_t x, uint16_t y, std::function<void(uint16_t, uint16_t, uint16_t, uint16_t)> eraseHandler);
    virtual void setVisible(boolean visible);

protected:
    //uint32_t lastDrew;
    uint8_t lastDrewSec;
    uint8_t lastDrewMin;
    boolean initialDraw = true;
    ClockModel *model;
    std::function<void(uint16_t, uint16_t, uint16_t, uint16_t)> eraseHandler;
    virtual void redraw();
    virtual void update(uint32_t now);
    int16_t xcolon = 0;
};

#endif