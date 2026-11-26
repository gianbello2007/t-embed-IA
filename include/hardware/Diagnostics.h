#pragma once

#include <Arduino.h>

struct I2cDiagnostics {
  bool pn532{false};
  bool bq27220{false};
  bool bq25896{false};
  uint8_t deviceCount{0};
};

class Diagnostics {
 public:
  void begin();
  const I2cDiagnostics& i2c() const;

 private:
  I2cDiagnostics i2c_;
};
