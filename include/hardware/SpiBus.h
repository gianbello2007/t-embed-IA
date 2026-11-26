#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// All SPI clients must acquire this lock before using the shared bus.  The
// display is currently the only continuously-rendering client; SD and radios
// will use this interface as their tasks are introduced.
class SpiBus {
 public:
  void begin();
  bool lock(TickType_t timeout = portMAX_DELAY);
  void unlock();

 private:
  SemaphoreHandle_t mutex_{nullptr};
};
