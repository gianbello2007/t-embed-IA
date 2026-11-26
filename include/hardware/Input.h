#pragma once

#include <Arduino.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "core/Event.h"

class Input {
 public:
  void begin(QueueHandle_t eventQueue);
  void poll();

 private:
  void emit(InputEventType type);
  void pollButton(gpio_num_t pin, bool& previousState, uint32_t& pressedAt,
                  InputEventType clickEvent, InputEventType longPressEvent);

  QueueHandle_t eventQueue_{nullptr};
  bool encoderA_{true};
  bool encoderB_{true};
  bool primaryPrevious_{true};
  bool backPrevious_{true};
  uint32_t primaryPressedAt_{0};
  uint32_t backPressedAt_{0};
};
