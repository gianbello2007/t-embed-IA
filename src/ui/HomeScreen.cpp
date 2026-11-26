#include "ui/HomeScreen.h"

#include <esp_heap_caps.h>

namespace {
constexpr uint16_t kBackground = TFT_BLACK;
constexpr uint16_t kSurface = TFT_DARKGREY;
constexpr uint16_t kText = TFT_WHITE;
constexpr uint16_t kMuted = TFT_LIGHTGREY;
constexpr uint16_t kAccent = 0x6D9F;  // blue-grey, readable on the IPS panel
constexpr uint16_t kGood = 0x05E0;
constexpr uint16_t kBad = 0xF800;
constexpr uint16_t kWarning = 0xFD20;

const char* pageName(HomeScreen::Page page) {
  switch (page) {
    case HomeScreen::Page::kSystem:
      return "Sistema";
    case HomeScreen::Page::kStorage:
      return "Arquivos";
    case HomeScreen::Page::kNetwork:
      return "Rede";
    case HomeScreen::Page::kHardware:
      return "Hardware";
    case HomeScreen::Page::kLauncher:
      return "T-Embed AI";
  }
  return "T-Embed AI";
}
}  // namespace

void HomeScreen::begin(Display& display) {
  display_ = &display;
  drawLauncher();
}

void HomeScreen::drawHeader(const char* title, const char* detail) {
  TFT_eSPI& tft = display_->tft();
  tft.fillScreen(kBackground);
  tft.fillRect(0, 0, tft.width(), 28, kSurface);
  tft.setTextColor(kText, kSurface);
  tft.setTextFont(2);
  tft.drawString(title, 10, 7);
  tft.setTextColor(kMuted, kSurface);
  tft.setTextFont(1);
  tft.drawRightString(detail, tft.width() - 8, 10);
}

void HomeScreen::drawFooter(const char* text) {
  TFT_eSPI& tft = display_->tft();
  tft.fillRect(0, tft.height() - 22, tft.width(), 22, kSurface);
  tft.setTextColor(kMuted, kSurface);
  tft.setTextFont(1);
  tft.drawCentreString(text, tft.width() / 2, tft.height() - 15);
}

void HomeScreen::drawStatusDot(int16_t x, int16_t y, bool active) {
  display_->tft().fillCircle(x, y, 4, active ? kGood : kBad);
}

void HomeScreen::drawLauncher() {
  TFT_eSPI& tft = display_->tft();
  drawHeader("T-Embed AI", "FASE 1");
  tft.setTextColor(kMuted, kBackground);
  tft.setTextFont(2);
  tft.drawString("Base operacional", 12, 38);

  const char* entries[] = {"Sistema", "Arquivos", "Rede", "Hardware"};
  for (uint8_t index = 0; index < 4; ++index) {
    const int16_t y = 61 + index * 20;
    const bool isSelected = index == selected_;
    if (isSelected) {
      tft.fillRoundRect(8, y - 2, tft.width() - 16, 18, 4, kAccent);
    }
    tft.setTextColor(isSelected ? kBackground : kText,
                     isSelected ? kAccent : kBackground);
    tft.setTextFont(2);
    tft.drawString(entries[index], 18, y);
  }
  drawFooter("Gire: navegar | Pressione: abrir");
}

void HomeScreen::drawSystem() {
  TFT_eSPI& tft = display_->tft();
  drawHeader(pageName(page_), "status real");
  char line[54];
  snprintf(line, sizeof(line), "Heap livre: %u KB", ESP.getFreeHeap() / 1024);
  tft.setTextColor(kText, kBackground);
  tft.setTextFont(2);
  tft.drawString(line, 12, 42);
  snprintf(line, sizeof(line), "PSRAM livre: %u KB",
           heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
  tft.drawString(line, 12, 67);
  tft.setTextColor(kMuted, kBackground);
  tft.setTextFont(1);
  tft.drawString("ESP32-S3 | 16 MB flash | OPI PSRAM", 12, 99);
  tft.drawString("USB CDC: habilitado para monitor", 12, 113);
  drawFooter("Botao usuario: voltar | longo: inicio");
}

void HomeScreen::drawStorage() {
  TFT_eSPI& tft = display_->tft();
  drawHeader(pageName(page_), "microSD");
  const bool mounted = storage_ != nullptr && storage_->isMounted();
  drawStatusDot(15, 48, mounted);
  tft.setTextColor(kText, kBackground);
  tft.setTextFont(2);
  tft.drawString(mounted ? "Cartao montado" : "Cartao indisponivel", 27, 40);
  if (mounted) {
    char line[50];
    const uint64_t totalMb = storage_->totalBytes() / (1024ULL * 1024ULL);
    const uint64_t usedMb = storage_->usedBytes() / (1024ULL * 1024ULL);
    snprintf(line, sizeof(line), "Uso: %llu / %llu MB", usedMb, totalMb);
    tft.drawString(line, 12, 69);
    tft.setTextColor(kMuted, kBackground);
    tft.setTextFont(1);
    tft.drawString("/T-EMBED-AI e subpastas criadas", 12, 104);
  } else {
    tft.setTextColor(kWarning, kBackground);
    tft.setTextFont(1);
    tft.drawString(storage_ == nullptr ? "servico nao iniciado" : storage_->lastError(), 12,
                   83);
    tft.setTextColor(kMuted, kBackground);
    tft.drawString("Insira um cartao e reinicie para montar", 12, 105);
  }
  drawFooter("Botao usuario: voltar | longo: inicio");
}

void HomeScreen::drawNetwork() {
  TFT_eSPI& tft = display_->tft();
  drawHeader(pageName(page_), "Wi-Fi");
  const bool online = wifi_ != nullptr && wifi_->connected();
  drawStatusDot(15, 48, online);
  tft.setTextColor(kText, kBackground);
  tft.setTextFont(2);
  tft.drawString(online ? "Conectado" : "Sem conexao", 27, 40);
  tft.setTextColor(kMuted, kBackground);
  tft.setTextFont(1);
  if (wifi_ == nullptr || !wifi_->configured()) {
    tft.drawString("Configuracao protegida por NVS", 12, 76);
    tft.drawString("A tela de credenciais entra na Fase 3.", 12, 92);
    tft.drawString("Nenhuma chave ou rede foi gravada no SD.", 12, 108);
  } else {
    char line[42];
    snprintf(line, sizeof(line), "RSSI: %ld dBm", static_cast<long>(wifi_->rssi()));
    tft.drawString(line, 12, 80);
  }
  drawFooter("Botao usuario: voltar | longo: inicio");
}

void HomeScreen::drawHardware() {
  TFT_eSPI& tft = display_->tft();
  drawHeader(pageName(page_), "sondagem I2C");
  const I2cDiagnostics empty{};
  const I2cDiagnostics& i2c = diagnostics_ == nullptr ? empty : diagnostics_->i2c();

  const struct {
    const char* name;
    bool detected;
  } entries[] = {{"PN532 (0x24)", i2c.pn532}, {"BQ27220 (0x55)", i2c.bq27220},
                 {"BQ25896 (0x6B)", i2c.bq25896}};
  for (uint8_t index = 0; index < 3; ++index) {
    const int16_t y = 43 + index * 24;
    drawStatusDot(16, y + 6, entries[index].detected);
    tft.setTextColor(kText, kBackground);
    tft.setTextFont(2);
    tft.drawString(entries[index].name, 28, y);
  }
  char line[30];
  snprintf(line, sizeof(line), "%u dispositivo(s) I2C", i2c.deviceCount);
  tft.setTextColor(kMuted, kBackground);
  tft.setTextFont(1);
  tft.drawString(line, 12, 120);
  drawFooter("Botao usuario: voltar | longo: inicio");
}

void HomeScreen::handle(const InputEvent& event) {
  if (page_ == Page::kLauncher) {
    if (event.type == InputEventType::kRotateClockwise) {
      selected_ = (selected_ + 1) % 4;
      drawLauncher();
    } else if (event.type == InputEventType::kRotateCounterClockwise) {
      selected_ = (selected_ + 3) % 4;
      drawLauncher();
    } else if (event.type == InputEventType::kPrimaryClick) {
      page_ = static_cast<Page>(selected_ + 1);
      refresh(*storage_, *diagnostics_, *wifi_);
    }
    return;
  }

  if (event.type == InputEventType::kBackClick ||
      event.type == InputEventType::kPrimaryLongPress ||
      event.type == InputEventType::kBackLongPress) {
    page_ = Page::kLauncher;
    drawLauncher();
  }
}

void HomeScreen::refresh(const Storage& storage, const Diagnostics& diagnostics,
                         const WiFiService& wifi) {
  storage_ = &storage;
  diagnostics_ = &diagnostics;
  wifi_ = &wifi;
  if (display_ == nullptr) {
    return;
  }
  lastRefresh_ = millis();
  switch (page_) {
    case Page::kLauncher:
      drawLauncher();
      break;
    case Page::kSystem:
      drawSystem();
      break;
    case Page::kStorage:
      drawStorage();
      break;
    case Page::kNetwork:
      drawNetwork();
      break;
    case Page::kHardware:
      drawHardware();
      break;
  }
}
