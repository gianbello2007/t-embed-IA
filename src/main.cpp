#include <Arduino.h>

#include "core/System.h"

namespace {
System firmwareSystem;
}

void setup() {
  firmwareSystem.begin();
}

void loop() {
  firmwareSystem.tick();
}
