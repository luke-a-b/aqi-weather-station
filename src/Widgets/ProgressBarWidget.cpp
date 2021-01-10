#include "Widgets/ProgressBarWidget.h"

ProgressBarWidget::ProgressBarWidget(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t textColor, uint16_t frameColor, uint16_t barColor) : 
    Widget(x, y, width, height),
    textColor(textColor),
    frameColor(frameColor),
    barColor(barColor)
{
}

void ProgressBarWidget::setProgress(uint8_t percentage, String text)
{
    this->percentage = percentage;
    this->text = text;
    redraw();
}

void ProgressBarWidget::setError(String text)
{
    if (!isVisible)
        setVisible(true);
    this->percentage = 100;
    this->text = text;
    uint16_t prevBarColor = this->barColor;
    uint16_t prevTextColor = this->textColor;
    this->barColor = TFT_RED;
    this->textColor = TFT_RED;
    redraw();
    this->barColor = prevBarColor;
    this->textColor = prevTextColor;
}

void ProgressBarWidget::redraw()
{
    if (isVisible)
    {
        uint8_t margin = 2;
        uint16_t barHeight = height - 2 * margin;
        uint16_t barWidth = width - 2 * margin;

        Widget::setTexFormat(ARIAL_BOLD_14, BC_DATUM, textColor, TFT_BLACK, width);
        tft.drawString(text, x + width / 2, y);

        tft.fillRoundRect(x, y + 5, width, height, 3, TFT_BLACK);
        tft.drawRoundRect(x, y + 5, width, height, 3, frameColor);
        tft.fillRect(x + margin, y + 5 + margin, barWidth * percentage / 100.0, barHeight, barColor);
        tft.unloadFont();
    }
}
