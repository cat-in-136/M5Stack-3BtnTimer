#ifndef DIGITS_DISPLAY_H_
#define DIGITS_DISPLAY_H_

#include <M5Stack.h>

class DigitDisplay {
public:
  uint16_t backColor;
  uint16_t textColor;
  uint8_t textSize;

  DigitDisplay() {
    backColor = TFT_BLACK;
    textColor = TFT_WHITE;
    textSize = (M5.Lcd.width() - 5) / 5 / 6;
  };

  void setMin(uint8_t val);
  uint8_t getMin() { return _min; }
  void setSec(uint8_t val);
  uint8_t getSec() { return _sec; }

  void draw(bool force);

private:
  bool _updated[3];

  uint8_t _min;
  uint8_t _sec;
};

#endif // DIGITS_DISPLAY_H_
