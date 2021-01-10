#include "Widgets/ForecastCarouselWidget.h"

ForecastCarouselWidget::ForecastCarouselWidget(ForecastModel *model, uint16_t y) : Widget(0, y, 240, 67),
                                                                                   model(model),
                                                                                   carousel(&tft, 0, y, 240, 67)
{
    frames[0] = &ForecastCarouselWidget::drawForecast1;
    frames[1] = &ForecastCarouselWidget::drawForecast2;
    //carousel.init();
    carousel.setFrames(frames, FRAMES_NUMBER);
    carousel.getUiState()->userData = this;
    carousel.disableAllIndicators();
    carousel.setFrameAnimation(SLIDE_LEFT);
    carousel.setTimePerTransition(1000);
    carousel.setTargetFPS(30);
    carousel.setTimePerFrame(5000);
    //carousel.disableAutoTransition();
}

bool ForecastCarouselWidget::handleTouchOff(uint16_t x, uint16_t y, uint16_t z)
{
    // TODO: add possibility to enable/disable auto transition
    // if (isTouchIn(x, y, z))
    // {
    //     carousel.enableAutoTransition();
    //     return true;
    // }
    return false;
}

void ForecastCarouselWidget::redraw()
{
    this->lastFrameState = IN_TRANSITION;
    drawForecast1(&tft, carousel.getUiState(), 0, 0);
}

void ForecastCarouselWidget::drawSingleDay(int32_t x, int32_t y, uint8_t dayIndex)
{
    if (isVisible)
    {
        TFT_eSprite sprite(&tft);
        sprite.createSprite(width / 3, height);
        sprite.fillSprite(TFT_BLACK);

        sprite.loadFont(ARIAL_BOLD_14);
        sprite.setTextDatum(BC_DATUM);
        sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
        sprite.setTextPadding(width / 3);
        sprite.drawString(model->getForecastWeekDayName(dayIndex), width / 6, 13);
        sprite.setTextColor(TFT_WHITE, TFT_BLACK);
        String tempString = String(model->getForecastTemp(dayIndex), 0);
        tempString += model->isMetricSelected() ? "°C" : "°F";

        sprite.drawString(tempString, width / 6, 13 + 13);
        sprite.unloadFont();

        drawBmpFromFile(model->getForecastIconFileName(dayIndex), 15, 13 + 13, &sprite);

        sprite.pushSprite(x, y);
        sprite.deleteSprite();
    }
}

void ForecastCarouselWidget::update(uint32_t now)
{
    if (this->isVisible)
    {
        carousel.update(now);
    }
}

void ForecastCarouselWidget::drawForecast1(TFT_eSPI *tft, CarouselState *state, int16_t x, int16_t y)
{
    ForecastCarouselWidget *instance = static_cast<ForecastCarouselWidget *>(state->userData);
    if (instance->isVisible &&
        ((state->frameState == FIXED && instance->lastFrameState != FIXED) ||
        (state->frameState == IN_TRANSITION && (x != 0 || y != 0))))
    {
        instance->drawSingleDay(0 * instance->width / 3 + x, instance->y + y, 0);
        instance->drawSingleDay(1 * instance->width / 3 + x, instance->y + y, 1);
        instance->drawSingleDay(2 * instance->width / 3 + x, instance->y + y, 3);

        instance->lastFrameState = state->frameState;
    }
}

void ForecastCarouselWidget::drawForecast2(TFT_eSPI *tft, CarouselState *state, int16_t x, int16_t y)
{
    ForecastCarouselWidget *instance = static_cast<ForecastCarouselWidget *>(state->userData);
    if (instance->isVisible &&
        ((state->frameState == FIXED && instance->lastFrameState != FIXED) ||
        (state->frameState == IN_TRANSITION && (x != 0 || y != 0))))
    {
        instance->drawSingleDay(0 * instance->width / 3 + x, instance->y + y, 5);
        instance->drawSingleDay(1 * instance->width / 3 + x, instance->y + y, 7);
        instance->drawSingleDay(2 * instance->width / 3 + x, instance->y + y, 9);

        instance->lastFrameState = state->frameState;
    }
}
