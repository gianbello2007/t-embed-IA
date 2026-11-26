#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "hardware/Board.h"
#include "hardware/Diagnostics.h"
#include "hardware/Display.h"
#include "hardware/Input.h"
#include "hardware/Rgb.h"
#include "hardware/SpiBus.h"
#include "hardware/Storage.h"
#include "services/WiFiService.h"
#include "ui/HomeScreen.h"

class System {
 public:
  void begin();
  void tick();

 private:
  QueueHandle_t events_{nullptr};
  Board board_;
  SpiBus spiBus_;
  Display display_;
  Input input_;
  Rgb rgb_;
  Storage storage_;
  Diagnostics diagnostics_;
  WiFiService wifi_;
  HomeScreen home_;
};
