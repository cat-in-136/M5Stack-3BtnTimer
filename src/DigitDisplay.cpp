#include "DigitDisplay.h"
#include "M5Stack.h"

static inline void convIntTo2Digit(uint16_t num, char *const buf) {
  if (num > 99) {
    buf[0] = 'E';
    buf[1] = 'r';
    buf[2] = 0;
  } else {
    const uint8_t n10 = num / 10;
    const uint8_t n1 = num - (10 * n10);

    buf[0] = '0' + n10;
    buf[1] = '0' + n1;
    buf[2] = 0;
  }
}

void DigitDisplay::setMin(uint8_t val) {
  if (val != _min) {
    _updated[0] = true;
    _min = val;
  }
}

void DigitDisplay::setSec(uint8_t val) {
  if (val != _sec) {
    _updated[2] = true;
    _sec = val;
  }
}

void DigitDisplay::draw(bool force) {
  static const int16_t lcdw = M5.Lcd.width();
  static const int16_t lcdh = M5.Lcd.height();

  // Caluculate text geometry (vertical)
  const int32_t th = 8 * textSize;
  const int32_t ty = lcdh / 2 - th / 2 - 1;

  // Setup text configuration
  const uint8_t tempTextDatum = M5.Lcd.getTextDatum();
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.setTextSize(textSize);
  M5.Lcd.setTextColor(textColor, backColor);

  if (_updated[0] || force) {
    _updated[0] = true;

    // prepare text
    char textBuf[4];
    convIntTo2Digit(_min, textBuf);

    const int32_t tx = lcdw / 2 - (6 * textSize) * 2;
    M5.Lcd.drawString(textBuf, tx, ty, 1);
  }
  if (_updated[1] || force) {
    _updated[1] = true;

    const int32_t tx = lcdw / 2;
    M5.Lcd.drawString(":", tx, ty, 1);
  }
  if (_updated[2] || force) {
    _updated[2] = true;

    // prepare text
    char textBuf[4];
    convIntTo2Digit(_sec, textBuf);

    const int32_t tx = lcdw / 2 + (6 * textSize) * 2;
    M5.Lcd.drawString(textBuf, tx, ty, 1);
  }

  M5.Lcd.setTextDatum(tempTextDatum);
}
