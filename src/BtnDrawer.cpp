#include "BtnDrawer.h"
#include "M5Stack.h"

void BtnDrawer::setText(uint8_t id, const String &str) {
  if (id < 3) {
    _texts[id] = str;
    _updated[id] = true;
  }
}

void BtnDrawer::draw(bool force) {
  Button* const buttons[3] = {
    &M5.BtnA,
    &M5.BtnB,
    &M5.BtnC,
  };

  for (uint_fast8_t id = 0; id < 3; id++) {
    if (_updated[id] || force || buttons[id]->wasPressed() || buttons[id]->wasReleased()) {
      drawButton(id, buttons[id]->wasPressed());
    }
  }
}

void BtnDrawer::drawButton(uint8_t id, bool pressed) {
  if (id < 3) {
    const size_t textLength = _texts[id].length() + 2;
    char textBuf[textLength];
    _texts[id].toCharArray(textBuf, textLength);

    _updated[id] = false;
    drawButton(id, pressed, textBuf);
  }
}

void BtnDrawer::drawButton(uint8_t id, bool pressed, const char *const title) {
  const uint16_t fgColor = (pressed) ? backColor : textColor;
  const uint16_t bgColor = (pressed) ? textColor : backColor;

  static const int16_t lcdw = M5.Lcd.width();
  static const int16_t lcdh = M5.Lcd.height();

  // Caluculate button box geometry
  const int32_t rx = lcdw * id / 3 + 1;
  const int32_t ry = lcdh - height;
  const int32_t rw = lcdw / 3 - 2;
  const int32_t rh = height;

  // Draw button box
  M5.Lcd.fillRect(rx, ry, rw, rh, bgColor);

  // Caluculate text geometry
  const int32_t th = 8 * textSize;
  const int32_t tx = rx + rw / 2;
  const int32_t ty = ry + rh / 2 - th / 2 - 1;

  const uint8_t tempTextDatum = M5.Lcd.getTextDatum();

  // Draw text
  M5.Lcd.setTextColor(fgColor, bgColor);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.setTextSize(textSize);
  M5.Lcd.drawString(title, tx, ty, 1);
  M5.Lcd.setTextDatum(tempTextDatum);
}
