#ifndef TIMER_ENTITY_H_
#define TIMER_ENTITY_H_

#include "BtnDrawer.h"
#include "DigitDisplay.h"

enum class TimerStatus {
  initial = 0,
  stopped,
};

class TimerEntity {
public:
  TimerEntity(DigitDisplay &digitDisplay, BtnDrawer &btnDrawer) {
    _status = TimerStatus::initial;
    _digitDisplay = digitDisplay;
    _btnDrawer = btnDrawer;

    transitToStatus(TimerStatus::initial);
  };

  void setup();
  void loop();

private:
  TimerStatus _status;

  DigitDisplay _digitDisplay;
  BtnDrawer _btnDrawer;

  void transitToStatus(TimerStatus status);
};

#endif // TIMER_ENTITY_H_
