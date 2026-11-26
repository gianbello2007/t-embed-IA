#pragma once

#include <Arduino.h>

#include "core/Event.h"
#include "hardware/Diagnostics.h"
#include "hardware/Display.h"
#include "hardware/Storage.h"
#include "services/WiFiService.h"

class HomeScreen {
 public:
  enum class Page : uint8_t { kLauncher, kSystem, kStorage, kNetwork, kHardware };

  void begin(Display& display);
  void handle(const InputEvent& event);
  void refresh(const Storage& storage, const Diagnostics& diagnostics,
               const WiFiService& wifi);

 private:
  void drawLauncher();
  void drawSystem();
  void drawStorage();
  void drawNetwork();
  void drawHardware();
  void drawHeader(const char* title, const char* detail);
  void drawFooter(const char* text);
  void drawStatusDot(int16_t x, int16_t y, bool active);
  const Storage* storage_{nullptr};
  const Diagnostics* diagnostics_{nullptr};
  const WiFiService* wifi_{nullptr};
  Display* display_{nullptr};
  Page page_{Page::kLauncher};
  uint8_t selected_{0};
  uint32_t lastRefresh_{0};
};
