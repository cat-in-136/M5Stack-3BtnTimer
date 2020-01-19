#include "TimerEntity.h"

#include "BtnDrawer.h"
#include "DigitDisplay.h"
#include "IconView.h"
#include "M5Stack.h"
#include "ProgressBar.h"

static const size_t BELL_ICON_WIDTH = 20;
static const size_t BELL_ICON_HEIGHT = 20;
static const uint8_t BELL_ICON[] = {
   0x00, 0x06, 0x00, 0x00, 0x06, 0x00, 0x00, 0x0f, 0x00, 0xc0, 0x3f, 0x00,
   0xe0, 0x7f, 0x00, 0xe0, 0x7f, 0x00, 0xf0, 0xff, 0x00, 0xf0, 0xff, 0x00,
   0xf0, 0xff, 0x00, 0xf0, 0xff, 0x00, 0xf0, 0xff, 0x00, 0xf0, 0xff, 0x00,
   0xf8, 0xff, 0x01, 0xf8, 0xff, 0x01, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03,
   0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x06, 0x00 };
static const size_t BELL_SLASH_ICON_WIDTH = 20;
static const size_t BELL_SLASH_ICON_HEIGHT = 20;
static const uint8_t BELL_SLASH_ICON[] = {
   0x00, 0x06, 0x00, 0x01, 0x06, 0x00, 0x03, 0x0f, 0x00, 0x87, 0x3f, 0x00,
   0xfe, 0x7f, 0x00, 0xfc, 0x7f, 0x00, 0xf0, 0xff, 0x00, 0xe0, 0xff, 0x00,
   0x80, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xfe, 0x00, 0x10, 0xfc, 0x00,
   0x78, 0xf0, 0x01, 0xf8, 0xe0, 0x01, 0xfc, 0xc1, 0x03, 0xfc, 0x03, 0x07,
   0x00, 0x0f, 0x0e, 0x00, 0x0f, 0x0c, 0x00, 0x0f, 0x00, 0x00, 0x06, 0x00 };

static inline unsigned long div_ceil(unsigned long a, unsigned long b) {
  return (a + b - 1) / b;
}

void TimerEntity::setup() {
  transitToStatus(TimerStatus::initial);

  _progressBar.rx = 0;
  _progressBar.rw = M5.Lcd.width();
  _bellIcon.x = 0;
  _bellIcon.y = _progressBar.rh + 2;
  setBeepingEnabled(_beepingEnabled);

  _progressBar.invalidate();
  _bellIcon.invalidate();
  _digitDisplay.invalidate();
  _btnDrawer.invalidate();
}

void TimerEntity::loop() {
  switch (_status) {
  case TimerStatus::initial:
    break;
  case TimerStatus::stopped:
    if ((M5.BtnA.wasPressed() && M5.BtnB.isPressed()) ||
        (M5.BtnA.isPressed() && M5.BtnB.wasPressed())) {
      if ((_digitDisplay.getMin() == 0) && (_digitDisplay.getSec() == 0)) {
        setBeepingEnabled(!_beepingEnabled);
      } else {
        _digitDisplay.setMin(0);
        _digitDisplay.setSec(0);
      }
    } else if (M5.BtnA.wasPressed()) {
      const uint8_t min = _digitDisplay.getMin() + 1;
      _digitDisplay.setMin((min > 99) ? 0 : min);
    } else if (M5.BtnB.wasPressed()) {
      const uint8_t sec = _digitDisplay.getSec() + 1;
      _digitDisplay.setSec((sec > 59) ? 0 : sec);
    } else if (M5.BtnC.wasPressed()) {
      if ((_digitDisplay.getMin() == 0) && (_digitDisplay.getSec() == 0)) {
        transitToStatus(TimerStatus::countUp);
      } else {
        transitToStatus(TimerStatus::countDown);
      }
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
      const uint8_t elapsedPercent = (uint8_t)(100 * elapsedTime / timerTime);

      if (remainingTime <= 0) {
        // transit to beeping if timer expired
        transitToStatus(TimerStatus::beeping);
      } else if (displayedRemainingTimeInSec - remainingTimeInSec >= 1) {
        _progressBar.setPercent(elapsedPercent);

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
        setBackColor(TFT_BLACK);
        _visualBeepFlag = false;
      }
      transitToStatus(TimerStatus::stopped);
    } else {
      const unsigned long beepingPeriod = (millis() - _startingTime) % 1000;

      // Speaker beep
      if (_beepingEnabled) {
        if (beepingPeriod % 100 < 50) {
          M5.Speaker.tone(1000, 100);
        }
      }

      // Visual beep (red background flash)
      const bool currentVisualBeepFlag = beepingPeriod < 500;
      if (currentVisualBeepFlag != _visualBeepFlag) {
        _visualBeepFlag = currentVisualBeepFlag;

        setBackColor(_visualBeepFlag ? TFT_RED : TFT_BLACK);
      }
    }
    break;
  case TimerStatus::countUp:
    if (M5.BtnC.wasPressed()) {
      transitToStatus(TimerStatus::stopped);
    } else {
      const unsigned long currentTime = millis();
      const long elapsedTime = currentTime - _startingTime;
      const long elapsedTimeInSec = elapsedTime / 1000;
      const long displayedElapsedTimeInSec =
          ((long)_digitDisplay.getMin() * 60l + (long)_digitDisplay.getSec());

      if (elapsedTimeInSec - displayedElapsedTimeInSec >= 1) {
        // update digits
        const uint16_t minutes = elapsedTimeInSec / 60;
        const uint16_t seconds = elapsedTimeInSec - minutes * 60;

        _digitDisplay.setMin(minutes);
        _digitDisplay.setSec(seconds);
      }
    }
    break;
  default:
    break;
  }

  _progressBar.draw();
  _bellIcon.draw();
  _digitDisplay.draw();
  _btnDrawer.draw();
}

void TimerEntity::setBackColor(uint16_t backColor) {
  if (_backColor != backColor) {
    _backColor = backColor;
    M5.Lcd.fillScreen(backColor);
    _progressBar.setColor(_progressBar.getForeColor(), backColor);
    _bellIcon.setColor(_progressBar.getForeColor(), backColor);
    _digitDisplay.setColor(_digitDisplay.getTextColor(), backColor);
    // Do not change color of _btnDrawer but required to redraw.
    _btnDrawer.invalidate();
  }
}

void TimerEntity::setBeepingEnabled(bool beepingEnabled) {
  if (_beepingEnabled != beepingEnabled) {
    _beepingEnabled = beepingEnabled;
    if (beepingEnabled) {
      _bellIcon.setIcon(BELL_ICON, BELL_ICON_WIDTH, BELL_ICON_HEIGHT);
    } else {
      _bellIcon.setIcon(BELL_SLASH_ICON, BELL_SLASH_ICON_WIDTH, BELL_SLASH_ICON_HEIGHT);
    }
    _bellIcon.invalidate();
  }
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
        _status == TimerStatus::beeping || _status == TimerStatus::countUp) {
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
  case TimerStatus::countUp:
    if (_status == TimerStatus::stopped) {
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
      _progressBar.setVisible(false);
      _btnDrawer.setTexts("Min", "Sec", "Start");
      break;
    case TimerStatus::countDown:
      _progressBar.setVisible(true);
      _progressBar.setPercent(0);
      _timerMin = _digitDisplay.getMin();
      _timerSec = _digitDisplay.getSec();
      _startingTime = millis();
      _btnDrawer.setTexts("", "", "Stop");
      break;
    case TimerStatus::beeping:
      _startingTime = millis();
      _visualBeepFlag = false;
      _progressBar.setVisible(false);
      _digitDisplay.setMin(0);
      _digitDisplay.setSec(0);
      _btnDrawer.setTexts("Stop", "Stop", "Stop");
      break;
    case TimerStatus::countUp:
      _timerMin = 0;
      _timerSec = 0;
      _startingTime = millis();
      _progressBar.setVisible(false);
      _btnDrawer.setTexts("", "", "Stop");
      break;
    default:
      break;
    }
  }
}
