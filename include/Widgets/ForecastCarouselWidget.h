#ifndef _FORECAST_CAROUSEL_WIDGET_H
#define _FORECAST_CAROUSEL_WIDGET_H

#include "Carousel.h"
#include "Widgets/Widget.h"
#include "Models/ForecastModel.h"
#include "Widgets/ForecastWidget.h"

#define FRAMES_NUMBER 2

class ForecastCarouselWidget : public Widget 
{
    public:
        ForecastCarouselWidget(ForecastModel *model, uint16_t y);
    protected:
        virtual bool handleTouchOff(uint16_t x, uint16_t y, uint16_t z);
        void drawSingleDay(int32_t x, int32_t y, uint8_t dayIndex);
        virtual void redraw();
        virtual void update(uint32_t now);

        ForecastModel *model;
        Carousel carousel;
        bool isTouchHandled = false;
        static void drawForecast1(TFT_eSPI *tft, CarouselState* state, int16_t x, int16_t y);
        static void drawForecast2(TFT_eSPI *tft, CarouselState* state, int16_t x, int16_t y);
        FrameCallback frames[FRAMES_NUMBER];
        FrameState lastFrameState = IN_TRANSITION;

};

#endif