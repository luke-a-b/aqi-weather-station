#ifndef _ASTRONOMY_WIDGET_H
#define _ASTRONOMY_WIDGET_H

#include "Widgets/Widget.h"
#include "Models/AstronomyModel.h"

class AstronomyWidget : public Widget {
public:
  AstronomyWidget(AstronomyModel *model, uint16_t y);
  void update();

protected:
  virtual void redraw();
  AstronomyModel *model;
};

#endif