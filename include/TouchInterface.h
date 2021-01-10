#ifndef _TOUCH_INTERFACE_H
#define _TOUCH_INTERFACE_H

#include <TFT_eSPI.h>

class TouchObserver
{
public:
    virtual bool handleTouchOn(uint16_t x, uint16_t y, uint16_t z) = 0;
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z) = 0;
    virtual bool handleSwipeRight() = 0;
    virtual bool handleSwipeLeft() = 0;
    virtual bool handleSwipeDown() = 0;
    virtual bool handleSwipeUp() = 0;
};

class TouchInterface
{
public:
    TouchInterface(TFT_eSPI *tft, TouchObserver *observer);
    void update();

private:
    TFT_eSPI *tft;
    TouchObserver *observer;
    boolean touchOn = false;
    uint16_t xStart, yStart, zStart;
    uint16_t xEnd, yEnd, zEnd;
};

#endif
