#include "hardware/Board.h"

#include <SPI.h>
#include <Wire.h>

#include "BoardPins.h"

void Board::initializeSharedSpiChipSelects() {
  const gpio_num_t chipSelects[] = {pins::kDisplayCs, pins::kSdCs,
                                    pins::kCc1101Cs, pins::kNrf24Cs};
  for (const gpio_num_t pin : chipSelects) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  // GPIO43/44 are reserved for the optional external nRF24L01 module.  Leaving
  // CE low avoids falsely activating a module if one is fitted.
  pinMode(pins::kNrf24Ce, OUTPUT);
  digitalWrite(pins::kNrf24Ce, LOW);
}

void Board::deselectSharedSpiDevices() {
  digitalWrite(pins::kDisplayCs, HIGH);
  digitalWrite(pins::kSdCs, HIGH);
  digitalWrite(pins::kCc1101Cs, HIGH);
  digitalWrite(pins::kNrf24Cs, HIGH);
}

void Board::enableSharedPeripheralRail() {
  pinMode(pins::kSharedPower, OUTPUT);
  digitalWrite(pins::kSharedPower, HIGH);
}

void Board::begin() {
  Serial.begin(115200);
  delay(20);

  initializeSharedSpiChipSelects();
  SPI.begin(pins::kSpiSck, pins::kSpiMiso, pins::kSpiMosi);
  deselectSharedSpiDevices();

  // This powers CC1101, WS2812 and MAX98357A together.  It is deliberately
  // owned here so no application can claim to switch one device independently.
  enableSharedPeripheralRail();

  pinMode(pins::kPn532Reset, OUTPUT);
  digitalWrite(pins::kPn532Reset, HIGH);
  pinMode(pins::kPn532Irq, INPUT_PULLUP);

  Wire.begin(pins::kI2cSda, pins::kI2cScl);
}
