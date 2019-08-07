#include "ProgressBar.h"
#include "M5Stack.h"

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

void ProgressBar::draw(bool force) {
  if (force || _updated) {
    _updatedBarGrowth = false;
    _updated = false;

    M5.Lcd.fillRect(rx, ry, rw, rh, backColor);
    if (_visible) {
      M5.Lcd.drawRect(rx, ry, rw, rh, foreColor);
      M5.Lcd.fillRect(rx + 1, ry + 1, rw * _percent / 100, rh - 1, foreColor);
    }
  } else if (_updatedBarGrowth) {
    _updatedBarGrowth = false;
    if (_visible) {
      M5.Lcd.fillRect(rx + 1, ry + 1, rw * _percent / 100, rh - 1, foreColor);
    }
  }
}
