#include <M5Stack.h>
#include <M5StackUpdater.h>

#include "src/TimerEntity.h"

BtnDrawer btnDrawer;
DigitDisplay digitDisplay;
TimerEntity timerEntity(digitDisplay, btnDrawer);

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
  M5.Speaker.begin();
  M5.Speaker.mute();

  timerEntity.setBeepingEnabled(false);
  timerEntity.setup();
}

// the loop routine runs over and over again forever
void loop() {
  timerEntity.loop();
  M5.update();
}
