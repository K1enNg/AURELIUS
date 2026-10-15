#include <math.h>
#include "ui/screens/screens.h"
#include "ui/ui.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "hw/sound.h"
#include "services/net.h"
#include "services/weather.h"
#include "config.h"

namespace screens {

// Menu tiles (2 columns x 3 rows). Icon index = tile index.
struct Tile { const char* label; uint16_t color; ui::Screen target; };
static const Tile TILES[] = {
  {"Clock",   TFT_CYAN,    ui::SCR_CLOCK},
  {"Weather", TFT_ORANGE,  ui::SCR_WEATHER},
  {"Lights",  TFT_YELLOW,  ui::SCR_LIGHTS},
  {"Sound",   TFT_GREEN,   ui::SCR_SOUND},
  {"Network", TFT_SKYBLUE, ui::SCR_NETWORK},
  {"System",  TFT_MAGENTA, ui::SCR_SYSTEM},
};
static const int TILE_COUNT = sizeof(TILES) / sizeof(TILES[0]);
static int s_index = 0;

static void drawIcon(int i, int cx, int cy, uint16_t c) {
  switch (i) {
    case 0:  // clock
      tft.drawCircle(cx, cy, 20, c);
      tft.drawCircle(cx, cy, 19, c);
      tft.drawLine(cx, cy, cx, cy - 13, c);
      tft.drawLine(cx, cy, cx + 9, cy + 4, c);
      break;
    case 1:  // weather (sun)
      tft.fillCircle(cx, cy, 9, c);
      for (int a = 0; a < 8; a++) {
        float r = a * PI / 4;
        tft.drawLine(cx + (int)(cosf(r) * 14), cy + (int)(sinf(r) * 14),
                     cx + (int)(cosf(r) * 21), cy + (int)(sinf(r) * 21), c);
      }
      break;
    case 2:  // lights (bulb)
      tft.fillCircle(cx, cy - 5, 14, c);
      tft.fillRect(cx - 7, cy + 8, 14, 5, c);
      tft.fillRect(cx - 4, cy + 14, 8, 4, c);
      break;
    case 3:  // sound (speaker)
      tft.fillRect(cx - 18, cy - 6, 9, 12, c);
      tft.fillTriangle(cx - 9, cy - 6, cx + 3, cy - 16, cx + 3, cy + 16, c);
      tft.fillTriangle(cx - 9, cy - 6, cx - 9, cy + 6, cx + 3, cy + 16, c);
      tft.drawLine(cx + 9, cy - 8, cx + 9, cy + 8, c);
      tft.drawLine(cx + 15, cy - 13, cx + 15, cy + 13, c);
      break;
    case 4:  // network (signal bars)
      for (int k = 0; k < 4; k++)
        tft.fillRect(cx - 20 + k * 11, cy + 16 - (k + 1) * 9, 8, (k + 1) * 9, c);
      break;
    case 5:  // system (chip)
      tft.fillRect(cx - 12, cy - 12, 24, 24, c);
      for (int k = 0; k < 3; k++) {
        int o = -8 + k * 8;
        tft.drawFastHLine(cx - 20, cy + o, 8, c);
        tft.drawFastHLine(cx + 12, cy + o, 8, c);
        tft.drawFastVLine(cx + o, cy - 20, 8, c);
        tft.drawFastVLine(cx + o, cy + 12, 8, c);
      }
      break;
  }
}

static void drawTile(int i, bool sel) {
  int x = 12 + (i % 2) * 154;
  int y = 68 + (i / 2) * 124;
  uint16_t base = TILES[i].color;
  uint16_t bg   = sel ? base : COL_TILE;
  uint16_t fg   = sel ? TFT_BLACK : base;
  uint16_t txt  = sel ? TFT_BLACK : TFT_WHITE;

  tft.fillRoundRect(x, y, 142, 112, 10, bg);
  if (!sel) tft.drawRoundRect(x, y, 142, 112, 10, COL_EDGE);
  drawIcon(i, x + 71, y + 44, fg);
  tft.setTextColor(txt, bg);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(TILES[i].label, x + 71, y + 90, 4);
}

static void drawTopBar(const tm& t, bool haveTime, bool wifi) {
  const weather::Data& w = weather::data();
  char buf[32];
  tft.fillRect(0, 0, SCREEN_W, BAR_H, COL_BAR);

  // row 1: time (left), WiFi indicator (right)
  if (haveTime) snprintf(buf, sizeof buf, "%02d:%02d", t.tm_hour, t.tm_min);
  else          snprintf(buf, sizeof buf, "--:--");
  tft.setTextColor(TFT_WHITE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, 12, 20, 4);

  tft.setTextColor(TFT_DARKGREY, COL_BAR);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("WiFi", 286, 20, 2);
  tft.fillCircle(300, 20, 5, wifi ? TFT_GREEN : TFT_RED);

  // row 2: weather (left), date (right)
  if (w.valid) snprintf(buf, sizeof buf, "%.0fC %s", w.temp, weather::codeToText(w.code));
  else         snprintf(buf, sizeof buf, "no weather");
  tft.setTextColor(TFT_ORANGE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(buf, 12, 45, 2);

  if (haveTime) {
    strftime(buf, sizeof buf, "%a %d %b", &t);
    tft.setTextColor(TFT_WHITE, COL_BAR);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(buf, SCREEN_W - 12, 45, 2);
  }
}

static void render(const tm& t, bool haveTime, bool full) {
  static int lastMinute = -2;
  static uint32_t lastRev = 0;
  static bool lastWifi = false;
  bool wifi = net::connected();
  int minute = haveTime ? t.tm_min : -1;
  uint32_t rev = weather::data().rev;

  if (full) {
    tft.fillScreen(TFT_BLACK);
    for (int i = 0; i < TILE_COUNT; i++) drawTile(i, i == s_index);
    widgets::drawHint("UP/DOWN: move", "SELECT: open");
    lastMinute = -2;  // force the top bar to draw
  }
  if (lastMinute != minute || lastRev != rev || lastWifi != wifi) {
    lastMinute = minute; lastRev = rev; lastWifi = wifi;
    drawTopBar(t, haveTime, wifi);
  }
}

static void onButton(Btn b) {
  if (b == B_UP || b == B_DOWN) {
    int old = s_index;
    s_index = (s_index + (b == B_DOWN ? 1 : TILE_COUNT - 1)) % TILE_COUNT;
    drawTile(old, false);
    drawTile(s_index, true);
    sound::beep(15);
  } else if (b == B_SELECT) {
    ui::goTo(TILES[s_index].target);
  }
}

const Def menuScreen = {render, onButton};

}
