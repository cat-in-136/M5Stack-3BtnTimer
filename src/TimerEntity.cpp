#include "TimerEntity.h"

#include "BtnDrawer.h"
#include "DigitDisplay.h"
#include "M5Stack.h"

void TimerEntity::setup() {
  transitToStatus(TimerStatus::initial);

  _digitDisplay.draw(true);
  _btnDrawer.draw(true);
}

void TimerEntity::loop() {
  switch (_status) {
  case TimerStatus::initial:
    break;
  case TimerStatus::stopped:
    if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
      _digitDisplay.setMin(0);
      _digitDisplay.setSec(0);
    } else if (M5.BtnA.wasPressed()) {
      const uint8_t min = _digitDisplay.getMin() + 1;
      _digitDisplay.setMin((min > 99) ? 0 : min);
    } else if (M5.BtnB.wasPressed()) {
      const uint8_t sec = _digitDisplay.getSec() + 1;
      _digitDisplay.setSec((sec > 59) ? 0 : sec);
    }
    break;
  default:
    break;
  }

  _digitDisplay.draw(false);
  _btnDrawer.draw(false);
}

void TimerEntity::transitToStatus(TimerStatus status) {
  TimerStatus newStatus = _status;

  // transition
  switch (status) {
  case TimerStatus::initial:
    _digitDisplay.setMin(0);
    _digitDisplay.setSec(0);
    newStatus = TimerStatus::stopped;
    break;
  case TimerStatus::stopped:
    if (_status == TimerStatus::initial) {
      newStatus = status;
    }
    break;
  default:
    break;
  }

  if (newStatus != _status) {
    _status = newStatus;
    // update parameter just after transition
    switch (newStatus) {
    case TimerStatus::initial:
      // not reached
      break;
    case TimerStatus::stopped:
      _btnDrawer.setTexts("Min", "Sec", "Start");
      break;
    default:
      break;
    }
  }
}
