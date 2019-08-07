#include <M5Stack.h>
#include <M5StackUpdater.h>

#include "src/BtnDrawer.h"
#include "src/DigitDisplay.h"

BtnDrawer btnDrawer;
DigitDisplay digitDisplay;

// the setup routine runs once when M5Stack starts up
void setup() {
  // Initialize the M5Stack object
  M5.begin();

  if (digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  // M5.Lcd.setRotation(2);
  M5.Lcd.fillScreen(TFT_BLACK);

  // mute speaker
  M5.Speaker.mute();

  btnDrawer.setText(BUTTON_A, "Min");
  btnDrawer.setText(BUTTON_B, "Sec");
  btnDrawer.setText(BUTTON_C, "Start");
  btnDrawer.draw(true);

  digitDisplay.setMin(12);
  digitDisplay.setSec(34);
  digitDisplay.draw(true);
}

// the loop routine runs over and over again forever
void loop() {
  if (M5.BtnA.isPressed() && M5.BtnB.isPressed()) {
    digitDisplay.setMin(0);
    digitDisplay.setSec(0);
  } else if (M5.BtnA.wasPressed()) {
    const uint8_t min = digitDisplay.getMin() + 1;
    digitDisplay.setMin((min > 99) ? 0 : min);
  } else if (M5.BtnB.wasPressed()) {
    const uint8_t sec = digitDisplay.getSec() + 1;
    digitDisplay.setSec((sec > 59) ? 0 : sec);
  }

  btnDrawer.draw(false);
  digitDisplay.draw(false);

  M5.update();
}
