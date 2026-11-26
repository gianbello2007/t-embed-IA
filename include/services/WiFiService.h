#pragma once

#include <Arduino.h>

class WiFiService {
 public:
  void begin();
  bool configured() const;
  bool connected() const;
  int32_t rssi() const;
};
