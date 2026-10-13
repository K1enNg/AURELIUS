#include "ui/screens/screens.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "services/net.h"
#include "services/timesvc.h"
#include "services/weather.h"
#include "config.h"

namespace screens {

// Shown until NTP delivers a real time: uptime counter + status.
static void drawWaiting() {
  tft.fillScreen(TFT_BLACK);
  widgets::drawHeader("CLOCK");
  widgets::drawHint("HOLD SELECT: back");

  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);

  unsigned long elapsed = timesvc::uptimeSec();
  char buf[24];
  snprintf(buf, sizeof(buf), "%02lu:%02lu:%02lu",
           (elapsed / 3600) % 100, (elapsed / 60) % 60, elapsed % 60);
  tft.drawString(buf, SCREEN_W / 2, 150, 7);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Waiting for NTP...", SCREEN_W / 2, 220, 2);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(net::connected() ? "WiFi connected" : "WiFi disconnected",
                 SCREEN_W / 2, 260, 2);
}

// Small weather summary under the clock
static void drawWeatherSummary() {
  const weather::Data& w = weather::data();
  tft.fillRect(0, 280, SCREEN_W, 90, TFT_BLACK);
  if (!w.valid) return;
  char buf[24];
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  snprintf(buf, sizeof buf, "%.1f C", w.temp);
  tft.drawString(buf, SCREEN_W / 2, 305, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(weather::codeToText(w.code), SCREEN_W / 2, 338, 4);
}

static void drawClock(const tm& t, bool full) {
  static int lastMin = -1, lastSec = -1;
  static uint32_t lastRev = 0xFFFFFFFF;
  char buf[16];

  if (full) {
    tft.fillScreen(TFT_BLACK);
    widgets::drawHeader("CLOCK");
    widgets::drawHint("HOLD SELECT: back");
    lastMin = lastSec = -1;
    lastRev = 0xFFFFFFFF;
  }
  tft.setTextDatum(MC_DATUM);

  if (t.tm_min != lastMin) {
    lastMin = t.tm_min;
    tft.fillRect(0, 85, SCREEN_W, 70, TFT_BLACK);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    snprintf(buf, sizeof buf, "%02d:%02d", t.tm_hour, t.tm_min);
    tft.drawString(buf, SCREEN_W / 2, 120, 7);

    strftime(buf, sizeof buf, "%a %d %b", &t);
    tft.fillRect(0, 175, SCREEN_W, 36, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(buf, SCREEN_W / 2, 192, 4);
  }
  if (t.tm_sec != lastSec) {
    lastSec = t.tm_sec;
    tft.fillRect(110, 225, 100, 30, TFT_BLACK);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    snprintf(buf, sizeof buf, "%02d s", t.tm_sec);
    tft.drawString(buf, SCREEN_W / 2, 240, 4);
  }
  uint32_t rev = weather::data().rev;
  if (lastRev != rev) {
    lastRev = rev;
    drawWeatherSummary();
  }
}

static void render(const tm& t, bool haveTime, bool full) {
  static bool needFull = true;            // first real-time frame after "waiting"
  static unsigned long lastWaitDraw = 0;

  if (!haveTime) {
    if (full || millis() - lastWaitDraw >= 1000) {
      drawWaiting();
      lastWaitDraw = millis();
    }
    needFull = true;
    return;
  }
  drawClock(t, full || needFull);
  needFull = false;
}

static void onButton(Btn) {}

const Def clockScreen = {render, onButton};

}  // namespace screens
