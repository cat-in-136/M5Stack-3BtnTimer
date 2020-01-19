#include "IconView.h"
#include "M5Stack.h"

void IconView::setColor(uint16_t foreColor, uint16_t backColor) {
  if ((_foreColor != foreColor) || (_backColor != backColor)) {
    _updated = true;
    _foreColor = foreColor;
    _backColor = backColor;
  }
}

void IconView::setVisible(bool visible) {
  if (_visible != visible) {
    _updated = true;
    _visible = visible;
  }
}

void IconView::setIcon(const uint8_t *icon, uint16_t w, uint16_t h) {
  _icon = icon;
  _w = w;
  _h = h;
  _updated = true;
}

void IconView::draw() {
  if (_updated) {
    _updated = false;

    M5.Lcd.fillRect(x, y, _w, _h, _backColor);
    if (_visible && (_icon != NULL)) {
      M5.Lcd.drawXBitmap(x, y, _icon, _w, _h, _foreColor);
    }
  }
}
