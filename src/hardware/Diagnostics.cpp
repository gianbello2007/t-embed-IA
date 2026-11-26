#include "hardware/Diagnostics.h"

#include <Wire.h>

#include "BoardPins.h"

namespace {
bool respondsAt(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}
}

void Diagnostics::begin() {
  i2c_ = {};
  for (uint8_t address = 1; address < 0x7F; ++address) {
    if (respondsAt(address)) {
      ++i2c_.deviceCount;
    }
  }
  i2c_.pn532 = respondsAt(pins::kPn532Address);
  i2c_.bq27220 = respondsAt(pins::kBq27220Address);
  i2c_.bq25896 = respondsAt(pins::kBq25896Address);
}

const I2cDiagnostics& Diagnostics::i2c() const {
  return i2c_;
}
