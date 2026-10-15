#include "ui/screens/screens.h"
#include "ui/widgets.h"
#include "hw/display.h"
#include "services/net.h"
#include "services/timesvc.h"
#include "services/alarm_clock.h"
#include "config.h"

namespace screens {

// Alarm minutes change in steps of this size (the buttons have no auto-repeat).
// Set to 1 for fine control.
static constexpr int MINUTE_STEP = 5;

// Alarm area layout (portrait): rows 376..437, the two-line hint sits below at 440.
static constexpr int kAlarmTop = 376;
static constexpr int kAlarmH   = 62;
static constexpr int kHintTop  = 440;

enum class Mode : uint8_t { View, EditHour, EditMinute, EditEnabled };
static Mode mode = Mode::View;
static bool timeValid = false;   // set by render(), read by onButton()

// ---------------------------------------------------------------- waiting
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

// ---------------------------------------------------------------- clock face
static void drawClock(const tm& t, bool full) {
  static int lastMin = -1, lastSec = -1;
  char buf[16];

  if (full) {
    tft.fillScreen(TFT_BLACK);
    widgets::drawHeader("CLOCK");     // hint is drawn by drawAlarm() (it depends on the mode)
    lastMin = lastSec = -1;
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
}

// ---------------------------------------------------------------- alarm area
// Two-line hint (drawn here so the text can follow the editing mode)
static void drawHint2(const char* l1, const char* l2) {
  tft.fillRect(0, kHintTop, SCREEN_W, 40, TFT_BLACK);
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(l1, SCREEN_W / 2, kHintTop + 12, 2);
  tft.drawString(l2, SCREEN_W / 2, kHintTop + 30, 2);
}

static void drawAlarmHint() {
  switch (mode) {
    case Mode::View:        drawHint2("SELECT: set alarm   UP/DOWN: on/off", "HOLD SELECT: back"); break;
    case Mode::EditHour:    drawHint2("UP/DOWN: hour   SELECT: next",        "HOLD SELECT: back"); break;
    case Mode::EditMinute:  drawHint2("UP/DOWN: minutes   SELECT: next",     "HOLD SELECT: back"); break;
    case Mode::EditEnabled: drawHint2("UP/DOWN: on/off   SELECT: done",      "HOLD SELECT: back"); break;
  }
}

// One editable box: highlighted when it is the active field
static void drawField(int cx, int w, const char* txt, bool active) {
  uint16_t bg = active ? TFT_YELLOW : TFT_BLACK;
  uint16_t fg = active ? TFT_BLACK  : TFT_WHITE;
  tft.fillRoundRect(cx - w / 2, 386, w, 38, 6, bg);
  if (!active) tft.drawRoundRect(cx - w / 2, 386, w, 38, 6, TFT_DARKGREY);
  tft.setTextColor(fg, bg);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(txt, cx, 406, 4);
}

static void drawAlarmSummary() {
  char buf[32];
  uint16_t color;
  if (alarmclock::snoozing()) {
    uint32_t left = alarmclock::snoozeLeftS();
    snprintf(buf, sizeof buf, "Snooze %u:%02u", (unsigned)(left / 60), (unsigned)(left % 60));
    color = TFT_YELLOW;
  } else if (alarmclock::enabled()) {
    snprintf(buf, sizeof buf, "Alarm %02u:%02u", alarmclock::hour(), alarmclock::minute());
    color = TFT_GREEN;
  } else {
    snprintf(buf, sizeof buf, "Alarm %02u:%02u off", alarmclock::hour(), alarmclock::minute());
    color = TFT_DARKGREY;
  }
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(color, TFT_BLACK);
  tft.drawString(buf, SCREEN_W / 2, 406, 4);
}

static void drawAlarmEditor() {
  char buf[8];
  snprintf(buf, sizeof buf, "%02u", alarmclock::hour());
  drawField(78, 56, buf, mode == Mode::EditHour);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(":", 128, 404, 4);

  snprintf(buf, sizeof buf, "%02u", alarmclock::minute());
  drawField(178, 56, buf, mode == Mode::EditMinute);

  drawField(250, 70, alarmclock::enabled() ? "ON" : "OFF", mode == Mode::EditEnabled);
}

// Redraws only when something the area shows has changed
static void drawAlarm(bool full) {
  static uint32_t lastSig = 0xFFFFFFFF;
  static int lastMode = -1;

  if (full) { lastSig = 0xFFFFFFFF; lastMode = -1; }
  if ((int)mode != lastMode) {
    lastMode = (int)mode;
    drawAlarmHint();
    lastSig = 0xFFFFFFFF;
  }

  uint32_t sig = ((uint32_t)mode << 28) |
                 ((uint32_t)alarmclock::enabled() << 27) |
                 ((uint32_t)alarmclock::hour() << 22) |
                 ((uint32_t)alarmclock::minute() << 16) |
                 (alarmclock::snoozeLeftS() & 0x1FF);
  if (sig == lastSig) return;
  lastSig = sig;

  tft.fillRect(0, kAlarmTop, SCREEN_W, kAlarmH, TFT_BLACK);
  if (mode == Mode::View) drawAlarmSummary();
  else                    drawAlarmEditor();
}

// ---------------------------------------------------------------- screen API
static void render(const tm& t, bool haveTime, bool full) {
  static bool needFull = true;            // first real-time frame after "waiting"
  static unsigned long lastWaitDraw = 0;
  static unsigned long lastCall = 0;

  // render() runs every loop while this screen is open, so a long gap means the
  // screen was left and re-entered: start in normal view again.
  if (millis() - lastCall > 500) mode = Mode::View;
  lastCall = millis();
  timeValid = haveTime;

  if (!haveTime) {
    if (full || millis() - lastWaitDraw >= 1000) {
      drawWaiting();
      lastWaitDraw = millis();
    }
    needFull = true;
    return;
  }
  bool fullNow = full || needFull;
  drawClock(t, fullNow);
  needFull = false;
  drawAlarm(fullNow);
}

// SELECT walks hour -> minutes -> on/off -> done. UP/DOWN change the active field.
// Every change is saved immediately, so leaving mid-edit keeps what you set.
static void onButton(Btn b) {
  if (!timeValid) return;

  switch (mode) {
    case Mode::View:
      if (b == B_SELECT)                   mode = Mode::EditHour;
      else if (b == B_UP || b == B_DOWN)   alarmclock::toggle();
      break;

    case Mode::EditHour:
      if (b == B_UP)          alarmclock::adjustHour(+1);
      else if (b == B_DOWN)   alarmclock::adjustHour(-1);
      else if (b == B_SELECT) mode = Mode::EditMinute;
      break;

    case Mode::EditMinute:
      if (b == B_UP)          alarmclock::adjustMinute(+1, MINUTE_STEP);
      else if (b == B_DOWN)   alarmclock::adjustMinute(-1, MINUTE_STEP);
      else if (b == B_SELECT) mode = Mode::EditEnabled;
      break;

    case Mode::EditEnabled:
      if (b == B_UP || b == B_DOWN) alarmclock::toggle();
      else if (b == B_SELECT)       mode = Mode::View;
      break;
  }
}

const Def clockScreen = {render, onButton};

}  // namespace screens