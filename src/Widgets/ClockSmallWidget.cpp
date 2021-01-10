#include "Widgets/ClockSmallWidget.h"

ClockSmallWidget::ClockSmallWidget(ClockModel *model, uint16_t x, uint16_t y, std::function<void(uint16_t, uint16_t, uint16_t, uint16_t)> eraseHandler) : Widget(x, y, 55, 15), model(model), eraseHandler(eraseHandler)
{
}

void ClockSmallWidget::setVisible(boolean visible)
{
    if (this->isVisible != visible)
    {
        this->initialDraw = true;
        this->lastDrewMin = 61;
        this->lastDrewSec = 61;
    }
    Widget::setVisible(visible);
}

void ClockSmallWidget::update(uint32_t now)
{
    redraw();
}

void ClockSmallWidget::redraw()
{
    if (isVisible)
    {
        time_t now = time(NULL);
        struct tm *timeNow = localtime(&now);

        if (this->initialDraw || this->lastDrewMin != timeNow->tm_min)
        {
            this->lastDrewMin = timeNow->tm_min;

            //tft.fillRect(x, y, width, height, TFT_BLACK);
            this->eraseHandler(this->x, this->y, this->width, this->height);

            boolean is24hStyle = false; //model->isClock24hSytyle();
            uint8_t xmargin = is24hStyle ? 15 : 0;

            setTexFormat(ARIAL_BOLD_14, TL_DATUM, TFT_WHITE, TFT_TRANSPARENT, width);

            char time[8];
            sprintf(time, "%s%d", is24hStyle && timeNow->tm_hour < 10 ? "0" : "", is24hStyle ? timeNow->tm_hour : (timeNow->tm_hour + 11) % 12 + 1);
            this->xcolon = this->x + xmargin + 1 + tft.drawString(time, this->x + xmargin, this->y);
    
            sprintf(time, "%02d%s", timeNow->tm_min, is24hStyle ? "" : (timeNow->tm_hour >= 12 ? "pm" : "am"));
            tft.drawString(time, this->xcolon + 4, y);
            tft.unloadFont();
        }

        if (this->initialDraw || this->lastDrewSec != timeNow->tm_sec)
        {
            this->lastDrewSec = timeNow->tm_sec;
            static bool colon = true;
            colon = !colon;
            setTexFormat(ARIAL_BOLD_14, TL_DATUM, colon ? TFT_WHITE : TFT_DARKGREY, TFT_TRANSPARENT, 0);
            tft.drawString(":", this->xcolon, y);
            tft.unloadFont();
        }

        this->initialDraw = false;
    }
}
