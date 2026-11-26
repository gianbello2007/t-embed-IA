#pragma once

#include <Arduino.h>

class Storage {
 public:
  bool begin();
  bool isMounted() const;
  uint64_t totalBytes() const;
  uint64_t usedBytes() const;
  const char* lastError() const;

 private:
  bool mounted_{false};
  const char* lastError_{"not initialized"};
};
