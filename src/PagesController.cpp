#include "PagesController.h"
#include "assert.h"

PagesController::PagesController(Backlight *backlight, DisplayModel *model)
    : backlight(backlight), model(model), mainPage(model, this),
      aqiPage(model->getAqiDisplayModelAt(0), model, this), aboutPage(this),
      currentWeatherDetailPage(model, this), forecastPage(model, this) {
  assert(backlight != nullptr);
  currentPage = &mainPage;
}

void PagesController::update(uint32_t now) {
  assert(currentPage != nullptr);
  currentPage->update(now);
}

void PagesController::showPage(Page *page) {
  assert(page != nullptr);
  assert(currentPage != nullptr);
  if (currentPage != page) {
    currentPage->setVisible(false);
    currentPage = page;
  }
  currentPage->setVisible(true);
}

void PagesController::hideCurrentPage() {
  assert(currentPage != nullptr);
  currentPage->setVisible(false);
}

void PagesController::showMainPage() { showPage(&mainPage); }

void PagesController::showAqiPage() {
  while (currentAqiPage < MAX_AQI_STATIONS &&
         model->getAqiDataModelAt(currentAqiPage)->stationUrl[0] == 0) {
    currentAqiPage++;
  }
  if (currentAqiPage < MAX_AQI_STATIONS) {
    aqiPage.setModel(model->getAqiDisplayModelAt(currentAqiPage));
    aqiPage.setVisible(false); // clear screen
    showPage(&aqiPage);
    currentAqiPage++;
  } else {
    currentAqiPage = 0;
    showMainPage();
  }
}

void PagesController::showCurrentWeatherDetail() {
  showPage(&currentWeatherDetailPage);
}

void PagesController::showForecastPage() { showPage(&forecastPage); }

void PagesController::showAboutPage() { showPage(&aboutPage); }

void PagesController::setBacklightTimeout(unsigned long timeout) {
  backlight->setBacklightTimeout(timeout);
}

void PagesController::switchOffBacklight() { backlight->setBacklight(false); }

void PagesController::switchOnBacklight() { backlight->setBacklight(true); }

bool PagesController::handleTouchOn(uint16_t x, uint16_t y, uint16_t z) {
  assert(currentPage != nullptr);
  return currentPage->handleTouchOn(x, y, z);
}

bool PagesController::handleTouchOff(uint16_t x, uint16_t y, uint16_t z) {
  assert(currentPage != nullptr);
  if (backlight->getBacklight()) {
    currentPage->handleTouchOff(x, y, z);
  }
  backlight->setBacklight(true);
  return true;
}

bool PagesController::handleSwipeRight() {
  assert(currentPage != nullptr);
  return currentPage->handleSwipeRight();
}

bool PagesController::handleSwipeLeft() {
  assert(currentPage != nullptr);
  return currentPage->handleSwipeLeft();
}

bool PagesController::handleSwipeDown() {
  assert(currentPage != nullptr);
  if (!currentPage->handleSwipeDown()) {
    backlight->setBacklight(false);
  }
  return true;
}

bool PagesController::handleSwipeUp() {
  assert(currentPage != nullptr);
  if (!currentPage->handleSwipeDown()) {
    if (currentPage == &mainPage) {
      showAboutPage();
    }
  }
  return true;
}

void PagesController::notifyAqiUpdateStart() {
  assert(currentPage != nullptr);
  currentPage->notifyAqiUpdateStart();
}

void PagesController::notifyAqiUpdated(boolean status) {
  assert(currentPage != nullptr);
  currentPage->notifyAqiUpdated(status);
}

void PagesController::notifyWeatherUpdateStart() {
  assert(currentPage != nullptr);
  currentPage->notifyWeatherUpdateStart();
}

void PagesController::notifyWeatherUpdated(boolean status) {
  assert(currentPage != nullptr);
  currentPage->notifyWeatherUpdated(status);
}

void PagesController::notifyLocalTempUpdated(boolean status) {
  assert(currentPage != nullptr);
  currentPage->notifyLocalTempUpdated(status);
}
