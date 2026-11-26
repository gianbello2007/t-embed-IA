#include "hardware/SpiBus.h"

void SpiBus::begin() {
  mutex_ = xSemaphoreCreateMutex();
}

bool SpiBus::lock(TickType_t timeout) {
  return mutex_ != nullptr && xSemaphoreTake(mutex_, timeout) == pdTRUE;
}

void SpiBus::unlock() {
  if (mutex_ != nullptr) {
    xSemaphoreGive(mutex_);
  }
}
