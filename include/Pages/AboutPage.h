#ifndef _ABOUT_PAGE_H
#define _ABOUT_PAGE_H

#include "Page.h"

class AboutPage : public Page
{
public:
    AboutPage(CommandsHanler* commandsHandler);

protected:
    virtual void update(uint32_t now);
    virtual void draw();
    virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);

private: 
    uint32_t lastUpdate = 0;

};

#endif
