#ifndef _COMMANDS_HANDLER_H
#define _COMMANDS_HANDLER_H

class CommandsHandler
{
public: 
    virtual void showMainPage() = 0;
    virtual void showAqiPage() = 0;
    virtual void showCurrentWeatherDetail() = 0;
    virtual void showForecastPage() = 0;
    virtual void showAboutPage() = 0;
    virtual void setBacklightTimeout(unsigned long timeout) = 0;
    virtual void switchOffBacklight() = 0;
    virtual void switchOnBacklight() = 0;
};

#endif
