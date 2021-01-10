#ifndef _CLOCK_WIDGET_H
#define _CLOCK_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/ClockModel.h"

// Custom color
#define TFT_CLOCKGREEN 0x7FE5
#define TFT_SILHOUTTEGREEN 0x01E0

class ClockWidget : public Widget
{
public:
    ClockWidget(ClockModel *model, uint16_t y);

protected:
    uint32_t lastDrew;
    ClockModel *model;
    virtual void redraw();
    virtual void update(uint32_t now);
};

#endif