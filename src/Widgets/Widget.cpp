#include <LittleFS.h>
#include "Widgets/Widget.h"
#include "Config.h"

Widget::Widget(int32_t x, int32_t y, uint16_t width, uint16_t height) {
  int16_t w = tft.width();
  int16_t h = tft.height();
  this->x = constrain(x, 0, w);
  this->y = constrain(y, 0, h);
  this->width = x + width < w ? width : w - x;
  this->height = y + height < h ? height : h - y;
}

void Widget::setVisible(boolean visible) {
  if (this->isVisible != visible) {
    this->isVisible = visible;
    if (visible) {
      redraw();
    }
  }
}

bool Widget::isTouchIn(uint16_t x, uint16_t y, uint16_t z) {
  return (x >= this->x) && (x <= this->x + this->width) && (y >= this->y) &&
         (y <= this->y + this->height);
}

// Callback function to provide the pixel color at x,y
uint16_t Widget::getPixelColor(uint16_t x, uint16_t y) {
  return tft.readPixel(x, y);
}

// void Widget::setTexFormat(const String fontName, uint8_t datum, uint16_t
// fgcolor, uint16_t bgcolor, uint16_t padding)
// {
//     tft.loadFont(fontName, LittleFS);
//     if (bgcolor == TFT_TRANSPARENT)
//     {
//         tft.setCallback(&Widget::getPixelColor);
//     }
//     else
//     {
//         tft.setCallback(nullptr);
//     }
//     setTexFormat(datum, fgcolor, bgcolor, padding);
// }

void Widget::setTexFormat(const uint8_t font[], uint8_t datum, uint16_t fgcolor,
                          uint16_t bgcolor, uint16_t padding) {
  tft.loadFont(font);
  if (bgcolor == TFT_TRANSPARENT) {
    tft.setCallback(&Widget::getPixelColor);
  } else {
    tft.setCallback(nullptr);
  }
  setTexFormat(datum, fgcolor, bgcolor, padding);
}

void Widget::setTexFormat(const GFXfont *font, uint8_t datum, uint16_t fgcolor,
                          uint16_t bgcolor, uint16_t padding) {
  tft.setFreeFont(font);
  tft.setCallback(nullptr);
  setTexFormat(datum, fgcolor, bgcolor, padding);
}

void Widget::setTexFormat(uint8_t datum, uint16_t fgcolor, uint16_t bgcolor,
                          uint16_t padding) {
  tft.setTextDatum(datum);
  if (bgcolor == TFT_TRANSPARENT) {
    tft.setTextColor(fgcolor);
  } else {
    tft.setTextColor(fgcolor, bgcolor);
  }
  tft.setTextPadding(padding);
}

void Widget::drawBitmap(uint16_t x, uint16_t y, const tImage *image) {
  uint16_t x0, y0;
  const uint16_t *pdata = image->data;
  for (y0 = 0; y0 < image->height; y0++) {
    for (x0 = 0; x0 < image->width; x0++) {
      uint32_t color = pgm_read_byte(pdata++);
      if (color != TFT_BLACK) {
        tft.drawPixel(x + x0, y + y0, color);
      }
    }
  }
}

void Widget::drawBmpFromFile(const String filename, uint8_t x, uint16_t y,
                             TFT_eSprite *sprite) {
  if ((sprite == nullptr) && ((x >= tft.width()) || (y >= tft.height())))
    return;

  boolean fileOk = true;
  fs::File bmpFS;
  // Open requested file on SD card
  bmpFS = LittleFS.open(filename, "r");

  if (!bmpFS) {
    fileOk = false;
    DEBUG_PRINTF("File not found: ");
    DEBUG_PRINTLN(filename);
  }

  uint32_t seekOffset;
  uint16_t w = 10; // set min width to 10 to display error X in case of failure
  uint16_t h = 10;
  uint16_t row;
  uint8_t r, g, b;

  if (fileOk && read16(bmpFS) == 0x4D42) {
    read32(bmpFS);
    read32(bmpFS);
    seekOffset = read32(bmpFS);
    read32(bmpFS);
    w = read32(bmpFS);
    h = read32(bmpFS);

    if ((read16(bmpFS) == 1) &&
        (read16(bmpFS) == 24) /* && (read32(bmpFS) == 0)*/) {
      y += h - 1;

      bool oldSwapBytes =
          sprite == nullptr ? tft.getSwapBytes() : sprite->getSwapBytes();
      if (sprite == nullptr)
        tft.setSwapBytes(true);
      else
        sprite->setSwapBytes(true);

      bmpFS.seek(seekOffset);

      uint16_t padding = (4 - ((w * 3) & 3)) & 3;
      uint8_t lineBuffer[w * 3 + padding];

      for (row = 0; row < h; row++) {
        bmpFS.read(lineBuffer, sizeof(lineBuffer));
        uint8_t *bptr = lineBuffer;
        uint16_t *tptr = (uint16_t *)lineBuffer;
        // Convert 24 to 16 bit colours
        for (uint16_t col = 0; col < w; col++) {
          b = *bptr++;
          g = *bptr++;
          r = *bptr++;
          *tptr++ = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
        }

        // Push the pixel row to screen, pushImage will crop the line if needed
        // y is decremented as the BMP image is drawn bottom up
        if (sprite == nullptr)
          tft.pushImage(x, y--, w, 1, (uint16_t *)lineBuffer);
        else
          sprite->pushImage(x, y--, w, 1, (uint16_t *)lineBuffer);
      }
      if (sprite == nullptr)
        tft.setSwapBytes(oldSwapBytes);
      else
        sprite->setSwapBytes(oldSwapBytes);
    } else {
      fileOk = false;
      DEBUG_PRINTF("BMP format not recognized. Filename: ");
      DEBUG_PRINTLN(filename);
    }
  }
  if (bmpFS)
    bmpFS.close();

  // if (!fileOk)
  // {
  //     if (sprite == nullptr)
  //     {
  //         tft.fillRect(x, y, w, h, TFT_WHITE);
  //         tft.drawRect(x, y, w, h, TFT_RED);
  //         tft.setFreeFont();
  //         tft.setTextDatum(CC_DATUM);
  //         tft.setTextColor(TFT_RED, TFT_WHITE);
  //         tft.setTextPadding(0);
  //         tft.drawString("X", x + w / 2, y + h / 2);
  //     }
  //     else
  //     {
  //         sprite->fillRect(x, y, w, h, TFT_WHITE);
  //         sprite->drawRect(x, y, w, h, TFT_RED);
  //         sprite->setFreeFont();
  //         sprite->setTextDatum(CC_DATUM);
  //         sprite->setTextColor(TFT_RED, TFT_WHITE);
  //         sprite->setTextPadding(0);
  //         sprite->drawString("X", x + w / 2, y + h / 2);
  //     }
  // }
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.
uint16_t Widget::read16(fs::File &f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t Widget::read32(fs::File &f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
