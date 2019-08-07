#include "TimerEntity.h"

#include "BtnDrawer.h"
#include "DigitDisplay.h"
#include "M5Stack.h"

static inline unsigned long div_ceil(unsigned long a, unsigned long b) {
  return (a + b - 1) / b;
}

void TimerEntity::setup() {
  transitToStatus(TimerStatus::initial);

  _digitDisplay.draw(true);
  _btnDrawer.draw(true);
}

void TimerEntity::loop() {
  bool forcedUpdateUI = false;

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
    } else if (M5.BtnC.wasPressed()) {
      transitToStatus(TimerStatus::countDown);
    }
    break;
  case TimerStatus::countDown:
    if (M5.BtnC.wasPressed()) {
      transitToStatus(TimerStatus::stopped);
    } else {
      const unsigned long currentTime = millis();
      const long timerTime = ((long)_timerMin * 60l + (long)_timerSec) * 1000l;
      const long elapsedTime = currentTime - _startingTime;
      const long remainingTime = timerTime - elapsedTime;
      const long remainingTimeInSec = div_ceil(remainingTime, 1000);
      const long displayedRemainingTimeInSec =
          ((long)_digitDisplay.getMin() * 60l + (long)_digitDisplay.getSec());

      if (remainingTime <= 0) {
        _digitDisplay.setMin(0);
        _digitDisplay.setSec(0);
        transitToStatus(TimerStatus::beeping);
      } else if (displayedRemainingTimeInSec - remainingTimeInSec >= 1) {
        // update digits
        const uint16_t minutes = remainingTimeInSec / 60;
        const uint16_t seconds = remainingTimeInSec - minutes * 60;

        _digitDisplay.setMin(minutes);
        _digitDisplay.setSec(seconds);
      }
    }
    break;
  case TimerStatus::beeping:
    if (M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()) {
      if (_timerMin != 0 || _timerSec != 0) {
        _digitDisplay.setMin(_timerMin);
        _digitDisplay.setSec(_timerSec);
      }
      if (_visualBeepFlag) {
        M5.Lcd.fillScreen(TFT_BLACK);
        _visualBeepFlag = false;
        forcedUpdateUI = true;
      }
      transitToStatus(TimerStatus::stopped);
    } else {
      const unsigned long beepingPeriod = (millis() - _startingTime) % 1000;

      // Speaker beep
      if (beepingEnabled) {
        if (beepingPeriod % 100 < 50) {
          M5.Speaker.tone(1000, 100);
        }
      }

      // Visual beep (red background flash)
      const bool currentVisualBeepFlag = beepingPeriod < 500;
      if (currentVisualBeepFlag != _visualBeepFlag) {
        _visualBeepFlag = currentVisualBeepFlag;

        M5.Lcd.fillScreen(_visualBeepFlag ? TFT_RED : TFT_BLACK);
        forcedUpdateUI = true;
      }
    }
    break;
  default:
    break;
  }

  _digitDisplay.draw(forcedUpdateUI);
  _btnDrawer.draw(forcedUpdateUI);
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
    if (_status == TimerStatus::initial || _status == TimerStatus::countDown ||
        _status == TimerStatus::beeping) {
      newStatus = status;
    }
    break;
  case TimerStatus::countDown:
    if (_status == TimerStatus::stopped) {
      newStatus = status;
    }
    break;
  case TimerStatus::beeping:
    newStatus = status;
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
    case TimerStatus::countDown:
      _timerMin = _digitDisplay.getMin();
      _timerSec = _digitDisplay.getSec();
      _startingTime = millis();
      _btnDrawer.setTexts("", "", "Stop");
      _btnDrawer.setTexts("Min", "Sec", "Start");
      break;
    case TimerStatus::beeping:
      _startingTime = millis();
      _visualBeepFlag = false;
      _btnDrawer.setTexts("Stop", "Stop", "Stop");
      break;
    default:
      break;
    }
  }
}
