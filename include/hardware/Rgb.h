#pragma once

#include <Arduino.h>

enum class LedStatus : uint8_t { kBooting, kReady, kWarning, kError };

class Rgb {
 public:
  void begin();
  void setStatus(LedStatus status);

 private:
  LedStatus status_{LedStatus::kBooting};
};
