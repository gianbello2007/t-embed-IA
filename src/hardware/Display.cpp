#include "hardware/Display.h"

#include "BoardPins.h"

void Display::begin() {
  pinMode(pins::kBacklight, OUTPUT);
  digitalWrite(pins::kBacklight, LOW);

  tft_.init();
  tft_.setRotation(3);
  tft_.fillScreen(TFT_BLACK);
  setBacklight(12);
}

void Display::setBacklight(uint8_t level) {
  if (level > 16) {
    level = 16;
  }
  if (brightness_ == level) {
    return;
  }

  // AW9364 accepts a reset-low interval followed by short pulses.  This is
  // intentionally not PWM.  The bounded 3ms reset belongs to hardware setup,
  // never to the animation path.
  digitalWrite(pins::kBacklight, LOW);
  delayMicroseconds(3000);
  for (uint8_t pulse = 0; pulse < level; ++pulse) {
    digitalWrite(pins::kBacklight, HIGH);
    delayMicroseconds(10);
    digitalWrite(pins::kBacklight, LOW);
    delayMicroseconds(10);
  }
  if (level > 0) {
    digitalWrite(pins::kBacklight, HIGH);
  }
  brightness_ = level;
}

TFT_eSPI& Display::tft() {
  return tft_;
}
