#include "Widgets/AstronomyWidget.h"

AstronomyWidget::AstronomyWidget(AstronomyModel *model, uint16_t y) : Widget(0, y, 240, 60),
                                                                                     model(model)
{
}

void AstronomyWidget::update(uint32_t now)
{
    if (now - lastUpdate > ASTRONOMY_UPDATE_INTERVAL * 1000)
    {
        redraw();
        lastUpdate = now;
    }
}

void AstronomyWidget::redraw()
{
    if (isVisible)
    {
        uint8_t center = width / 2;
        drawBmpFromFile(model->getMoonIconFileName(), x + center - 21, y + 1);

        setTexFormat(ARIAL_BOLD_14, BL_DATUM, TFT_ORANGE, TFT_BLACK, center - 31);
        tft.drawString(model->getSunTranslation(), x + 10, y + 15);
        tft.setTextDatum(BR_DATUM);
        tft.drawString(model->getMoonTranslation(), x + width - 10, y + 15);
        tft.setTextDatum(BC_DATUM);
        tft.drawString(model->getMoonPhaseName(), x + width / 2, y + height);

        tft.setTextDatum(BL_DATUM);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.drawString(model->getSunriseTime(), x + 10, y + 30);
        tft.drawString(model->getSunsetTime(), x + 10, y + 45);

        tft.setTextDatum(BR_DATUM);
        tft.drawString(model->getMoonriseTime(), x + width - 10, y + 30);
        tft.drawString(model->getMoonsetTime(), x + width - 10, y + 45);

        tft.unloadFont();
    }
}
