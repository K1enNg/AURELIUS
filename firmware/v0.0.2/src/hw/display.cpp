#include "hw/display.h"
#include "config.h"

TFT_eSPI tft;

void displayBegin() {
  tft.init();
  tft.setRotation(ROTATION);
  tft.fillScreen(TFT_BLACK);
}
