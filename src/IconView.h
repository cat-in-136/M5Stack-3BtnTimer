#ifndef ICON_VIEW_H_
#define ICON_VIEW_H_

#include <M5Stack.h>

class IconView {
public:
  uint16_t x = 0;
  uint16_t y = 0;

  IconView() {
    _backColor = TFT_BLACK;
    _foreColor = TFT_WHITE;
  };

  void setColor(uint16_t foreColor, uint16_t backColor);
  uint16_t getForeColor() { return _foreColor; };
  uint16_t getBackColor() { return _backColor; };
  void setVisible(bool visible);
  void setIcon(const uint8_t *icon, uint16_t w, uint16_t h);

  void invalidate() { _updated = true; };
  void draw();

private:
  uint16_t _backColor;
  uint16_t _foreColor;
  bool _updated;

  bool _visible = true;
  const uint8_t *_icon = NULL;
  uint16_t _w = 20;
  uint16_t _h = 20;
};

#endif // ICON_VIEW_H
