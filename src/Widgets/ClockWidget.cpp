#include "Widgets/ClockWidget.h"

ClockWidget::ClockWidget(ClockModel *model, uint16_t y) : Widget(0, y, 240, 79),
                                                                         model(model)
{
}

void ClockWidget::update(uint32_t now)
{
    if (isVisible && now - lastDrew > 1000)
    {
        lastDrew = now;
        static bool display = true;
        display = !display;
        setTexFormat(DSEG7_44, BC_DATUM, display ? TFT_CLOCKGREEN : TFT_SILHOUTTEGREEN, TFT_TRANSPARENT, 0);
        uint8_t margin = model->isClock24hSytyle() ? 0 : 10;
        tft.setCursor(x + 135 - margin, y + 71);
        tft.print(":");
        time_t tnow = time(NULL);
        if (localtime(&tnow)->tm_sec == 0)
        {
            redraw();
        }
    }
}

void ClockWidget::redraw()
{
    if (isVisible)
    {
        //tft.fillRect(x, y, width, height, TFT_BLUE);
        String time = model->getCurrentTime();
        uint8_t margin = model->isClock24hSytyle() ? 0 : 10;
        
        // draw current data
        setTexFormat(ARIAL_BOLD_14, BC_DATUM, TFT_WHITE, TFT_BLACK, width);
        tft.drawString(model->getCurrentDate(), x + 135 - margin, y + 14);

        // draw AQI data
        if (model->isAqiDataValid())
        {
            drawBmpFromFile(model->getAqiLevelIconFileName(), x + 8, y + 19);
            uint16_t color = tft.readPixel(30, 17 + 7);
            setTexFormat(ARIAL_BOLD_14, BC_DATUM, color, TFT_BLACK, 50);
            tft.drawString(String(model->getAqiPercentMax()) + "%", x + 8 + 26, y + height);
        }
        else
        {
            drawBmpFromFile(model->getAqiNaLevelIconFileName(), x + 8, y + 19);
            setTexFormat(ARIAL_BOLD_14, BC_DATUM, TFT_RED, TFT_BLACK, 50);
            tft.drawString("--%", x + 8 + 26, y + height);
        }

        // draw clock
        setTexFormat(BL_DATUM, TFT_CLOCKGREEN, TFT_BLACK, width/2);
        if (!model->isClock24hSytyle())
        {
            // FIXEME: pm/am will not work while time is already converted to proper format by the model
            tft.drawString(time.substring(0, time.indexOf(':')).toInt() >= 12 ? "pm" : "am", x + 221 - margin, y + 72);
        }
        tft.unloadFont();

        String Silhouette = model->isClock24hSytyle() ? "88:88" : "18:88";
        setTexFormat(DSEG7_44, BL_DATUM, model->isClockSilhouetteEnabled() ? TFT_SILHOUTTEGREEN : TFT_BLACK, TFT_TRANSPARENT, 0);
        tft.setCursor(x + 211 - (38 * 4) - margin, y + 71);
        tft.print(Silhouette);

        tft.setTextColor(TFT_CLOCKGREEN);
        tft.setCursor(x + 211 - (38 * (strlen(time.c_str()) - 1)) - margin, y + 71);
        tft.print(time);
    }
}
