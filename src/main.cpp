#include <Arduino.h>

#include "core/System.h"

namespace {
System system;
}

void setup() {
  system.begin();
}

void loop() {
  system.tick();
}
