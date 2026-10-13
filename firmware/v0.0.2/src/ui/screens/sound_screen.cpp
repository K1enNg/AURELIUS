#include "ui/screens/screens.h"
#include "ui/ui.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "hw/sound.h"
#include "config.h"

namespace screens {

static void draw() {
  bool muted = sound::isMuted();
  tft.fillScreen(TFT_BLACK);
  widgets::drawHeader("SOUND");
  tft.setTextColor(muted ? TFT_RED : TFT_GREEN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString(muted ? "MUTED" : "ON", SCREEN_W / 2, 200, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("Beeps on button presses", SCREEN_W / 2, 265, 2);
  widgets::drawHint("SELECT: mute   UP/DOWN: test beep", "HOLD SELECT: back");
}

static void render(const tm&, bool, bool full) {
  if (full) draw();
}

static void onButton(Btn b) {
  if (b == B_SELECT) {
    sound::toggleMute();
    ui::requestRedraw();
    sound::beep(100);  // audible only when just unmuted
  } else if (b == B_UP || b == B_DOWN) {
    sound::beep(60);
  }
}

const Def soundScreen = {render, onButton};

}  // namespace screens
