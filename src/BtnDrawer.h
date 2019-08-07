#ifndef BTN_DRAWER_H_
#define BTN_DRAWER_H_

#include <M5Stack.h>

class BtnDrawer {
public:
  uint16_t backColor;
  uint16_t textColor;
  int16_t height;
  uint8_t textSize;

  BtnDrawer() {
    backColor = TFT_DARKGREY;
    textColor = TFT_WHITE;
    height = 20;
    textSize = 2;
  };
  BtnDrawer(const String &btnA, const String &btnB, const String &btnC)
      : _texts{btnA, btnB, btnC} {};

  void setText(uint8_t id, const String &str);
  void setTexts(const String &btnA, const String &btnB, const String &btnC);
  void draw(bool force);

private:
  String _texts[3];
  bool _updated[3];
  void drawButton(uint8_t id, bool pressed);
  void drawButton(uint8_t id, bool pressed, const char *const title);
};

#endif // BTN_DRAWER_H_
