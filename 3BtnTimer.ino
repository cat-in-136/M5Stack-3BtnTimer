#include <M5Stack.h>
#include <M5StackUpdater.h>

#include "src/BtnDrawer.h"

BtnDrawer btnDrawer;

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

  // LCD display
  M5.Lcd.print("Hello World");

  btnDrawer.setText(BUTTON_A, "Min");
  btnDrawer.setText(BUTTON_B, "Sec");
  btnDrawer.setText(BUTTON_C, "Start");
  btnDrawer.draw(true);
}

// the loop routine runs over and over again forever
void loop() {
  btnDrawer.draw(false);

  M5.update();
}
