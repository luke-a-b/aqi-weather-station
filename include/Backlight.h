#ifndef _BACKLIGHT_H
#define _BACKLIGHT_H

class Backlight {
public:
  Backlight(unsigned long backlightTimeout = 0);
  void update(uint32_t now);
  void setBacklight(boolean backlight);
  bool isBacklightActive();
  void setBacklightTimeout(unsigned long backlightTimeout);

private:
  unsigned long lastUpdate;
  unsigned long backlightTimeout;
};

#endif