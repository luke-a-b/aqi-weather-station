#ifndef _AQI_PAGE_H
#define _AQI_PAGE_H

#include "Page.h"
#include "Models/AqiDisplayModel.h"
#include "Models/ClockModel.h"
#include "Widgets/ClockSmallWidget.h"
#include "Widgets/WifiStatusWidget.h"

class AqiPage : public Page {
public:
  AqiPage(AqiDisplayModel *model, ClockModel *clock,
          CommandsHandler *commandsHandler);

  void setModel(AqiDisplayModel *model);
  virtual void setVisible(boolean visible);

protected:
  virtual void draw();
  virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
  virtual void notifyAqiUpdateStart(AqiDisplayModel *model);
  virtual void notifyAqiUpdated(AqiDisplayModel *model);

private:
  AqiDisplayModel *model;
  ClockSmallWidget clockSmallWidget;
  WifiStatusWidget wifiStatusWidget;
  boolean initialDraw = true;
  float lastAqiLevel;
  uint16_t lastAqiPm25;
  uint16_t lastAqiPm10;
  float lastAqiTemp;
  uint8_t lastAqiHumidity;
  uint16_t lastAqiPressure;

  void resetValues();

  void printTextUnit(String text, String unit, uint16_t x, uint16_t y);

  static void fillGradientRect(uint16_t x, uint16_t y, uint16_t width,
                               uint16_t height);

  static uint16_t gradientColor(uint16_t x, uint16_t y);
  void drawLevels(uint16_t x, uint16_t y);
  void drawIndicator(float percent, uint16_t x, uint16_t y);
  void fillArc(uint16_t x, uint16_t y, uint16_t r, float startAngle,
               float endAngle, uint32_t color);
  void unpack(uint16_t color, uint8_t *colors);
  uint16_t pack(uint8_t *colors);
  uint16 mixColors(uint16_t c1, uint16_t c2, float f);
  boolean isDifferent(float f1, float f2);
};

#endif
