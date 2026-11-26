#include "services/WiFiService.h"

#include <WiFi.h>

void WiFiService::begin() {
  // Credentials are intentionally not provisioned in firmware or on SD.  The
  // Settings app will use NVS in a later phase; until then the radio is off.
  WiFi.mode(WIFI_OFF);
}

bool WiFiService::configured() const {
  return false;
}

bool WiFiService::connected() const {
  return WiFi.status() == WL_CONNECTED;
}

int32_t WiFiService::rssi() const {
  return connected() ? WiFi.RSSI() : 0;
}
