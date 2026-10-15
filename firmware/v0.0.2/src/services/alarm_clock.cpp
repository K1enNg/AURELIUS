#include "services/alarm_clock.h"
#include <Preferences.h>

namespace alarmclock {
namespace {

constexpr int      BUZZER_PIN    = 15;
constexpr uint32_t BEEP_MS       = 90;                 // each beep (blocks only this long)
constexpr uint32_t BEEP_GAP_MS   = 220;                // gap between the 3 beeps of a group
constexpr uint32_t GROUP_GAP_MS  = 1200;               // pause between groups
constexpr uint32_t AUTO_STOP_MS  = 5UL * 60UL * 1000UL; // give up if nobody reacts

Preferences prefs;
uint8_t  hr = 6, mn = 0;
bool     on = false;

bool     isRinging = false, isSnoozing = false;
uint32_t ringStart = 0, nextBeepAt = 0, snoozeUntil = 0;
int      beepsInGroup = 0;
int32_t  lastTriggerKey = -1;   // (day, hour, minute) of the last trigger, so one minute rings once

void save() {
  prefs.begin("alarm", false);
  prefs.putUChar("h", hr);
  prefs.putUChar("m", mn);
  prefs.putBool("on", on);
  prefs.end();
}

void buzz(uint32_t ms) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}

void startRinging(uint32_t now) {
  isRinging = true;
  ringStart = now;
  nextBeepAt = now;
  beepsInGroup = 0;
}

// A changed schedule cancels any snooze and lets the new time fire right away
void scheduleChanged() {
  isSnoozing = false;
  lastTriggerKey = -1;
  on = true;
  save();
}

}

void begin() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  prefs.begin("alarm", false);
  hr = prefs.getUChar("h", 6);
  mn = prefs.getUChar("m", 0);
  on = prefs.getBool("on", false);
  prefs.end();
  if (hr > 23) hr = 6;
  if (mn > 59) mn = 0;
}

void update(const tm& t, bool haveTime) {
  uint32_t now = millis();

  if (isRinging) {
    if (now - ringStart >= AUTO_STOP_MS) { isRinging = false; return; }
    if (now >= nextBeepAt) {
      buzz(BEEP_MS);
      if (++beepsInGroup >= 3) { beepsInGroup = 0; nextBeepAt = now + GROUP_GAP_MS; }
      else                     { nextBeepAt = now + BEEP_GAP_MS; }
    }
    return;
  }

  if (isSnoozing) {
    if ((int32_t)(now - snoozeUntil) >= 0) { isSnoozing = false; startRinging(now); }
    return;
  }

  if (!on || !haveTime) return;
  if (t.tm_hour == hr && t.tm_min == mn) {
    int32_t key = t.tm_yday * 1440 + t.tm_hour * 60 + t.tm_min;
    if (key != lastTriggerKey) {
      lastTriggerKey = key;
      startRinging(now);
    }
  }
}

bool    enabled() { return on; }
uint8_t hour()    { return hr; }
uint8_t minute()  { return mn; }

void adjustHour(int dir) {
  hr = (uint8_t)(((int)hr + dir + 24) % 24);
  scheduleChanged();
}

void adjustMinute(int dir, int step) {
  mn = (uint8_t)((((int)mn + dir * step) % 60 + 60) % 60);
  scheduleChanged();
}

void setEnabled(bool v) {
  on = v;
  if (!on) { isSnoozing = false; }
  else     { lastTriggerKey = -1; }
  save();
}

void toggle() { setEnabled(!on); }

bool ringing()  { return isRinging; }
bool snoozing() { return isSnoozing; }

uint32_t snoozeLeftS() {
  if (!isSnoozing) return 0;
  int32_t left = (int32_t)(snoozeUntil - millis());
  return left > 0 ? (uint32_t)(left + 999) / 1000 : 0;
}

void snooze() {
  isRinging = false;
  isSnoozing = true;
  snoozeUntil = millis() + (uint32_t)SNOOZE_MIN * 60000UL;
}

void dismiss() {
  isRinging = false;
  isSnoozing = false;
}
}