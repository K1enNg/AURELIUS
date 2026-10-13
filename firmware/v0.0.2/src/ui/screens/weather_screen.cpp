#include "ui/screens/screens.h"
#include "ui/ui.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "services/lights.h"
#include "services/net.h"
#include "services/weather.h"
#include "config.h"

namespace screens {

static void draw() {
  const weather::Data& w = weather::data();
  tft.fillScreen(TFT_BLACK);
  widgets::drawHeader("WEATHER");
  widgets::drawHint("SELECT: refresh", "HOLD SELECT: back");

  if (!w.valid) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("No weather data yet", SCREEN_W / 2, 240, 4);
    return;
  }

  char buf[32];

  // big temperature number (font 7) + unit (font 4), centred together
  snprintf(buf, sizeof buf, "%.1f", w.temp);
  int tw = tft.textWidth(buf, 7);
  int cw = tft.textWidth("C", 4);
  int x0 = (SCREEN_W - (tw + 10 + cw)) / 2;
  tft.setTextColor(TFT_ORANGE, TFT_BLACK);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, x0, 100, 7);
  tft.drawString("C", x0 + tw + 10, 100, 4);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(weather::codeToText(w.code), SCREEN_W / 2, 165, 4);

  // 3-day list
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("HIGH / LOW", 300, 210, 2);

  const char* labels[3] = {"Today", "Tomorrow", "Day 3"};
  for (int i = 0; i < 3; i++) {
    int yc = 245 + i * 52;
    tft.drawFastHLine(16, yc - 26, SCREEN_W - 32, COL_EDGE);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(labels[i], 20, yc, 4);
    snprintf(buf, sizeof buf, "%.0f / %.0f", w.hi[i], w.lo[i]);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(buf, 300, yc, 4);
  }
}

static void render(const tm&, bool, bool full) {
  if (full) draw();
}

static void onButton(Btn b) {
  if (b != B_SELECT) return;
  if (!net::connected()) {
    widgets::drawHint("WiFi not connected");
    return;
  }
  widgets::drawHint("Refreshing...");
  if (weather::refreshNow()) lights::onWeatherChanged();
  ui::requestRedraw();
}

const Def weatherScreen = {render, onButton};

}
