#include "TouchInterface.h"
#include "assert.h"

#define SWIPE_THRESHOLD 20

TouchInterface::TouchInterface(TFT_eSPI *tft, TouchObserver* observer)
{
    assert(tft != nullptr);
    assert(observer != nullptr);
    this->tft = tft;
    this->observer = observer;
}

void TouchInterface::update()
{
    uint16_t x, y, z;
    if (tft->getTouch(&x, &y, 350))
    {
        z = tft->getTouchRawZ();
        if (!touchOn)
        {
            touchOn = true;
            xStart = x;
            yStart = y;
            zStart = z;
        }
        xEnd = x;
        yEnd = y;
        zEnd = z;
        observer->handleTouchOn(x, y, z);
    }
    else if (touchOn)
    {
        if (xEnd - xStart > SWIPE_THRESHOLD)
        {
            observer->handleSwipeRight();
        }
        else if (xStart - xEnd > SWIPE_THRESHOLD)
        {
            observer->handleSwipeLeft();
        }
        else if (yEnd - yStart > SWIPE_THRESHOLD)
        {
            observer->handleSwipeDown();
        }
        else if (yStart - yEnd > SWIPE_THRESHOLD)
        {
            observer->handleSwipeUp();
        }
        else
        {
            observer->handleTouchOff(xEnd, yEnd, zEnd);
        }
        touchOn = false;
    }
}
