#include "ui/screens/alarm_ring.h"
#include "hw/display.h"
#include "services/alarm_clock.h"
#include "config.h"

namespace screens {

static void drawBanner(bool on) {
  uint16_t bg = on ? TFT_RED : TFT_MAROON;
  tft.fillRect(0, 0, SCREEN_W, 80, bg);
  tft.setTextColor(TFT_WHITE, bg);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(2);
  tft.drawString("WAKE UP!", SCREEN_W / 2, 40, 4);
  tft.setTextSize(1);
}

static void drawHintLines() {
  char l1[40];
  snprintf(l1, sizeof l1, "Any button: snooze %u min", (unsigned)alarmclock::SNOOZE_MIN);
  tft.fillRect(0, 400, SCREEN_W, 80, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(l1, SCREEN_W / 2, 425, 2);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("HOLD SELECT: stop", SCREEN_W / 2, 450, 2);
}

void alarmRingDraw(const tm& t, bool haveTime, bool full) {
  static int lastMin = -1;
  static int lastBlink = -1;
  char buf[24];

  if (full) {
    tft.fillScreen(TFT_BLACK);
    drawHintLines();
    lastMin = -1;
    lastBlink = -1;
  }

  // banner flashes in step with the beeps
  int blink = (millis() / 500) % 2;
  if (blink != lastBlink) {
    lastBlink = blink;
    drawBanner(blink);
  }

  // current time + the alarm that is ringing
  int minute = haveTime ? t.tm_min : -1;
  if (minute != lastMin) {
    lastMin = minute;
    tft.fillRect(0, 130, SCREEN_W, 200, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    if (haveTime) snprintf(buf, sizeof buf, "%02d:%02d", t.tm_hour, t.tm_min);
    else          snprintf(buf, sizeof buf, "--:--");
    tft.drawString(buf, SCREEN_W / 2, 200, 7);

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof buf, "Alarm %02u:%02u", alarmclock::hour(), alarmclock::minute());
    tft.drawString(buf, SCREEN_W / 2, 290, 4);
  }
}

}