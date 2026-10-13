#include "ui/screens/screens.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "config.h"

namespace screens {

static unsigned long s_lastUpdate = 0;

static void draw(bool full) {
  if (full) {
    tft.fillScreen(TFT_BLACK);
    widgets::drawHeader("SYSTEM");
    widgets::drawHint("HOLD SELECT: back");
  }
  char buf[32];
  snprintf(buf, sizeof buf, "%s r%d", ESP.getChipModel(), (int)ESP.getChipRevision());
  widgets::drawInfoRow(56, "Chip", buf);
  snprintf(buf, sizeof buf, "%u MHz", (unsigned)getCpuFrequencyMhz());
  widgets::drawInfoRow(120, "CPU speed", buf);
  snprintf(buf, sizeof buf, "%u KB", (unsigned)(ESP.getFreeHeap() / 1024));
  widgets::drawInfoRow(184, "Free heap", buf);
  snprintf(buf, sizeof buf, "%u MB", (unsigned)(ESP.getFlashChipSize() / (1024 * 1024)));
  widgets::drawInfoRow(248, "Flash", buf);
  unsigned long s = millis() / 1000;
  snprintf(buf, sizeof buf, "%02lu:%02lu:%02lu", s / 3600, (s / 60) % 60, s % 60);
  widgets::drawInfoRow(312, "Uptime", buf);
}

static void render(const tm&, bool, bool full) {
  if (full || millis() - s_lastUpdate > 1000) {
    draw(full);
    s_lastUpdate = millis();
  }
}

static void onButton(Btn) {}

const Def systemScreen = {render, onButton};

}