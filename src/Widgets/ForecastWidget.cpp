#include "Widgets/ForecastWidget.h"

ForecastWidget::ForecastWidget(ForecastModel *model, uint16_t x, uint16_t y, uint8_t dayIndex) : Widget(x, y, 80, 67),
                                                                                                 model(model),
                                                                                                 dayIndex(dayIndex)
{
}

void ForecastWidget::setCoordinates(int32_t x, int32_t y)
{
    if (x + width > 0 && x < tft.width() && y + height > 0 && y < tft.height())
    {
        bool isRedrawNeeded = this->x != x || this->y != y;
        this->x = x;
        this->y = y;
        if (isRedrawNeeded)
            redraw();
    }
}

void ForecastWidget::redraw()
{
    if (isVisible)
    {
        TFT_eSprite sprite(&tft);
        sprite.createSprite(width, height);
        sprite.fillSprite(TFT_BLACK);

        sprite.loadFont(ARIAL_BOLD_14);
        sprite.setTextDatum(BC_DATUM);
        sprite.setTextColor(TFT_ORANGE, TFT_BLACK);
        sprite.setTextPadding(width);
        sprite.drawString(model->getForecastWeekDayName(dayIndex), width / 2, 13);
        sprite.setTextColor(TFT_WHITE, TFT_BLACK);
        String tempString = String(model->getForecastTemp(dayIndex), 0);
        tempString += model->isMetricSelected() ? "°C" : "°F";

        sprite.drawString(tempString, width / 2, 13 + 13);
        sprite.unloadFont();

        drawBmpFromFile(model->getForecastIconFileName(dayIndex), 15, 13 + 13, &sprite);

        sprite.pushSprite(x, y);
        sprite.deleteSprite();
    }
}
