#ifndef BTN_DRAWER_H_
#define BTN_DRAWER_H_

#include <M5Stack.h>

class BtnDrawer {
public:
  int16_t height = 20;
  uint8_t textSize = 2;

  BtnDrawer() {
    _backColor = TFT_DARKGREY;
    _textColor = TFT_WHITE;
  };
  BtnDrawer(const String &btnA, const String &btnB, const String &btnC)
      : _texts{btnA, btnB, btnC} {};

  void setColor(uint16_t textColor, uint16_t backColor);
  uint16_t getTextColor() { return _textColor; };
  uint16_t getBackColor() { return _backColor; };
  void setText(uint8_t id, const String &str);
  void setTexts(const String &btnA, const String &btnB, const String &btnC);

  void invalidate() { _updated[0] = _updated[1] = _updated[2] = true; };
  void draw();

private:
  uint16_t _backColor;
  uint16_t _textColor;
  String _texts[3];
  bool _updated[3];

  void drawButton(uint8_t id, bool pressed);
  void drawButton(uint8_t id, bool pressed, const char *const title);
};

#endif // BTN_DRAWER_H_
