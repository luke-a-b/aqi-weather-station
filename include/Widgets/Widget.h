#ifndef _WIDGET_H
#define _WIDGET_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LittleFS.h>

#include "Fonts/Fonts.h"

extern TFT_eSPI tft;

typedef struct {
  const uint16_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t  dataSize;
} tImage;

class Widget
{
public:
    Widget(int32_t x, int32_t y, uint16_t width, uint16_t height);
 
    bool isTouchIn(uint16_t x, uint16_t y, uint16_t z);
 
    virtual void setVisible(boolean visible);
    virtual bool handleTouchOn(uint16_t x, uint16_t y, uint16_t z) { return false; }
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z) { return false; }
    virtual bool handleSwipeRight() { return false; }
    virtual bool handleSwipeLeft() { return false; }
    virtual bool handleSwipeDown() { return false; }
    virtual bool handleSwipeUp() { return false; }
    virtual void update(uint32_t now) {}
    virtual void redraw() = 0;

    static void drawBitmap(uint16_t x, uint16_t y, const tImage *image);
    static void drawBmpFromFile(const String filename, uint8_t x, uint16_t y, TFT_eSprite *sprite = nullptr);
    // static void setTexFormat(const String fontName, uint8_t datum, uint16_t fgcolor, uint16_t bgcolor, uint16_t padding);
    static void setTexFormat(const GFXfont *font, uint8_t datum, uint16_t fgcolor, uint16_t bgcolor, uint16_t padding);
    static void setTexFormat(const uint8_t font[], uint8_t datum, uint16_t fgcolor, uint16_t bgcolor, uint16_t padding);
    static void setTexFormat(uint8_t datum, uint16_t fgcolor, uint16_t bgcolor, uint16_t padding);
protected:
    boolean isVisible = false;
    int32_t x;
    int32_t y;
    uint16_t width;
    uint16_t height;
private:
    static uint16_t getPixelColor(uint16_t x, uint16_t y);
    static uint16_t read16(fs::File &f);
    static uint32_t read32(fs::File &f);
};

#endif
