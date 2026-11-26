#pragma once

#include <Arduino.h>

class Board {
 public:
  void begin();
  void deselectSharedSpiDevices();
  void enableSharedPeripheralRail();

 private:
  void initializeSharedSpiChipSelects();
};
