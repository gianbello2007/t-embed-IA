#include "hardware/Storage.h"

#include <SD.h>
#include <SPI.h>

#include "BoardPins.h"

namespace {
constexpr const char* kDirectories[] = {
    "/T-EMBED-AI",          "/T-EMBED-AI/CHATS",  "/T-EMBED-AI/FILES",
    "/T-EMBED-AI/DOCUMENTS", "/T-EMBED-AI/PROJECTS", "/T-EMBED-AI/SCRIPTS",
    "/T-EMBED-AI/SCRIPTS/PYTHON", "/T-EMBED-AI/SCRIPTS/JAVASCRIPT",
    "/T-EMBED-AI/SCRIPTS/CPP", "/T-EMBED-AI/CONFIG", "/T-EMBED-AI/ASSETS",
    "/T-EMBED-AI/FONTS",    "/T-EMBED-AI/CACHE", "/T-EMBED-AI/LOGS",
    "/T-EMBED-AI/TEMP",     "/T-EMBED-AI/BACKUPS"};
}

bool Storage::begin() {
  digitalWrite(pins::kSdCs, HIGH);
  if (!SD.begin(pins::kSdCs, SPI, 25000000)) {
    mounted_ = false;
    lastError_ = "microSD not mounted";
    return false;
  }

  for (const char* path : kDirectories) {
    if (!SD.exists(path) && !SD.mkdir(path)) {
      mounted_ = false;
      lastError_ = "cannot create T-EMBED-AI directory";
      return false;
    }
  }

  mounted_ = true;
  lastError_ = "";
  return true;
}

bool Storage::isMounted() const {
  return mounted_;
}

uint64_t Storage::totalBytes() const {
  return mounted_ ? SD.totalBytes() : 0;
}

uint64_t Storage::usedBytes() const {
  return mounted_ ? SD.usedBytes() : 0;
}

const char* Storage::lastError() const {
  return lastError_;
}
