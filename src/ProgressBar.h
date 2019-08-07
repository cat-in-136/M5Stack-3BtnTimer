#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <M5Stack.h>

class ProgressBar {
public:
  uint16_t rx = 0;
  uint16_t ry = 0;
  uint16_t rw = 320;
  uint16_t rh = 20;

  ProgressBar() {
    _backColor = TFT_BLACK;
    _foreColor = TFT_WHITE;
  };

  void setColor(uint16_t foreColor, uint16_t backColor);
  uint16_t getForeColor() { return _foreColor; };
  uint16_t getBackColor() { return _backColor; };
  void setVisible(bool visible);
  void setPercent(uint8_t percent);

  void invalidate() { _updated = true; };
  void draw();

private:
  uint16_t _backColor;
  uint16_t _foreColor;
  bool _updatedBarGrowth;
  bool _updated;

  bool _visible = true;
  uint8_t _percent = 0;
};

#endif // PROGRESS_BAR_H_
