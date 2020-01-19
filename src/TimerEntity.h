#ifndef TIMER_ENTITY_H_
#define TIMER_ENTITY_H_

#include "BtnDrawer.h"
#include "DigitDisplay.h"
#include "IconView.h"
#include "ProgressBar.h"

enum class TimerStatus {
  initial = 0,
  stopped,
  countDown,
  beeping,
  countUp,
};

class TimerEntity {
public:
  TimerEntity(DigitDisplay &digitDisplay, BtnDrawer &btnDrawer) {
    _digitDisplay = digitDisplay;
    _btnDrawer = btnDrawer;

    _status = TimerStatus::initial;
    _backColor = TFT_BLACK;

    transitToStatus(TimerStatus::initial);
  };

  void setup();
  void loop();

  void setBeepingEnabled(bool beepingEnabled);
  bool isBeepingEnabled() { return _beepingEnabled; };

private:
  TimerStatus _status;
  unsigned long _startingTime;
  uint8_t _timerMin;
  uint8_t _timerSec;

  uint16_t _backColor;
  bool _visualBeepFlag;
  bool _beepingEnabled = true;

  IconView _bellIcon;
  ProgressBar _progressBar;
  DigitDisplay _digitDisplay;
  BtnDrawer _btnDrawer;

  void setBackColor(uint16_t backColor);
  void transitToStatus(TimerStatus status);
};

#endif // TIMER_ENTITY_H_
