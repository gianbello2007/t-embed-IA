#pragma once

// TFT_eSPI setup for the ST7789 in the T-Embed CC1101 Plus.  The panel is
// physically 170x320; Display.cpp selects landscape rotation (320x170).
#define ST7789_DRIVER
#define TFT_WIDTH 170
#define TFT_HEIGHT 320
#define TFT_MOSI 9
#define TFT_SCLK 11
#define TFT_CS 41
#define TFT_DC 16
#define TFT_RST -1
#define TFT_RGB_ORDER TFT_RGB
#define TFT_INVERSION_ON
#define SPI_FREQUENCY 40000000
#define SPI_READ_FREQUENCY 20000000
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define SMOOTH_FONT
