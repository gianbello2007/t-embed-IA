#pragma once

#include <Arduino.h>
#include <driver/gpio.h>

// Verified against LilyGo's public T-Embed-Plus documentation.  GPIO 43/44
// belong to the optional external nRF24L01 interface and must not be assumed
// populated on the handheld itself.
namespace pins {
constexpr gpio_num_t kSharedPower = GPIO_NUM_15;
constexpr gpio_num_t kI2cSda = GPIO_NUM_8;
constexpr gpio_num_t kI2cScl = GPIO_NUM_18;
constexpr gpio_num_t kSpiMosi = GPIO_NUM_9;
constexpr gpio_num_t kSpiMiso = GPIO_NUM_10;
constexpr gpio_num_t kSpiSck = GPIO_NUM_11;
constexpr gpio_num_t kDisplayCs = GPIO_NUM_41;
constexpr gpio_num_t kDisplayDc = GPIO_NUM_16;
constexpr gpio_num_t kBacklight = GPIO_NUM_21;
constexpr gpio_num_t kSdCs = GPIO_NUM_13;
constexpr gpio_num_t kEncoderA = GPIO_NUM_4;
constexpr gpio_num_t kEncoderB = GPIO_NUM_5;
constexpr gpio_num_t kEncoderButton = GPIO_NUM_0;
constexpr gpio_num_t kUserButton = GPIO_NUM_6;
constexpr gpio_num_t kRgbData = GPIO_NUM_14;
constexpr gpio_num_t kIrTx = GPIO_NUM_2;
constexpr gpio_num_t kIrRx = GPIO_NUM_1;
constexpr gpio_num_t kMicData = GPIO_NUM_42;
constexpr gpio_num_t kMicClock = GPIO_NUM_39;
constexpr gpio_num_t kSpeakerBclk = GPIO_NUM_46;
constexpr gpio_num_t kSpeakerLrclk = GPIO_NUM_40;
constexpr gpio_num_t kSpeakerData = GPIO_NUM_7;
constexpr gpio_num_t kPn532Reset = GPIO_NUM_45;
constexpr gpio_num_t kPn532Irq = GPIO_NUM_17;
constexpr gpio_num_t kCc1101Cs = GPIO_NUM_12;
constexpr gpio_num_t kCc1101Gdo0 = GPIO_NUM_3;
constexpr gpio_num_t kCc1101Gdo2 = GPIO_NUM_38;
constexpr gpio_num_t kCc1101Sw1 = GPIO_NUM_47;
constexpr gpio_num_t kCc1101Sw0 = GPIO_NUM_48;
constexpr gpio_num_t kNrf24Cs = GPIO_NUM_44;
constexpr gpio_num_t kNrf24Ce = GPIO_NUM_43;

constexpr uint8_t kPn532Address = 0x24;
constexpr uint8_t kBq27220Address = 0x55;
constexpr uint8_t kBq25896Address = 0x6B;
constexpr uint8_t kRgbCount = 8;
}  // namespace pins
