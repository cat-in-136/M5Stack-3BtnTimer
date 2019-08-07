#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <M5Stack.h>

class ProgressBar {
public:
  uint16_t backColor;
  uint16_t foreColor;

  uint16_t rx;
  uint16_t ry;
  uint16_t rw;
  uint16_t rh;

  ProgressBar() {
    backColor = TFT_BLACK;
    foreColor = TFT_WHITE;

    _visible = true;
    rx = ry = 0;
    rw = 320;
    rh = 20;
    _percent = 0;
  };

  void setVisible(bool visible);
  void setPercent(uint8_t percent);

  void draw(bool force);

private:
  bool _updatedBarGrowth;
  bool _updated;

  bool _visible;
  uint8_t _percent;
};

#endif // PROGRESS_BAR_H_
