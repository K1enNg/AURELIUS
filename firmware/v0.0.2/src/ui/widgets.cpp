#include "ui/widgets.h"
#include "hw/display.h"
#include "config.h"

namespace widgets {

void drawHeader(const char* title) {
  tft.fillRect(0, 0, SCREEN_W, 40, COL_BAR);
  tft.setTextColor(TFT_WHITE, COL_BAR);
  tft.setTextDatum(ML_DATUM);
  tft.drawString(title, 12, 20, 4);
}

void drawHint(const char* l1, const char* l2) {
  tft.fillRect(0, HINT_Y, SCREEN_W, SCREEN_H - HINT_Y, TFT_BLACK);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  if (l2) {
    tft.drawString(l1, SCREEN_W / 2, HINT_Y + 12, 2);
    tft.drawString(l2, SCREEN_W / 2, HINT_Y + 30, 2);
  } else {
    tft.drawString(l1, SCREEN_W / 2, HINT_Y + 20, 2);
  }
}

void drawInfoRow(int y, const char* label, const String& value) {
  tft.fillRect(0, y, SCREEN_W, 56, TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString(label, 16, y, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(value.c_str(), 16, y + 20, 4);
}

String clip(String s, size_t n) {
  if (s.length() > n) s = s.substring(0, n - 1) + "~";
  return s;
}

}
