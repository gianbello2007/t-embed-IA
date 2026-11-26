#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class Display {
 public:
  void begin();
  void setBacklight(uint8_t level);
  TFT_eSPI& tft();

 private:
  TFT_eSPI tft_;
  uint8_t brightness_{0};
};
