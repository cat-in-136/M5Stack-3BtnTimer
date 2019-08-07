#ifndef DIGITS_DISPLAY_H_
#define DIGITS_DISPLAY_H_

#include <M5Stack.h>

class DigitDisplay {
public:
  uint8_t textSize;

  DigitDisplay() {
    textSize = (M5.Lcd.width() - 5) / 5 / 6;
    _backColor = TFT_BLACK;
    _textColor = TFT_WHITE;
  };

  void setColor(uint16_t textColor, uint16_t backColor);
  uint16_t getTextColor() { return _textColor; };
  uint16_t getBackColor() { return _backColor; };
  void setMin(uint8_t val);
  uint8_t getMin() { return _min; }
  void setSec(uint8_t val);
  uint8_t getSec() { return _sec; }

  void invalidate() { _updated[0] = _updated[1] = _updated[2] = true; };
  void draw();

private:
  uint16_t _backColor;
  uint16_t _textColor;
  bool _updated[3];

  uint8_t _min;
  uint8_t _sec;
};

#endif // DIGITS_DISPLAY_H_
