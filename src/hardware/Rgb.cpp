#include "hardware/Rgb.h"

#include <FastLED.h>

#include "BoardPins.h"

namespace {
CRGB leds[pins::kRgbCount];

CRGB colorFor(LedStatus status) {
  switch (status) {
    case LedStatus::kBooting:
      return CRGB(10, 20, 55);
    case LedStatus::kReady:
      return CRGB(0, 40, 20);
    case LedStatus::kWarning:
      return CRGB(50, 24, 0);
    case LedStatus::kError:
      return CRGB(50, 0, 8);
  }
  return CRGB::Black;
}
}  // namespace

void Rgb::begin() {
  FastLED.addLeds<WS2812B, pins::kRgbData, GRB>(leds, pins::kRgbCount);
  FastLED.setBrightness(20);
  setStatus(LedStatus::kBooting);
}

void Rgb::setStatus(LedStatus status) {
  status_ = status;
  fill_solid(leds, pins::kRgbCount, colorFor(status_));
  FastLED.show();
}
