#include "hardware/Input.h"

#include "BoardPins.h"

namespace {
constexpr uint32_t kLongPressMs = 700;
constexpr uint32_t kDebounceMs = 20;
}

void Input::begin(QueueHandle_t eventQueue) {
  eventQueue_ = eventQueue;
  pinMode(pins::kEncoderA, INPUT_PULLUP);
  pinMode(pins::kEncoderB, INPUT_PULLUP);
  pinMode(pins::kEncoderButton, INPUT_PULLUP);
  pinMode(pins::kUserButton, INPUT_PULLUP);
  encoderA_ = digitalRead(pins::kEncoderA);
  encoderB_ = digitalRead(pins::kEncoderB);
  primaryPrevious_ = digitalRead(pins::kEncoderButton);
  backPrevious_ = digitalRead(pins::kUserButton);
}

void Input::emit(InputEventType type) {
  if (eventQueue_ == nullptr) {
    return;
  }
  const InputEvent event{type, millis()};
  xQueueSend(eventQueue_, &event, 0);
}

void Input::pollButton(gpio_num_t pin, bool& previousState, uint32_t& pressedAt,
                       InputEventType clickEvent, InputEventType longPressEvent) {
  const bool currentState = digitalRead(pin);
  if (currentState == previousState) {
    return;
  }

  const uint32_t now = millis();
  if (!currentState) {
    pressedAt = now;
  } else if (now - pressedAt >= kDebounceMs) {
    emit(now - pressedAt >= kLongPressMs ? longPressEvent : clickEvent);
  }
  previousState = currentState;
}

void Input::poll() {
  const bool currentA = digitalRead(pins::kEncoderA);
  if (currentA != encoderA_) {
    const bool currentB = digitalRead(pins::kEncoderB);
    // Quadrature edge decoder.  Direction is intentionally derived only from
    // a phase-A transition to keep the event rate usable for menu navigation.
    emit(currentB != currentA ? InputEventType::kRotateClockwise
                              : InputEventType::kRotateCounterClockwise);
    encoderA_ = currentA;
    encoderB_ = currentB;
  }

  pollButton(pins::kEncoderButton, primaryPrevious_, primaryPressedAt_,
             InputEventType::kPrimaryClick, InputEventType::kPrimaryLongPress);
  pollButton(pins::kUserButton, backPrevious_, backPressedAt_,
             InputEventType::kBackClick, InputEventType::kBackLongPress);
}
