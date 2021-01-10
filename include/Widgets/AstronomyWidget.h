#ifndef _ASTRONOMY_WIDGET_H
#define _ASTRONOMY_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/AstronomyModel.h"

#define ASTRONOMY_UPDATE_INTERVAL 3600 // update every hour

class AstronomyWidget : public Widget
{
public:
    AstronomyWidget(AstronomyModel *model, uint16_t y);

protected:
    virtual void redraw();
    virtual void update(uint32_t now);
    AstronomyModel *model;
    int32_t lastUpdate = -ASTRONOMY_UPDATE_INTERVAL * 1000;
};

#endif