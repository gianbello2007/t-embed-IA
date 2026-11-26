#include "core/System.h"

#include <esp_heap_caps.h>

#include "core/Event.h"

void System::begin() {
  board_.begin();
  Serial.println("\nT-Embed AI: Fase 1 foundation booting");
  Serial.printf("PSRAM: %s, %u KB free\n", psramFound() ? "found" : "not found",
                heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);

  spiBus_.begin();
  events_ = xQueueCreate(16, sizeof(InputEvent));
  input_.begin(events_);

  rgb_.begin();
  display_.begin();

  const bool storageAvailable = storage_.begin();
  diagnostics_.begin();
  wifi_.begin();

  home_.begin(display_);
  home_.refresh(storage_, diagnostics_, wifi_);
  rgb_.setStatus(storageAvailable ? LedStatus::kReady : LedStatus::kWarning);

  Serial.printf("microSD: %s\n", storageAvailable ? "mounted" : storage_.lastError());
  const I2cDiagnostics& i2c = diagnostics_.i2c();
  Serial.printf("I2C: %u devices; PN532=%d BQ27220=%d BQ25896=%d\n", i2c.deviceCount,
                i2c.pn532, i2c.bq27220, i2c.bq25896);
}

void System::tick() {
  input_.poll();
  InputEvent event{};
  while (events_ != nullptr && xQueueReceive(events_, &event, 0) == pdTRUE) {
    home_.handle(event);
  }

  // Rendering is event-driven.  A modest refresh keeps the real heap/SD/Wi-Fi
  // information current without blocking input or running a busy redraw loop.
  static uint32_t lastRefresh = 0;
  if (millis() - lastRefresh >= 1000) {
    lastRefresh = millis();
    home_.refresh(storage_, diagnostics_, wifi_);
  }
  delay(4);
}
