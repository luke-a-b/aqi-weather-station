#include "Pages/AqiPage.h"
#include "Bitmaps/IconHumidity.h"
#include "Bitmaps/IconPm10.h"
#include "Bitmaps/IconPm25.h"
#include "Bitmaps/IconPressure.h"
#include "Bitmaps/IconTemp.h"
#include "PollutionLevels.h"
#include "Widgets/ClockSmallWidget.h"
#include "Config.h"
#include "assert.h"

#define INDICATOR_RADIUS 4
#define INDICATOR_LENGTH 40
#define INDICATOR_COLOR 0x0339
#define TOP_COLOR TFT_NAVY
#define BOTTOM_COLOR TFT_BLACK

#define GRADIENT_HEIGHT 300
#define OUTSIDE_GRADIENT TFT_BLACK

AqiPage::AqiPage(AqiDisplayModel *model, ClockModel *clock,
                 CommandsHanler *commandsHandler)
    : Page(commandsHandler), model(model),
      clockSmallWidget(clock, 180, 2, &AqiPage::fillGradientRect),
      wifiStatusWidget(2, 0) {
  resetValues();
  addWidget(&clockSmallWidget);
  addWidget(&wifiStatusWidget);
}

void AqiPage::resetValues() {
  this->initialDraw = true;
  this->lastAqiLevel = -1.0;
  this->lastAqiPm25 = UINT16_MAX;
  this->lastAqiPm10 = UINT16_MAX;
  this->lastAqiTemp = 1000;
  this->lastAqiHumidity = 101;
  this->lastAqiPressure = 0;
  this->aqiStationName = "";
}

void AqiPage::setModel(AqiDisplayModel *model) {
  assert(model != nullptr);
  if (this->model != model) {
    this->model = model;
    resetValues();
  }
}

void AqiPage::update(uint32_t now) {
  if (!this->isVisible)
    return;
  Page::update(now);
  draw();
}

void AqiPage::setVisible(boolean visible) {
  if (this->isVisible != visible) {
    resetValues();
  }
  Page::setVisible(visible);
}

bool AqiPage::handleTouchOff(uint16_t x, uint16_t y, uint16_t z) {
  if (!this->isVisible)
    return false;
  commandsHandler->showAqiPage();
  return true;
}

void AqiPage::draw() {
  if (!this->isVisible)
    return;

  if (this->initialDraw)
    fillGradientRect(0, 0, tft.width(), tft.height());

  if ((this->initialDraw || this->lastAqiLevel != model->getAqiLevel()) &&
      !isnan(model->getAqiLevel())) // index
  {
    this->lastAqiLevel = model->getAqiLevel();
    drawLevels(120, 85);
    drawIndicator(this->lastAqiLevel, 120, 85);
  }

  int32_t y = 110;
  int32_t offset = 40;

  if (this->initialDraw || this->lastAqiPm25 != model->getAqiPm25()) {
    this->lastAqiPm25 = model->getAqiPm25();
    printTextUnit(String(this->lastAqiPm25), "µg/m³", 50, y);
    printTextUnit(String(model->getAqiPercentPm25()), "%", 145, y);
  }

  y += offset;
  if (this->initialDraw || this->lastAqiPm10 != model->getAqiPm10()) {
    this->lastAqiPm10 = model->getAqiPm10();
    printTextUnit(String(model->getAqiPm10()), "µg/m³", 50, y);
    printTextUnit(String(model->getAqiPercentPm10()), "%", 145, y);
  }

  y += offset;
  if (this->initialDraw || this->lastAqiTemp != model->getAqiTemp()) {
    this->lastAqiTemp = model->getAqiTemp();
    printTextUnit(String(this->lastAqiTemp, 1),
                  (model->isMetricSelected() ? FPSTR(CONST_UNIT_CELCIUS)
                                             : FPSTR(CONST_UNIT_FAHRENHEIT)),
                  90, y);
  }

  y += offset;
  if (this->initialDraw || this->lastAqiHumidity != model->getAqiHumidity()) {
    this->lastAqiHumidity = model->getAqiHumidity();
    printTextUnit(String(this->lastAqiHumidity), "%", 90, y);
  }

  y += offset;
  if (this->initialDraw || this->lastAqiPressure != model->getAqiPressure()) {
    this->lastAqiPressure = model->getAqiPressure();
    printTextUnit(String(model->getAqiPressure()), "hPa", 90, y);
  }

  if (this->initialDraw || this->aqiStationName != model->getAqiStaionName()) {
    this->aqiStationName = model->getAqiStaionName();
    tft.setTextDatum(BC_DATUM);
    tft.loadFont(ArialBold14);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.setCursor(120 - strlen(this->aqiStationName.c_str()) / 2 * 8, y + 35);
    tft.print(this->aqiStationName);
  }

  tft.unloadFont();

  if (this->initialDraw) {
    y = 100;
    offset = 40;
    Widget::drawBitmap(6, y, &iconpm25);
    Widget::drawBitmap(5, y += offset, &iconpm10);
    Widget::drawBitmap(7, y += offset, &icontemp);
    Widget::drawBitmap(3, y += offset, &iconhumidity);
    Widget::drawBitmap(3, y += offset, &iconpressure);
  }
  this->initialDraw = false;
}

void AqiPage::printTextUnit(String text, String unit, uint16_t x, uint16_t y) {
  if (!this->initialDraw) {
    // tft.fillRect(x, y, 95, 30, TFT_YELLOW);
    fillGradientRect(x, y, 95, 30); // clear previous value
  }
  Widget::setTexFormat(ArialBold28, BR_DATUM, TFT_WHITE, TFT_BLACK, 0);
  tft.setCursor(x, y);
  tft.print(text);
  tft.loadFont(ArialBold14);
  tft.setCursor(tft.getCursorX() + 2, y + 10);
  tft.print(unit);
}

uint16_t AqiPage::gradientColor(uint16_t /*x - not used here*/, uint16_t y) {

  if (y > GRADIENT_HEIGHT)
    return OUTSIDE_GRADIENT; // Outside gradient area
  uint8_t alpha =
      (255 * y) / GRADIENT_HEIGHT; // alpha is a value in the range 0-255
  return tft.alphaBlend(alpha, BOTTOM_COLOR, TOP_COLOR);
}

void AqiPage::fillGradientRect(uint16_t x, uint16_t y, uint16_t width,
                               uint16_t height) {
  for (uint16_t i = 0; i < height; ++i) {
    uint16_t color = gradientColor(x, y + i);
    tft.drawFastHLine(x, y + i, width, color);
  }
}

void AqiPage::fillArc(uint16_t x, uint16_t y, uint16_t r, float startAngle,
                      float endAngle, uint32_t color) {
  uint16_t x2 = sin(startAngle - 0.01) * r + x;
  uint16_t y2 = cos(startAngle - 0.01) * r + y;
  uint16_t x3;
  uint16_t y3;

  for (float i = startAngle; i < endAngle; i += 0.1) {
    x3 = sin(i) * r + x;
    y3 = cos(i) * r + y;

    tft.fillTriangle(x, y, x2, y2, x3, y3, color);

    x2 = x3;
    y2 = y3;
  }
  x3 = sin(endAngle) * r + x;
  y3 = cos(endAngle) * r + y;
  tft.fillTriangle(x, y, x2, y2, x3, y3, color);
}

void AqiPage::drawLevels(uint16_t x, uint16_t y) {
  fillArc(x, y, 65, HALF_PI, PI + HALF_PI, TFT_BLUE);

  uint16_t colors[] = {0x0400, 0x07e0, 0xffe0, 0xfd65, 0xf963, 0x9000};

  uint16_t x2 = x - 60;
  uint16_t y2 = y;

  for (float i = 0; i <= 5; i += 0.1) {
    float angle = PI * i / 5;

    uint16_t x3 = sin(PI + HALF_PI - angle) * 60 + x;
    uint16_t y3 = cos(PI + HALF_PI - angle) * 60 + y;

    uint16 c = mixColors(colors[(int)i], colors[(int)i + 1], i - ((int)i));
    tft.fillTriangle(x, y, x2, y2, x3, y3, c);

    x2 = x3;
    y2 = y3;
  }

  fillArc(x, y, 20, HALF_PI, PI + HALF_PI, TFT_BLACK);
  tft.drawFastHLine(x - 65, y, 130, TFT_BLACK);
}

void AqiPage::drawIndicator(float percent, uint16_t x, uint16_t y) {
  float angle = percent * PI / PollutionLevels::MAX_LEVELS;
  tft.fillCircle(x, y, INDICATOR_RADIUS, INDICATOR_COLOR);

  uint16_t x1 = sin(-angle) * (INDICATOR_RADIUS - 2) + x;
  uint16_t y1 = cos(-angle) * (INDICATOR_RADIUS - 2) + y;

  uint16_t x2 = sin(-angle + PI) * (INDICATOR_RADIUS - 2) + x;
  uint16_t y2 = cos(-angle + PI) * (INDICATOR_RADIUS - 2) + y;

  uint16_t x3 = sin(PI + HALF_PI - angle) * INDICATOR_LENGTH + x;
  uint16_t y3 = cos(PI + HALF_PI - angle) * INDICATOR_LENGTH + y;

  tft.fillTriangle(x1, y1, x2, y2, x3, y3, INDICATOR_COLOR);
}

void AqiPage::unpack(uint16_t color, uint8_t *colors) {
  colors[0] = (color >> 11) & 0x1f;
  colors[1] = (color >> 5) & 0x3f;
  colors[2] = (color >> 0) & 0x1f;
}

uint16_t AqiPage::pack(uint8_t *colors) {
  uint16_t r = colors[0];
  uint16_t g = colors[1];
  uint16_t b = colors[2];
  return (r << 11) | (g << 5) | b;
}

uint16 AqiPage::mixColors(uint16_t c1, uint16_t c2, float f) {
  uint8_t rgb1[3];
  uint8_t rgb2[3];
  unpack(c1, rgb1);
  unpack(c2, rgb2);

  uint8_t rgb[3];
  for (int i = 0; i < 3; i++) {
    rgb[i] = (1 - f) * rgb1[i] + f * rgb2[i];
  }

  return pack(rgb);
}
