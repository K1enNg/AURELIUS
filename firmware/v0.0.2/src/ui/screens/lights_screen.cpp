#include "ui/screens/screens.h"
#include "ui/ui.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "hw/sound.h"
#include "services/lights.h"
#include "config.h"

namespace screens {

static const int SWATCH_X = SCREEN_W / 2, SWATCH_Y = 170, SWATCH_R = 70;

static uint8_t ledBits() {
  return (lights::channelOn(0) ? 1 : 0) | (lights::channelOn(1) ? 2 : 0) |
         (lights::channelOn(2) ? 4 : 0);
}

static void drawSwatch() {
  bool r = lights::channelOn(0), g = lights::channelOn(1), b = lights::channelOn(2);
  uint16_t c = rgb(r ? 255 : 0, g ? 255 : 0, b ? 255 : 0);
  tft.fillCircle(SWATCH_X, SWATCH_Y, SWATCH_R + 6, TFT_BLACK);
  if (r || g || b) tft.fillCircle(SWATCH_X, SWATCH_Y, SWATCH_R, c);
  tft.drawCircle(SWATCH_X, SWATCH_Y, SWATCH_R + 3, TFT_DARKGREY);
}

static void draw() {
  tft.fillScreen(TFT_BLACK);
  widgets::drawHeader("LIGHTS");
  drawSwatch();
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(lights::modeName(), SCREEN_W / 2, 290, 4);
  widgets::drawHint("UP/DOWN: change mode", "HOLD SELECT: back");
}

static void render(const tm&, bool, bool full) {
  static uint8_t lastBits = 0;
  uint8_t bits = ledBits();
  if (full) {
    draw();
    lastBits = bits;
  } else if (bits != lastBits) {   // e.g. party animation stepped
    lastBits = bits;
    drawSwatch();
  }
}

static void onButton(Btn b) {
  if (b != B_UP && b != B_DOWN) return;
  lights::cycle(b == B_DOWN ? 1 : -1);
  ui::requestRedraw();
  sound::beep(15);
}

const Def lightsScreen = {render, onButton};

}  // namespace screens
