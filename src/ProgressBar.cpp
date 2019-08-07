#include "ProgressBar.h"
#include "M5Stack.h"

void ProgressBar::setColor(uint16_t foreColor, uint16_t backColor) {
  if ((_foreColor != foreColor) || (_backColor != backColor)) {
    _updated = true;
    _foreColor = foreColor;
    _backColor = backColor;
  }
}

void ProgressBar::setVisible(bool visible) {
  if (_visible != visible) {
    _updated = true;
    _visible = visible;
  }
}

void ProgressBar::setPercent(uint8_t percent) {
  if (_percent != percent) {
    if (percent > _percent) {
      _updatedBarGrowth = true;
    } else {
      _updated = true;
    }
    _percent = percent;
  }
}

void ProgressBar::draw() {
  if (_updated) {
    _updatedBarGrowth = false;
    _updated = false;

    M5.Lcd.fillRect(rx, ry, rw, rh, _backColor);
    if (_visible) {
      M5.Lcd.drawRect(rx, ry, rw, rh, _foreColor);
      M5.Lcd.fillRect(rx + 1, ry + 1, rw * _percent / 100, rh - 1, _foreColor);
    }
  } else if (_updatedBarGrowth) {
    _updatedBarGrowth = false;
    if (_visible) {
      M5.Lcd.fillRect(rx + 1, ry + 1, rw * _percent / 100, rh - 1, _foreColor);
    }
  }
}
