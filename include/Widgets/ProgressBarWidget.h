#ifndef _PROGRESS_BAR_WIDGET_H
#define _PROGRESS_BAR_WIDGET_H

#include <Arduino.h>
#include "Widgets/Widget.h"

class ProgressBarWidget : public Widget {
    public:
        ProgressBarWidget(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t textColor, uint16_t frameColor, uint16_t barColor);
        void setProgress(uint8_t percentage, String text);
        void setError(String text);
    protected:
        virtual void redraw();
        uint8_t percentage = 0;
        String text = "";
        uint16_t textColor;
        uint16_t frameColor;
        uint16_t barColor;
};

#endif
