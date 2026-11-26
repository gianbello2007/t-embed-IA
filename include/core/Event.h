#pragma once

#include <Arduino.h>

enum class InputEventType : uint8_t {
  kRotateClockwise,
  kRotateCounterClockwise,
  kPrimaryClick,
  kPrimaryLongPress,
  kBackClick,
  kBackLongPress,
};

struct InputEvent {
  InputEventType type;
  uint32_t atMillis;
};
