#ifndef _ABOUT_PAGE_H
#define _ABOUT_PAGE_H

#include "Page.h"

class AboutPage : public Page
{
public:
    AboutPage(CommandsHanler* commandsHandler);

protected:
    virtual void draw();
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);

    void drawLabelValue(uint8_t line, String label, String value);

};

#endif
